#include "causalDiscovery.h"
#include "statistic.h"
#include "graph.h"
#include "dataset.h"
#include <memory>
#include <stack>
#include <set>
#include <stdexcept>

void CausalDiscovery::createFullyConnectedGraph(Graph &graph, const Dataset &data)
{
    for (int i = 0; i < graph.getNumVertices(); ++i)
    {
        for (int j = i + 1; j < graph.getNumVertices(); ++j)
        {
            graph.addEdge(i, j, false); // Assuming the graph is undirected
                                        // Note: we pass j, i in case of undirected graph inside addEdge function
            //graph.addEdge(j, i, false); // Add the reverse edge as well
        }
    }
}

std::shared_ptr<Column> CausalDiscovery::getColumnData(const Dataset &data, int index) const
{
    std::shared_ptr<Column> column = data.getColumn(index);
    if (!column)
    {
        throw std::runtime_error("Invalid column index");
    }
    return column;
}

bool CausalDiscovery::testConditionalIndependence(const Dataset &data, int i, int j, const std::set<int> &conditioningSet, double alpha)
{
    std::shared_ptr<Column> data_i = getColumnData(data, i);
    std::shared_ptr<Column> data_j = getColumnData(data, j);

    std::vector<std::shared_ptr<Column>> data_conditioningSet;
    for (int k : conditioningSet)
    {
        std::shared_ptr<Column> column_k = getColumnData(data, k);
        if (column_k)
        {
            data_conditioningSet.push_back(column_k);
        }
        else
        {
            throw std::runtime_error("Invalid column data");
        }
    }

    // Perform the statistical test
    double p_value = Statistic::performStatisticalTest(data_i, data_j, data_conditioningSet);

    // Return true if the p-value is greater than the significance level alpha
    return p_value > alpha;
}

void CausalDiscovery::addToConditioningSet(std::set<int> &conditioningSet, int numVertices, int i, int j)
{
    for (int k = 0; k < numVertices; ++k)
    {
        // Don't add i or j to the conditioning set
        if (k == i || k == j)
        {
            continue;
        }

        // If k is not already in the conditioning set, add it
        if (conditioningSet.find(k) == conditioningSet.end())
        {
            conditioningSet.insert(k);
            break;
        }
    }
}

void CausalDiscovery::applyPCAlgorithm(Graph &graph, const Dataset &data, double alpha)
{
    int numVertices = graph.getNumVertices();

    // For each pair of vertices
    for (int i = 0; i < numVertices; ++i)
    {
        for (int j = i + 1; j < numVertices; ++j) // Only process the edge (i, j) if i < j
        {
            // Start with an empty set of conditioning variables
            std::set<int> conditioningSet;

            // While there are still vertices to condition on
            while (conditioningSet.size() < numVertices - 2)
            {
                // Test if i and j are conditionally independent given the conditioning set
                bool independent = testConditionalIndependence(data, i, j, conditioningSet, alpha);

                // If i and j are conditionally independent, remove the edge between them and break the loop
                if (independent)
                {
                    graph.removeEdge(i, j);
                    graph.removeEdge(j, i); // Also remove the edge in the opposite direction
                    break;
                }
                // If i and j are not conditionally independent, add a new vertex to the conditioning set
                else
                {
                    addToConditioningSet(conditioningSet, numVertices, i, j);
                }
            }
        }
    }
}

void CausalDiscovery::pruneGraph(Graph &graph, const Dataset &data, double alpha)
{
    for (int X = 0; X < graph.getNumVertices(); ++X)
    {
        auto neighbors = graph.getNeighbors(X);
        for (int i = 0; i < neighbors.size(); ++i)
        {
            for (int j = i + 1; j < neighbors.size(); ++j)
            {
                int Y = neighbors[i];
                int Z = neighbors[j];
                bool independent = testConditionalIndependence(data, Y, Z, {X}, alpha);
                if (independent)
                {
                    graph.removeEdge(Y, Z);
                }
            }
        }
    }
}

void CausalDiscovery::orientVStructures(Graph &graph, const Dataset &data, double alpha)
{
    for (int Z = 0; Z < graph.getNumVertices(); ++Z)
    {
        auto neighbors = graph.getNeighbors(Z);
        for (int i = 0; i < neighbors.size(); ++i)
        {
            for (int j = i + 1; j < neighbors.size(); ++j)
            {
                int X = neighbors[i];
                int Y = neighbors[j];
                if (!graph.hasEdge(X, Y))
                {
                    bool independent = testConditionalIndependence(data, X, Y, {Z}, alpha);
                    if (!independent)
                    {
                        graph.orientEdge(X, Z);
                        graph.orientEdge(Y, Z);
                    }
                }
            }
        }
    }
}

std::vector<std::vector<int>> CausalDiscovery::powerSet(const std::vector<int> &originalSet)
{
    std::vector<std::vector<int>> resultSet;
    std::vector<int> emptySet;
    resultSet.push_back(emptySet);

    for (int i = 0; i < originalSet.size(); i++)
    {
        std::vector<std::vector<int>> tempSet = resultSet;

        for (auto &set : tempSet)
        {
            set.push_back(originalSet[i]);
        }

        for (auto &set : tempSet)
        {
            resultSet.push_back(set);
        }
    }

    return resultSet;
}

bool CausalDiscovery::areDSeparated(int node1, int node2, const std::vector<int> &conditioningSet, const Graph &graph)
{
    std::vector<bool> visited(graph.getNumVertices(), false);

    // Mark nodes in the conditioning set as visited
    for (int node : conditioningSet)
    {
        visited[node] = true;
    }

    // Start DFS from node1
    std::stack<int> stack;
    stack.push(node1);

    while (!stack.empty())
    {
        int node = stack.top();
        stack.pop();

        if (node == node2)
        {
            return false; // There's a path from node1 to node2 that doesn't pass through any node in the conditioning set
        }

        if (!visited[node])
        {
            visited[node] = true;

            for (int neighbor : graph.getNeighbors(node))
            {
                if (!visited[neighbor])
                {
                    stack.push(neighbor);
                }
            }
        }
    }

    return true; // No such path exists, so node1 and node2 are d-separated
}

std::set<std::pair<int, int>> CausalDiscovery::identifyPossibleDSep(const Graph &graph)
{
    std::set<std::pair<int, int>> dsepSet;

    // Get the number of vertices in the graph
    int numVertices = graph.getNumVertices();

    // Initialize a vector to hold the nodes, with a size equal to the number of vertices
    std::vector<int> nodes(numVertices);

    // Assign each node a unique identifier (in this case, the index of the node)
    for (int i = 0; i < numVertices; ++i)
    {
        nodes[i] = i;
    }

    // For each pair of nodes in the graph
    for (int i = 0; i < nodes.size(); ++i)
    {
        for (int j = i + 1; j < nodes.size(); ++j)
        {
            // Get the set of all other nodes
            std::vector<int> otherNodes = nodes;
            otherNodes.erase(otherNodes.begin() + i);
            otherNodes.erase(otherNodes.begin() + j - 1);

            // For each subset of other nodes
            for (auto &subset : powerSet(otherNodes))
            {
                // If node1 and node2 are d-separated by the subset, add them to the dsep set
                if (areDSeparated(nodes[i], nodes[j], subset, graph))
                {
                    dsepSet.insert({nodes[i], nodes[j]});
                }
            }
        }
    }

    return dsepSet;
}

void CausalDiscovery::applyFCIRules(Graph &graph, const Dataset &data, double alpha, const std::set<std::pair<int, int>> &possibleDSep)
{
    // For each edge in the graph
    for (const auto &edge : graph.getEdges())
    {
        int node1 = std::get<0>(edge);
        int node2 = std::get<1>(edge);
        bool isOriented = std::get<2>(edge);

        // If the pair of nodes is in the set of possibly d-separated pairs
        if (possibleDSep.find(std::make_pair(node1, node2)) != possibleDSep.end())
        {
            // Apply the FCI rules to orient the edge
            // ...
        }
    }
}

void CausalDiscovery::finalOrientation(Graph &graph, const Dataset &data)
{
    // For each edge in the graph
    for (const auto &edge : graph.getEdges())
    {
        int src = std::get<0>(edge);
        int dest = std::get<1>(edge);
        bool isOriented = std::get<2>(edge);

        // If the edge is not oriented
        if (!isOriented)
        {
            // Orient the edge based on some criterion
            // For example, you might use a statistical test to determine the direction of causality
            // Here we'll just orient the edge from the lower-indexed vertex to the higher-indexed vertex
            if (src < dest)
            {
                graph.orientEdge(src, dest);
            }
            else
            {
                graph.orientEdge(dest, src);
            }
        }
    }
}

std::unique_ptr<Graph> CausalDiscovery::FCI(const Dataset &data, double alpha)
{
    std::unique_ptr<Graph> graph = std::make_unique<Graph>(data.getNumOfColumns());
    graph->printGraph();

    // Step 1
    createFullyConnectedGraph(*graph, data);
    graph->printGraph();

    // Step 2
    applyPCAlgorithm(*graph, data, alpha);
    graph->printGraph();

    // Step 3
    pruneGraph(*graph, data, alpha);
    graph->printGraph();

    // Step 4
    orientVStructures(*graph, data, alpha);
    graph->printGraph();

    // Step 5
    std::set<std::pair<int, int>> possibleDSep = identifyPossibleDSep(*graph);
    applyFCIRules(*graph, data, alpha, possibleDSep);
    graph->printGraph();

    // Step 6
    finalOrientation(*graph, data);
    graph->printGraph();

    return graph;
}

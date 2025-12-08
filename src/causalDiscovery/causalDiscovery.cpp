#include "causalDiscovery.h"
#include "statistic.h"
#include "graph.h"
#include "dataset.h"
#include <memory>
#include <stack>
#include <set>
#include <stdexcept>
#include <iostream>

void CausalDiscovery::createFullyConnectedGraph(std::shared_ptr<Graph> graph)
{
    if (!graph)
    {
        throw std::runtime_error("Graph is nullptr");
    }

    for (int i = 0; i < graph->getNumVertices(); ++i)
    {
        for (int j = i + 1; j < graph->getNumVertices(); ++j)
        {
            graph->addDoubleDirectedEdge(i, j);
        }
    }
}

void CausalDiscovery::applyForbiddenEdges(std::shared_ptr<Graph> graph) {
    const auto& forbiddenEdges = graph->getForbiddenEdges();

    for (const auto& edge : forbiddenEdges) {
        int from = edge.first;
        int to = edge.second;

        // Remove the directed edge if it exists
        if (graph->hasDirectedEdge(from, to)) {
            graph->removeSingleEdge(from, to);
        }
        if (graph->hasDirectedEdge(to, from)) {
            graph->removeSingleEdge(to, from);
        }
    }
}

void CausalDiscovery::enforceRequiredEdges(std::shared_ptr<Graph> graph) {
    const auto& requiredEdges = graph->getRequiredEdges();

    for (const auto& edge : requiredEdges) {
        int from = edge.first;
        int to = edge.second;

        // Ensure the required edge exists (add if missing)
        if (!graph->hasDirectedEdge(from, to) && !graph->hasDoubleDirectedEdge(from, to)) {
            graph->addDirectedEdge(from, to);
        }
    }
}

void CausalDiscovery::addToConditioningSet(std::set<int> &conditioningSet, int numVertices, int i, int j)
{
    for (int k = 0; k < numVertices; ++k)
    {
        if (k == i || k == j || conditioningSet.find(k) != conditioningSet.end())
        {
            continue;
        }

        conditioningSet.insert(k);
        return;
    }

    throw std::runtime_error("No valid vertex to add to the conditioning set");
}

void CausalDiscovery::applyPCAlgorithm(std::shared_ptr<Graph> graph, double alpha)
{
    /* ...  iteratively increasing the size of the conditioning set and removing edges when independence is detected...*/

    int numVertices = graph->getNumVertices();
    std::shared_ptr<const Dataset> data = graph->getDataset();

    for (int i = 0; i < numVertices; ++i)
    {
        for (int j = i + 1; j < numVertices; ++j)
        {
            std::set<int> conditioningSet;

            while (conditioningSet.size() < numVertices - 2)
            {
                double p_value = Statistic::testConditionalIndependence(data, i, j, conditioningSet);
                bool independent = p_value > alpha;

                // TODO: add domain-specific rules whether remove edge or not

                if (independent)
                {
                    graph->removeSingleEdge(i, j);
                    graph->removeSingleEdge(j, i);
                    break;
                }
                else
                {
                    try
                    {
                        addToConditioningSet(conditioningSet, numVertices, i, j);
                    }
                    catch (const std::runtime_error &e)
                    {
                        std::cerr << e.what() << std::endl;
                        break;
                    }
                }
            }
        }
    }
}

void CausalDiscovery::pruneGraph(std::shared_ptr<Graph> graph, double alpha)
{
    std::shared_ptr<const Dataset> data = graph->getDataset();

    for (int conditioningNode = 0; conditioningNode < graph->getNumVertices(); ++conditioningNode)
    {
        auto neighbors = graph->getNeighbors(conditioningNode);
        for (int i = 0; i < neighbors.size(); ++i)
        {
            for (int j = i + 1; j < neighbors.size(); ++j)
            {
                int firstNeighbor = neighbors[i];
                int secondNeighbor = neighbors[j];

                // Check if an edge exists between firstNeighbor and secondNeighbor before running the independence test
                if (graph->hasDoubleDirectedEdge(firstNeighbor, secondNeighbor))
                {
                    double p_value = Statistic::testConditionalIndependence(data, firstNeighbor, secondNeighbor, { conditioningNode });
                    bool independent = p_value > alpha;

                    // TODO: add domain-specific rules whether remove edge or not

                    if (independent) 
                    {
                        graph->removeSingleEdge(firstNeighbor, secondNeighbor);
                    }
                }
            }
        }
    }
}

void CausalDiscovery::orientVStructures(std::shared_ptr<Graph> graph, double alpha)
{
    std::shared_ptr<const Dataset> data = graph->getDataset();

    for (int Z = 0; Z < graph->getNumVertices(); ++Z)
    {
        auto neighbors = graph->getNeighbors(Z);
        for (int i = 0; i < neighbors.size(); ++i)
        {
            for (int j = i + 1; j < neighbors.size(); ++j)
            {
                int X = neighbors[i];
                int Y = neighbors[j];
                if (!graph->hasDoubleDirectedEdge(X, Y))
                {
                    double p_value = Statistic::testConditionalIndependence(data, X, Y, {Z});
                    bool independent = p_value > alpha;

                    // TODO: maybe we could use domain-specific rules to orient the edges
                    // but now, I wouldn't change on this

                    if (!independent)
                    {
                        //graph->orientEdge(X, Z);
                        //graph->orientEdge(Y, Z);

                        graph->removeSingleEdge(Z, X);
                        graph->removeSingleEdge(Z, Y);
                    }
                }
            }
        }
    }
}

std::vector<std::vector<int>> CausalDiscovery::powerSet(const std::vector<int> &originalSet)
{
    std::vector<std::vector<int>> resultSet;
    resultSet.push_back({});

    for (int elem : originalSet)
    {
        std::vector<std::vector<int>> newSets;
        for (auto &subset : resultSet)
        {
            std::vector<int> newSubset = subset;
            newSubset.push_back(elem);
            newSets.push_back(newSubset);
        }
        resultSet.insert(resultSet.end(), newSets.begin(), newSets.end());
    }

    return resultSet;
}

bool CausalDiscovery::areDSeparated(std::shared_ptr<Graph> graph, int node1, int node2, const std::vector<int> &conditioningSet)
{
    std::vector<bool> visited(graph->getNumVertices(), false);

    for (int node : conditioningSet)
    {
        visited[node] = true;
    }

    std::stack<int> stack;
    stack.push(node1);

    while (!stack.empty())
    {
        int node = stack.top();
        stack.pop();

        if (node == node2)
        {
            return false;
        }

        if (!visited[node])
        {
            visited[node] = true;
            for (int neighbor : graph->getNeighbors(node))
            {
                if (!visited[neighbor])
                {
                    stack.push(neighbor);
                }
            }
        }
    }

    return true;
}

std::set<std::pair<int, int>> CausalDiscovery::identifyPossibleDSep(std::shared_ptr<Graph> graph)
{
    std::set<std::pair<int, int>> dsepSet;
    int numVertices = graph->getNumVertices();
    std::vector<int> nodes(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        nodes[i] = i;
    }

    for (int i = 0; i < nodes.size(); ++i)
    {
        for (int j = i + 1; j < nodes.size(); ++j)
        {
            std::vector<int> otherNodes;
            for (int k = 0; k < nodes.size(); ++k)
            {
                if (k != i && k != j)
                {
                    otherNodes.push_back(nodes[k]);
                }
            }

            for (auto &subset : powerSet(otherNodes))
            {
                if (areDSeparated(graph, nodes[i], nodes[j], subset))
                {
                    dsepSet.insert({nodes[i], nodes[j]});
                }
            }
        }
    }

    return dsepSet;
}

bool CausalDiscovery::shouldOrientEdge(std::shared_ptr<Graph> graph, int node1, int node2, double alpha) {
    // Placeholder for actual FCI rule logic
    // For example, you might check for certain conditions in the graph
    // and decide whether to orient the edge from node1 to node2
    // This is a simplified example and should be replaced with actual FCI rules

    // Example condition: If node1 has a directed edge to node2, orient the edge
    if (graph->hasDirectedEdge(node1, node2)) {
        return true;
    }

    // Example condition: If node2 has a directed edge to node1, orient the edge in the opposite direction
    if (graph->hasDoubleDirectedEdge(node2, node1)) {
        //graph->orientEdge(node2, node1);
        graph->removeSingleEdge(node1, node2);
        return false;
    }

    // Add more FCI rules as needed
    return false;
}

void CausalDiscovery::applyFCIRules(std::shared_ptr<Graph> graph, double alpha, const std::set<std::pair<int, int>> &possibleDSep)
{
    for (const auto& edge : graph->getEdges()) {
        int node1 = std::get<0>(edge);
        int node2 = std::get<1>(edge);
        bool isOriented = std::get<2>(edge);

        if (possibleDSep.find({ node1, node2 }) != possibleDSep.end()) {
            // Apply FCI rules to orient the edge
            if (!isOriented) {
                // Example FCI rule: Orient the edge based on some condition
                // This is a placeholder. Replace with actual FCI rule application.
                if (shouldOrientEdge(graph, node1, node2, alpha)) {
                    //graph->orientEdge(node1, node2);
                    graph->removeSingleEdge(node2, node1);
                }
            }
        }
    }
}

void CausalDiscovery::applyDirectionConstraints(std::shared_ptr<Graph> graph) {
    const auto& constraints = graph->getDirectionConstraints();

    for (const auto& constraint : constraints) {
        int from = constraint.first;
        int to = constraint.second;

        // If an undirected edge exists, orient it according to constraint
        if (graph->hasDoubleDirectedEdge(from, to)) {
            graph->removeSingleEdge(to, from);
        }
        // If directed edge exists in wrong direction, flip it
        else if (graph->hasDirectedEdge(to, from)) {
            graph->removeSingleEdge(to, from);
            graph->addDirectedEdge(from, to);
        }
    }
}

void CausalDiscovery::finalOrientation(std::shared_ptr<Graph> graph)
{
    // First apply direction constraints
    applyDirectionConstraints(graph);

    // Iterate over each edge in the graph
    for (const auto &edge : graph->getEdges())
    {
        int src = std::get<0>(edge);
        int dest = std::get<1>(edge);
        bool isOriented = std::get<2>(edge);

        // Orient the edge if it is not already oriented
        if (!isOriented)
        {
            // Check if there's a direction constraint
            if (graph->hasDirectionConstraint(src, dest)) {
                graph->removeSingleEdge(dest, src);
            }
            else if (graph->hasDirectionConstraint(dest, src)) {
                graph->removeSingleEdge(src, dest);
            }
            else {
                // Default: Orient from lower to higher index
                if (src < dest)
                {
                    graph->removeSingleEdge(dest, src);
                }
                else
                {
                    graph->removeSingleEdge(src, dest);
                }
            }
        }
    }
}

void CausalDiscovery::runFCI(std::shared_ptr<Graph> graph, double alpha)
{
    // Step 1: create fully connected graph, remove forbidden edges, add required edges
    createFullyConnectedGraph(graph);
    applyForbiddenEdges(graph);
    enforceRequiredEdges(graph);

    // Step 2
    applyPCAlgorithm(graph, alpha);

    // Step 3
    pruneGraph(graph, alpha);

    // Re-enforce required edges after pruning
    enforceRequiredEdges(graph);

    // Step 4
    orientVStructures(graph, alpha);

    // Step 5
    std::set<std::pair<int, int>> possibleDSep = identifyPossibleDSep(graph);
    applyFCIRules(graph, alpha, possibleDSep);

    // Step 6
    finalOrientation(graph);
}

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
            graph->addUndirectedEdge(i, j);
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
                double p_value = Statistic::performStatisticalTest(*data, i, j, conditioningSet);
                bool independent = p_value > alpha;

                if (independent)
                {
                    graph->removeUndirectedEdge(i, j);
                    graph->removeUndirectedEdge(j, i);
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

    for (int X = 0; X < graph->getNumVertices(); ++X)
    {
        auto neighbors = graph->getNeighbors(X);
        for (int i = 0; i < neighbors.size(); ++i)
        {
            for (int j = i + 1; j < neighbors.size(); ++j)
            {
                int Y = neighbors[i];
                int Z = neighbors[j];
                double p_value = Statistic::performStatisticalTest(*data, Y, Z, {X});
                bool independent = p_value > alpha;
                if (independent)
                {
                    graph->removeUndirectedEdge(Y, Z);
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
                if (!graph->hasUndirectedEdge(X, Y))
                {
                    double p_value = Statistic::performStatisticalTest(*data, X, Y, {Z});
                    bool independent = p_value > alpha;
                    if (!independent)
                    {
                        graph->orientEdge(X, Z);
                        graph->orientEdge(Y, Z);
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

void CausalDiscovery::applyFCIRules(std::shared_ptr<Graph> graph, double alpha, const std::set<std::pair<int, int>> &possibleDSep)
{
    for (const auto& edge : graph->getEdges())
    {
        int node1 = std::get<0>(edge);
        int node2 = std::get<1>(edge);
        bool isOriented = std::get<2>(edge);

        if (possibleDSep.find({ node1, node2 }) != possibleDSep.end())
        {
            // Apply FCI rules to orient the edge
            // This is a placeholder. Replace with actual FCI rule application.
            if (!isOriented)
            {
                // Example placeholder for an FCI rule
                graph->orientEdge(node1, node2);
            }
        }
    }
}

void CausalDiscovery::finalOrientation(std::shared_ptr<Graph> graph)
{
    // Iterate over each edge in the graph
    for (const auto &edge : graph->getEdges())
    {
        int src = std::get<0>(edge);
        int dest = std::get<1>(edge);
        bool isOriented = std::get<2>(edge);

        // Orient the edge if it is not already oriented
        if (!isOriented)
        {
            // Orient the edge from the lower-indexed vertex to the higher-indexed vertex
            if (src < dest)
            {
                graph->orientEdge(src, dest);
            }
            else
            {
                graph->orientEdge(dest, src);
            }
        }
    }
}

void CausalDiscovery::runFCI(std::shared_ptr<Graph> graph, double alpha)
{

    // Step 1
    createFullyConnectedGraph(graph);
    graph->printGraph();

    // Step 2
    applyPCAlgorithm(graph, alpha);
    graph->printGraph();

    // Step 3
    pruneGraph(graph, alpha);
    graph->printGraph();

    // Step 4
    orientVStructures(graph, alpha);
    graph->printGraph();

    // Step 5
    std::set<std::pair<int, int>> possibleDSep = identifyPossibleDSep(graph);
    applyFCIRules(graph, alpha, possibleDSep);
    graph->printGraph();

    // Step 6
    finalOrientation(graph);
    graph->printGraph();
}

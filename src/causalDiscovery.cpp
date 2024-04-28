#include "causalDiscovery.h"
#include "statistic.h"
#include "graph.h"
#include "dataset.h"
#include <memory>
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
    // Access the data for variables i, j, and the conditioning set directly from the dataset
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

void CausalDiscovery::orientVStructures(Graph &graph, const Dataset &data, double alpha)
{
    // Implementation goes here
}

void CausalDiscovery::identifyPossibleDSep(Graph &graph, const Dataset &data)
{
    // Implementation goes here
}

void CausalDiscovery::applyFCIRules(Graph &graph, const Dataset &data, double alpha)
{
    // Implementation goes here
}

void CausalDiscovery::finalOrientation(Graph &graph, const Dataset &data)
{
    // Implementation goes here
}

std::unique_ptr<Graph> CausalDiscovery::FCI(const Dataset &data, double alpha)
{
    std::unique_ptr<Graph> graph = std::make_unique<Graph>(data.getNumOfColumns());
    graph->printGraph();

    createFullyConnectedGraph(*graph, data);
    graph->printGraph();

    applyPCAlgorithm(*graph, data, alpha);
    graph->printGraph();

    orientVStructures(*graph, data, alpha);
    identifyPossibleDSep(*graph, data);
    applyFCIRules(*graph, data, alpha);
    finalOrientation(*graph, data);

    return graph;
}

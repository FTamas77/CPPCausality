#include "causalDiscovery.h"
#include <memory>

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

void CausalDiscovery::applyPCAlgorithm(Graph &graph, const Dataset &data, double alpha)
{
    // Implementation goes here
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

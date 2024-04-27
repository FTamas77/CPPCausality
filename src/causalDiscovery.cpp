#include "causalDiscovery.h"
#include <memory>

void CausalDiscovery::createFullyConnectedGraph(Graph &graph, const Dataset &data)
{
    // Implement this function
}

void CausalDiscovery::orientVStructures(Graph &graph, const Dataset &data, double alpha)
{
    // Implement this function
}

void CausalDiscovery::applyFCIRules(Graph &graph, const Dataset &data, double alpha)
{
    // Implement this function
}

void CausalDiscovery::finalOrientation(Graph &graph, const Dataset &data)
{
    // Implement this function
}

std::unique_ptr<Graph> CausalDiscovery::FCI(const Dataset &data, double alpha)
{

    std::unique_ptr<Graph> graph = std::make_unique<Graph>(data.getNumOfColumns());
    createFullyConnectedGraph(*graph, data);
    orientVStructures(*graph, data, alpha);
    applyFCIRules(*graph, data, alpha);
    finalOrientation(*graph, data);
    return graph;
}

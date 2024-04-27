#ifndef CAUSALDISCOVERY_H
#define CAUSALDISCOVERY_H

#include "Graph.h"
#include "Dataset.h"
#include <memory>

class CausalDiscovery
{
private:
    void createFullyConnectedGraph(Graph &graph, const Dataset &data);
    void orientVStructures(Graph &graph, const Dataset &data, double alpha);
    void applyFCIRules(Graph &graph, const Dataset &data, double alpha);
    void finalOrientation(Graph &graph, const Dataset &data);

public:
    std::unique_ptr<Graph> FCI(const Dataset &data, double alpha);
};

#endif // CAUSALDISCOVERY_H
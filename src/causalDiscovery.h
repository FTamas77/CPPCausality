#ifndef CAUSALDISCOVERY_H
#define CAUSALDISCOVERY_H

#include "Graph.h"
#include "Dataset.h"
#include <memory>
#include <set>

class CausalDiscovery
{
    void createFullyConnectedGraph(Graph &graph, const Dataset &data);

    std::shared_ptr<Column> getColumnData(const Dataset &data, int index) const;
    bool testConditionalIndependence(const Dataset &data, int i, int j, const std::set<int> &conditioningSet, double alpha);

    void addToConditioningSet(std::set<int> &conditioningSet, int numVertices, int i, int j);
    void applyPCAlgorithm(Graph &graph, const Dataset &data, double alpha);

    void orientVStructures(Graph &graph, const Dataset &data, double alpha);
    void identifyPossibleDSep(Graph &graph, const Dataset &data);
    void applyFCIRules(Graph &graph, const Dataset &data, double alpha);
    void finalOrientation(Graph &graph, const Dataset &data);

public:
    std::unique_ptr<Graph> FCI(const Dataset &data, double alpha);
};

#endif // CAUSALDISCOVERY_H
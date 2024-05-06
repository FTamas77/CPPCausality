#ifndef CAUSALDISCOVERY_H
#define CAUSALDISCOVERY_H

#include "Graph.h"
#include "Dataset.h"
#include <memory>
#include <set>

class CausalDiscovery
{
    // Step 1
    void createFullyConnectedGraph(Graph &graph, const Dataset &data);

    // Step 2
    std::shared_ptr<Column> getColumnData(const Dataset &data, int index) const;
    bool testConditionalIndependence(const Dataset &data, int i, int j, const std::set<int> &conditioningSet, double alpha);
    void addToConditioningSet(std::set<int> &conditioningSet, int numVertices, int i, int j);
    void applyPCAlgorithm(Graph &graph, const Dataset &data, double alpha);

    // Step 3
    void pruneGraph(Graph &graph, const Dataset &data, double alpha);

    // Step 4
    void orientVStructures(Graph &graph, const Dataset &data, double alpha);

    // Step 5
    std::vector<std::vector<int>> powerSet(const std::vector<int> &originalSet);
    bool areDSeparated(int node1, int node2, const std::vector<int> &conditioningSet, const Graph &graph);
    std::set<std::pair<int, int>> identifyPossibleDSep(const Graph &graph);
    void applyFCIRules(Graph &graph, const Dataset &data, double alpha, const std::set<std::pair<int, int>> &possibleDSep);

    // Step 6
    void finalOrientation(Graph &graph, const Dataset &data);

public:
    std::unique_ptr<Graph> FCI(const Dataset &data, double alpha);
};

#endif // CAUSALDISCOVERY_H
#ifndef CAUSALDISCOVERY_H
#define CAUSALDISCOVERY_H

#include "Graph.h"
#include "Dataset.h"
#include <memory>
#include <set>

class CausalDiscovery
{
    // Step 1
    void createFullyConnectedGraph(std::shared_ptr<Graph> graph);

    // Step 2
    void addToConditioningSet(std::set<int> &conditioningSet, int numVertices, int i, int j);

    void applyPCAlgorithm(std::shared_ptr<Graph> graph, double alpha);

    // Step 3
    void pruneGraph(std::shared_ptr<Graph> graph, double alpha);

    // Step 4
    void orientVStructures(std::shared_ptr<Graph> graph, double alpha);

    // Step 5
    std::vector<std::vector<int>> powerSet(const std::vector<int> &originalSet);

    bool areDSeparated(std::shared_ptr<Graph> graph, int node1, int node2, const std::vector<int> &conditioningSet);

    std::set<std::pair<int, int>> identifyPossibleDSep(std::shared_ptr<Graph> graph);

    void applyFCIRules(std::shared_ptr<Graph> graph, double alpha, const std::set<std::pair<int, int>> &possibleDSep);

    // Step 6
    void finalOrientation(std::shared_ptr<Graph> graph);

public:
    void runFCI(std::shared_ptr<Graph> data, double alpha);
};

#endif // CAUSALDISCOVERY_H
#ifndef GRAPH_H
#define GRAPH_H

#include "dataset.h"
#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <iostream>
#include <stdexcept>

class Graph
{
public:
    using NeighborSet = std::unordered_set<int>;

    Graph() = delete;

    explicit Graph(std::shared_ptr<Dataset> dataset);

    std::shared_ptr<const Dataset> getDataset() const;

    void addDirectedEdge(int src, int dest);
    void addDoubleDirectedEdge(int src, int dest);

    bool hasDirectedEdge(int src, int dest) const;
    bool hasDoubleDirectedEdge(int src, int dest) const;

    void removeSingleEdge(int src, int dest);

    size_t getNumVertices() const;

    std::vector<int> getNeighbors(int vertex) const;

    void printGraph() const;

    void compareGraphs(const Graph& other) const;

    std::vector<std::tuple<int, int, bool>> getEdges() const;

    friend bool operator==(const Graph& lhs, const Graph& rhs);

    friend bool operator==(const std::shared_ptr<Graph>& lhs, const std::shared_ptr<Graph>& rhs);

    // Forbidden edges
    void addForbiddenEdge(int from, int to);
    bool isForbiddenEdge(int from, int to) const;
    const std::vector<std::pair<int, int>>& getForbiddenEdges() const;

    // Required edges
    void addRequiredEdge(int from, int to);
    bool isRequiredEdge(int from, int to) const;
    const std::vector<std::pair<int, int>>& getRequiredEdges() const;

    // Direction constraints (if edge exists, enforce direction)
    void addDirectionConstraint(int from, int to);
    bool hasDirectionConstraint(int from, int to) const;
    const std::vector<std::pair<int, int>>& getDirectionConstraints() const;

private:
    std::vector<NeighborSet> m_adjList;
    std::shared_ptr<Dataset> m_dataset;

    std::vector<std::pair<int, int>> forbiddenEdges;
    std::vector<std::pair<int, int>> requiredEdges;
    std::vector<std::pair<int, int>> directionConstraints;
};

#endif // GRAPH_H

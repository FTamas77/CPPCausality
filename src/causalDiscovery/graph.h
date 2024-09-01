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

    //bool hasEdge(int src, int dest) const;
    bool hasDirectedEdge(int src, int dest) const;
    bool hasDoubleDirectedEdge(int src, int dest) const;

    void removeSingleEdge(int src, int dest);

    size_t getNumVertices() const;

    std::vector<int> getNeighbors(int vertex) const;

    //void orientEdge(int src, int dest);

    void printGraph() const;

    void compareGraphs(const Graph& other) const;

    std::vector<std::tuple<int, int, bool>> getEdges() const;

    friend bool operator==(const Graph& lhs, const Graph& rhs);

    friend bool operator==(const std::shared_ptr<Graph>& lhs, const std::shared_ptr<Graph>& rhs);

    // TODO: move to the cpp
    void addForbiddenEdge(int from, int to) {
        forbiddenEdges.emplace_back(from, to);
    }

    bool isForbiddenEdge(int from, int to) const {
        for (const auto& edge : forbiddenEdges) {
            if (edge.first == from && edge.second == to) {
                return true;
            }
        }
        return false;
    }

    const std::vector<std::pair<int, int>>& getForbiddenEdges() const {
        return forbiddenEdges;
    }

private:
    std::vector<NeighborSet> m_adjList;

    std::shared_ptr<Dataset> m_dataset;

    std::vector<std::pair<int, int>> forbiddenEdges;
};

#endif // GRAPH_H

#ifndef GRAPH_H
#define GRAPH_H

#include "dataset.h"
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
#include <iostream>
#include <stdexcept>

class Graph
{
public:
    using EdgeList = std::list<int>;

    Graph() = delete;

    explicit Graph(std::shared_ptr<Dataset> dataset);

    std::shared_ptr<const Dataset> getDataset() const;

    void addDirectedEdge(int src, int dest);
    void addUndirectedEdge(int src, int dest);

    bool hasDirectedEdge(int src, int dest) const;
    bool hasUndirectedEdge(int src, int dest) const;

    // TODO: maybe removeEdge(int src, int dest) instead of removeDirectedEdge and removeUndirectedEdge
    void removeDirectedEdge(int src, int dest);
    void removeUndirectedEdge(int src, int dest);

    size_t getNumVertices() const;

    std::vector<int> getNeighbors(int vertex) const;

    void orientEdge(int src, int dest);

    void printGraph() const;

    std::vector<std::tuple<int, int, bool>> getEdges() const;

    friend bool operator==(const Graph& lhs, const Graph& rhs);

    friend bool operator==(const std::shared_ptr<Graph>& lhs, const std::shared_ptr<Graph>& rhs);

private:
    std::vector<EdgeList> m_adjList;

    std::shared_ptr<Dataset> m_dataset;
};

#endif // GRAPH_H

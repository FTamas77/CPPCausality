#ifndef GRAPH_H
#define GRAPH_H

#include "dataset.h"

#include <vector>
#include <list>

class Graph
{
    std::vector<std::list<std::pair<int, bool>>> m_adjList; // The adjacency list

public:
    Graph() : Graph(0) {}

    Graph(int numVertices);

    void addEdge(int src, int dest, bool isDirected);

    const std::list<std::pair<int, bool>> &getAdjVertices(int vertex) const;

    size_t getNumVertices() const;

    void printGraph() const;

    void removeEdge(int src, int dest);

    std::vector<int> getNodes() const;

    std::vector<int> getNeighbors(int vertex) const;

    bool hasEdge(int src, int dest) const;

    void orientEdge(int src, int dest);

    std::vector<std::tuple<int, int, bool>> getEdges() const;
};

bool operator==(const Graph &lhs, const Graph &rhs);

#endif
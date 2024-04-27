#ifndef GRAPH_H
#define GRAPH_H

#include "dataset.h"

#include <vector>
#include <list>

class Graph
{
private:
    int numVertices;                                      // Number of vertices in the graph
    std::vector<std::list<std::pair<int, bool>>> adjList; // The adjacency list

public:
    Graph(int numVertices);

    void addEdge(int src, int dest, bool isDirected);

    const std::list<std::pair<int, bool>> &getAdjVertices(int vertex) const;
};

#endif
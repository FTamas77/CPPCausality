#include "graph.h"

Graph::Graph(int numVertices) : numVertices(numVertices), adjList(numVertices) {}

void Graph::addEdge(int src, int dest, bool isDirected)
{
    adjList[src].push_back({dest, isDirected}); // Add an edge from src to dest
    if (!isDirected)
    {
        adjList[dest].push_back({src, isDirected}); // Add an edge from dest to src for undirected graph
    }
}

const std::list<std::pair<int, bool>> &Graph::getAdjVertices(int vertex) const
{
    return adjList[vertex]; // Get the adjacent vertices of a given vertex
}
#include "graph.h"
#include <iostream>

Graph::Graph(int numVertices) : m_adjList(numVertices) {}

void Graph::addEdge(int src, int dest, bool isDirected)
{
    m_adjList[src].push_back({dest, isDirected}); // Add an edge from src to dest
    if (!isDirected)
    {
        m_adjList[dest].push_back({src, isDirected}); // Add an edge from dest to src for undirected graph
    }
}

const std::list<std::pair<int, bool>> &Graph::getAdjVertices(int vertex) const
{
    return m_adjList[vertex]; // Get the adjacent vertices of a given vertex
}

int Graph::getNumVertices() const
{
    return m_adjList.size();
}

void Graph::printGraph() const
{
    for (int i = 0; i < m_adjList.size(); ++i)
    {
        std::cout << "Vertex: " << i << ":\n";
        for (const auto &edge : m_adjList[i])
        {
            if (edge.second)
            {
                std::cout << " | "  << i << " --> " << edge.first;
            } else {
                std::cout << " | " << i << " --- " << edge.first;
            }
        }
        std::cout << "\n";
    }
}
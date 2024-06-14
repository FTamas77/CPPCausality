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

size_t Graph::getNumVertices() const
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
                std::cout << " | "
                          << " --> " << edge.first;
            }
            else
            {
                std::cout << " | "
                          << " --- " << edge.first;
            }
        }
        std::cout << "\n";
    }
}

void Graph::removeEdge(int src, int dest)
{
    // Remove the edge from src to dest
    m_adjList[src].remove_if([dest](const std::pair<int, bool> &edge) {
        return edge.first == dest;
    });

    // Remove the edge from dest to src
    m_adjList[dest].remove_if([src](const std::pair<int, bool> &edge) {
        return edge.first == src;
    });
}

std::vector<int> Graph::getNodes() const
{
    std::vector<int> nodes;
    for (int i = 0; i < m_adjList.size(); ++i)
    {
        nodes.push_back(i);
    }
    return nodes;
}

std::vector<int> Graph::getNeighbors(int vertex) const
{
    std::vector<int> neighbors;
    for (const auto &pair : m_adjList[vertex])
    {
        neighbors.push_back(pair.first);
    }
    return neighbors;
}

bool Graph::hasEdge(int src, int dest) const
{
    for (const auto &pair : m_adjList[src])
    {
        if (pair.first == dest)
        {
            return true;
        }
    }
    return false;
}

void Graph::orientEdge(int src, int dest)
{
    for (auto &pair : m_adjList[src])
    {
        if (pair.first == dest)
        {
            pair.second = true; // Make the edge directed
            break;
        }
    }
}

std::vector<std::tuple<int, int, bool>> Graph::getEdges() const
{
    std::vector<std::tuple<int, int, bool>> edges;
    for (int i = 0; i < m_adjList.size(); ++i)
    {
        for (const auto &pair : m_adjList[i])
        {
            edges.push_back({i, pair.first, pair.second});
        }
    }
    return edges;
}

bool operator==(const Graph &lhs, const Graph &rhs)
{
    // Compare the number of vertices
    if (lhs.getNumVertices() != rhs.getNumVertices())
    {
        return false;
    }

    // Compare the edges
    auto lhsEdges = lhs.getEdges();
    auto rhsEdges = rhs.getEdges();
    if (lhsEdges.size() != rhsEdges.size())
    {
        return false;
    }
    for (size_t i = 0; i < lhsEdges.size(); ++i)
    {
        if (lhsEdges[i] != rhsEdges[i])
        {
            return false;
        }
    }

    return true;
}
#include "graph.h"
#include <memory>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <iostream> 

Graph::Graph(std::shared_ptr<Dataset> dataset) : m_dataset(std::move(dataset))
{
    if (!m_dataset) {
        throw std::invalid_argument("Dataset cannot be null");
    }
    int numVertices = m_dataset->getNumOfColumns();
    m_adjList.resize(numVertices);
}

std::shared_ptr<const Dataset> Graph::getDataset() const {
    return m_dataset;
}

void Graph::addDirectedEdge(int src, int dest) {
    if (src >= 0 && src < m_adjList.size()) {
        m_adjList[src].insert(dest);
    }
}

void Graph::addUndirectedEdge(int src, int dest) {
    if (src >= 0 && src < m_adjList.size() && dest >= 0 && dest < m_adjList.size()) {
        m_adjList[src].insert(dest);
        m_adjList[dest].insert(src);
    }
}

bool Graph::hasEdge(int src, int dest) const {
    if (src >= 0 && src < m_adjList.size() && dest >= 0 && dest < m_adjList.size()) {
        return m_adjList[src].find(dest) != m_adjList[src].end();
    }
    return false;
}

bool Graph::hasDirectedEdge(int src, int dest) const {
    if (src >= 0 && src < m_adjList.size()) {
        return m_adjList[src].find(dest) != m_adjList[src].end();
    }

    return false;
}

bool Graph::hasUndirectedEdge(int src, int dest) const {
    if (src >= 0 && src < m_adjList.size() && dest >= 0 && dest < m_adjList.size()) {
        bool srcToDest = m_adjList[src].find(dest) != m_adjList[src].end();
        bool destToSrc = m_adjList[dest].find(src) != m_adjList[dest].end();

        return srcToDest && destToSrc;
    }
    return false;
}

void Graph::removeEdge(int src, int dest) {
    if (src >= 0 && src < m_adjList.size() && dest >= 0 && dest < m_adjList.size()) {
        m_adjList[src].erase(dest);
        if (m_adjList[dest].find(src) != m_adjList[dest].end()) {
            // If the reverse edge exists, it's an undirected edge
            m_adjList[dest].erase(src);
        }
    }
}

size_t Graph::getNumVertices() const {
    return m_adjList.size();
}

std::vector<int> Graph::getNeighbors(int vertex) const {
    if (vertex >= 0 && vertex < m_adjList.size()) {
        return std::vector<int>(m_adjList[vertex].begin(), m_adjList[vertex].end());
    }

    return {};
}

void Graph::orientEdge(int src, int dest) {
    if (src >= 0 && src < m_adjList.size() && dest >= 0 && dest < m_adjList.size()) {
        m_adjList[dest].erase(src);
    }
}

void Graph::printGraph() const {
    for (int i = 0; i < m_adjList.size(); ++i) {
        std::cout << "Vertex: " << i << ":\n";

        for (const auto& dest : m_adjList[i]) {
            if (m_adjList[dest].find(i) != m_adjList[dest].end()) {
                // There is an edge from dest to i as well, so it's undirected
                std::cout << " | --- " << dest << " (undirected)" << std::endl;
            }
            else {
                // There is no edge from dest to i, so it's directed
                std::cout << " | --> " << dest << " (directed)" << std::endl;
            }
        }

        std::cout << "\n";
    }
}

void Graph::compareGraphs(const Graph& other) const {
    std::set<std::pair<int, int>> edges1, edges2;

    // Collect edges from the current graph
    for (int src = 0; src < getNumVertices(); ++src) {
        for (int dest : getNeighbors(src)) {
            edges1.insert({ src, dest });
        }
    }

    // Collect edges from the other graph
    for (int src = 0; src < other.getNumVertices(); ++src) {
        for (int dest : other.getNeighbors(src)) {
            edges2.insert({ src, dest });
        }
    }

    // Find additional edges in the current graph
    std::vector<std::pair<int, int>> additionalInCurrent;
    for (const auto& edge : edges1) {
        if (edges2.find(edge) == edges2.end()) {
            additionalInCurrent.push_back(edge);
        }
    }

    // Find additional edges in the other graph
    std::vector<std::pair<int, int>> additionalInOther;
    for (const auto& edge : edges2) {
        if (edges1.find(edge) == edges1.end()) {
            additionalInOther.push_back(edge);
        }
    }

    // Find common edges
    std::vector<std::pair<int, int>> commonEdges;
    for (const auto& edge : edges1) {
        if (edges2.find(edge) != edges2.end()) {
            commonEdges.push_back(edge);
        }
    }

    // Print results
    std::cout << "Additional edges in the current graph:\n";
    for (const auto& edge : additionalInCurrent) {
        std::cout << edge.first << " -> " << edge.second << "\n";
    }

    std::cout << "Additional edges in the other graph:\n";
    for (const auto& edge : additionalInOther) {
        std::cout << edge.first << " -> " << edge.second << "\n";
    }

    std::cout << "Common edges in both graphs:\n";
    for (const auto& edge : commonEdges) {
        std::cout << edge.first << " -> " << edge.second << "\n";
    }
}

std::vector<std::tuple<int, int, bool>> Graph::getEdges() const {
    std::vector<std::tuple<int, int, bool>> edges;
    std::set<std::pair<int, int>> addedEdges;

    for (int src = 0; src < m_adjList.size(); ++src) {
        for (const auto& dest : m_adjList[src]) {

            // Check if the edge or its reverse has already been added
            if (addedEdges.find({ src, dest }) != addedEdges.end() || addedEdges.find({ dest, src }) != addedEdges.end()) {
                continue;
            }

            // Determine if the edge is directed
            bool isDirected = hasDirectedEdge(src, dest) && !hasDirectedEdge(dest, src);

            // Add the edge to the result
            edges.emplace_back(src, dest, isDirected);

            // Mark this edge as added
            addedEdges.insert({ src, dest });
        }
    }
    return edges;
}

bool operator==(const Graph& lhs, const Graph& rhs) {
    if (lhs.getNumVertices() != rhs.getNumVertices()) {
        return false;
    }

    for (size_t i = 0; i < lhs.m_adjList.size(); ++i) {
        if (lhs.m_adjList[i] != rhs.m_adjList[i]) {
            return false;
        }
    }

    return true;
}

bool operator==(const std::shared_ptr<Graph>& lhs, const std::shared_ptr<Graph>& rhs) {
    if (lhs == nullptr || rhs == nullptr) {
        return lhs == rhs;
    }
    return *lhs == *rhs;
}

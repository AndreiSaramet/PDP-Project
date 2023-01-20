#include "Graph.h"
#include "../merge-sort/merge_sort.h"

#include <set>
#include <list>

void Graph::addEdge(const int &v1, const int &v2) {
    this->adj[v1][v2] = this->adj[v2][v1] = true;
}


std::pair<const std::vector<int>, const int> Graph::greedyColoring() const {
    std::vector<int> result{};
    std::vector<bool> available{};

    result.assign(this->v, -1);
    result[0] = 0;

    for (int u = 1; u < v; u++) {
        available.assign(this->v, false);
        for (const int &neighbour: find_neighbours(u)) {
            result[neighbour] == -1 ?: available[result[neighbour]] = true;
        }

        result[u] = (int) (std::find(available.begin(), available.end(), false) - available.begin());
    }

    return std::pair<const std::vector<int>, const int>{result, *std::max_element(result.cbegin(), result.cend())};
}

std::vector<int> Graph::find_neighbours(const int &u) const {
    std::vector<int> neighbours{};
    for (int i = 0; i < v; i++) {
        if (this->adj[u][i]) {
            neighbours.emplace_back(i);
        }
    }
    return neighbours;
}

std::pair<const std::vector<int>, const int> Graph::randomGraphColoring() const {
    std::vector<int> result{};
    std::vector<bool> available{};

    result.assign(this->v, -1);
    const int startVertex = (int) (rand() % this->v);
    result[startVertex] = 0;

    for (int u = 0; u < v; u++) {
        if (u == startVertex) {
            continue;
        }
        available.assign(this->v, false);
        for (const int &neighbour: find_neighbours(u)) {
            result[neighbour] == -1 ?: available[result[neighbour]] = true;
        }

        result[u] = (int) (std::find(available.begin(), available.end(), false) - available.begin());
    }

    return std::pair<const std::vector<int>, const int>{result, *std::max_element(result.cbegin(), result.cend())};
}


std::pair<const std::vector<int>, const int> Graph::welshPowellColoring() const {
    std::vector<int> vertices{};
    std::vector<int> colours{};
    colours.assign(this->v, -1);
    for (int i = 0; i < this->v; ++i) {
        vertices.push_back(i);
    }
    sort(vertices);

    int k = 0;
    while (true) {
        std::set<int> kColoured{};
        for (const int &vertex: vertices) {
            if (colours[vertex] == -1 && std::find_if(kColoured.begin(), kColoured.end(), [&vertex, this](
                    const int &other) -> bool { return this->areAdjacent(vertex, other); }) == kColoured.end()) {
                kColoured.insert(vertex);
                colours[vertex] = k;
            }
        }
        if (kColoured.empty()) {
            break;
        }
        k++;
    }
    return std::pair<std::vector<int>, int>{colours, k};
}

std::pair<const std::vector<int>, const int> Graph::optimizedPowellColoring() const {
    std::vector<int> vertices{};
    std::vector<int> colours{};
    colours.assign(this->v, -1);
    for (int i = 0; i < this->v; ++i) {
        vertices.push_back(i);
    }
    sort(vertices);
    std::list<int> sortedVertices{vertices.cbegin(), vertices.cend()};
//    std::cout<<sortedVertices.size() << " -> ";
    int k = 0;
    while (true) {
        std::set<int> kColoured{};
        for (const int &vertex: sortedVertices) {
            if (colours[vertex] == -1 && std::find_if(kColoured.begin(), kColoured.end(), [&vertex, this](
                    const int &other) -> bool { return this->areAdjacent(vertex, other); }) == kColoured.end()) {
                kColoured.insert(vertex);
                colours[vertex] = k;
            }
        }
        if (kColoured.empty()) {
            break;
        }
        k++;
        auto it = std::remove_if(sortedVertices.begin(), sortedVertices.end(), [&kColoured](const int &vertex) -> bool {
            return kColoured.find(vertex) != kColoured.end();
        });
        sortedVertices.resize(std::distance(sortedVertices.begin(), it));
//        std::cout<<sortedVertices.size() <<" -> ";
    }
    return std::pair<std::vector<int>, int>{colours, k};
}

bool Graph::compareValenceDesc(const int &v1, const int &v2) const {
    return std::count(this->adj[v1].begin(), this->adj[v1].end(), true) >
           std::count(this->adj[v2].begin(), this->adj[v2].end(), true);
}

bool Graph::areAdjacent(const int &v1, const int &v2) const {
    return this->adj[v1][v2];
}

void Graph::sort(std::vector<int> &vertices) const {
    merge_sort(10, vertices,
               [this](const int &v1, const int &v2) -> bool { return this->compareValenceDesc(v1, v2); });
}

Graph generateGraph(int vertexNo, int edgeNo) {
    Graph gr{vertexNo};
    const int upperBound = vertexNo * (vertexNo - 1) / 2;
    edgeNo = edgeNo > upperBound ? upperBound : edgeNo;
    while (edgeNo) {
        const int vertex1 = rand() % vertexNo;
        const int vertex2 = rand() % vertexNo;
        if (gr.areAdjacent(vertex1, vertex2) || vertex1 == vertex2) {
            continue;
        }
        gr.addEdge(vertex1, vertex2);
        edgeNo--;
    }
    return gr;
}
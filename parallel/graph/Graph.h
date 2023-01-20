#ifndef PARALLEL_GRAPH_H
#define PARALLEL_GRAPH_H

#include <vector>

class Graph {
private:
    size_t v;
    std::vector<std::vector<bool>> adj;
public:
    explicit Graph(const size_t &v) : v{v}, adj{v} {
        for (std::vector<bool> &el: this->adj) {
            el.assign(v, false);
        }
    }

    ~Graph() = default;

    void addEdge(const int &v1, const int &v2);

    std::pair<const std::vector<int>, const int> greedyColoring() const;

    std::pair<const std::vector<int>, const int> randomGraphColoring() const;

    std::pair<const std::vector<int>, const int> welshPowellColoring() const;

    std::pair<const std::vector<int>, const int> optimizedPowellColoring() const;

    std::vector<int> find_neighbours(const int &u) const;

    void sort(std::vector<int> &vertices) const;

    bool areAdjacent(const int &v1, const int &v2) const;

    bool compareValenceDesc(const int &v1, const int &v2) const;
};

Graph generateGraph(int vertexNo, int edgeNo);


#endif //PARALLEL_GRAPH_H

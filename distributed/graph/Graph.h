#ifndef DISTRIBUTED_GRAPH_H
#define DISTRIBUTED_GRAPH_H

#include <vector>


class Graph {
private:
    size_t v{};
    std::vector<std::vector<bool>> adj{};
public:
    Graph() = default;

    explicit Graph(const size_t &v);

    Graph(size_t v,
          const std::vector<std::vector<bool>> &adj);

    Graph(size_t v,
          const std::vector<std::vector<int>> &adj);

    ~Graph() = default;

    void addEdge(const int &v1,
                 const int &v2);

    std::vector<std::vector<int>> getAdj() const;

    std::pair<const std::vector<int>, const int> greedyColoring() const;

    std::pair<const std::vector<int>, const int> randomGraphColoring() const;

    std::pair<const std::vector<int>, const int> welshPowellColoring(
            const std::function<void(std::vector<int> &,
                                     const std::function<bool(const int &, const int &)> &)> &sort =
            [](std::vector<int> &vertices,
               const std::function<bool(const int &, const int &)> &comparator) -> void {
                std::sort(vertices.begin(), vertices.end(), comparator);
            }) const;

    std::pair<const std::vector<int>, const int> optimizedPowellColoring(
            const std::function<void(std::vector<int> &,
                                     const std::function<bool(const int &, const int &)> &)> &sort =
            [](std::vector<int> &vertices,
               const std::function<bool(const int &, const int &)> &comparator) -> void {
                std::sort(vertices.begin(), vertices.end(), comparator);
            }) const;

    std::vector<int> find_neighbours(const int &u) const;

    bool areAdjacent(const int &v1, const int &v2) const;

    bool compareValenceDesc(const int &v1, const int &v2) const;
};

Graph generateGraph(int vertexNo, int edgeNo);

#endif //DISTRIBUTED_GRAPH_H

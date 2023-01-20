#include <iostream>
#include "graph/Graph.h"
#include "merge-sort/merge_sort.h"


int main() {
//    Graph g2(5);
//    g2.addEdge(0, 1);
//    g2.addEdge(0, 2);
//    g2.addEdge(1, 2);
//    g2.addEdge(1, 4);
//    g2.addEdge(2, 4);
//    g2.addEdge(4, 3);
//    std::cout << "Coloring of graph 1 \n";
//    auto result = g.greedyColoring();
//    for (int u = 0; u < result.size(); u++)
//        std::cout << "Vertex " << u << " ---> Color "
//                  << result[u] << std::endl;
//    std::cout << "\n";
    int threadsNo{}, vertexNo, edgeNo{};
    std::cout << "threads no = ";
    std::cin >> threadsNo;
    std::cout << "vertex no = ";
    std::cin >> vertexNo;
    std::cout << "edge no (<= " + std::to_string(vertexNo * (vertexNo - 1) / 2) + ") = ";
    std::cin >> edgeNo;

    const std::function<void(std::vector<int> &, const std::function<bool(const int &, const int &)> &)> sort = [=](
            std::vector<int> &vertices, const std::function<bool(const int &, const int &)> &comparator) -> void {
        merge_sort(threadsNo, vertices, comparator);
    };

    auto beginTime = std::chrono::high_resolution_clock::now();
    auto endTIme = std::chrono::high_resolution_clock::now();
    Graph g = generateGraph(vertexNo, edgeNo);
    beginTime = std::chrono::high_resolution_clock::now();
    std::cout << g.welshPowellColoring().second << "\n";
    endTIme = std::chrono::high_resolution_clock::now();
    std::cout << "run in: " +
                 std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(endTIme - beginTime).count()) +
                 "\n";
    beginTime = std::chrono::high_resolution_clock::now();
    std::cout << g.optimizedPowellColoring().second << "\n";
    endTIme = std::chrono::high_resolution_clock::now();
    std::cout << "run in: " +
                 std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(endTIme - beginTime).count()) +
                 "\n";
    return 0;
}

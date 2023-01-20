#include <iostream>
#include "graph/Graph.h"


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

    auto beginTime = std::chrono::high_resolution_clock::now();
    auto endTIme = std::chrono::high_resolution_clock::now();
    Graph g = generateGraph(1000, 7000 + 5000 + 5000);
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

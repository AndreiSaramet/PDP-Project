#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <future>

void concat(const std::vector<int> &v1,
            const std::vector<int> &v2,
            std::vector<int> &res) {
    std::copy(v1.cbegin(), v1.cend(), std::back_inserter(res));
    std::copy(v2.cbegin(), v2.cend(), std::back_inserter(res));
}

void halve(const std::vector<int>::const_iterator &source_begin,
           const std::vector<int>::const_iterator &source_end,
           std::vector<int> &firstHalf,
           std::vector<int> &secondHalf) {
    const long middle = (source_end - source_begin) / 2;
    std::copy(source_begin, source_begin + middle, std::back_inserter(firstHalf));
    std::copy(source_begin + middle, source_end, std::back_inserter(secondHalf));
}

void halve(const std::vector<int> &source,
           std::vector<int> &firstHalf,
           std::vector<int> &secondHalf) {
    const long middle = (long) source.size() / 2;
    std::copy(source.cbegin(), source.cbegin() + middle, std::back_inserter(firstHalf));
    std::copy(source.cbegin() + middle, source.cend(), std::back_inserter(secondHalf));
}

void merge(const std::vector<int>::const_iterator &v1_begin,
           const std::vector<int>::const_iterator &v1_end,
           const std::vector<int>::const_iterator &v2_begin,
           const std::vector<int>::const_iterator &v2_end,
           std::vector<int> &result,
           const std::function<bool(const int &, const int &)> &comparator = [](const int &a,
                                                                                const int &b) -> bool {
               return a < b;
           }) {
    std::merge(v1_begin, v1_end, v2_begin, v2_end, std::back_inserter(result), comparator);
}

void merge_rec(const std::vector<int>::const_iterator &v1_begin,
               const std::vector<int>::const_iterator &v1_end,
               const std::vector<int>::const_iterator &v2_begin,
               const std::vector<int>::const_iterator &v2_end,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator = [](const int &a,
                                                                                    const int &b) -> bool {
                   return a < b;
               }) {
    if (v1_end - v1_begin <= 1 || v2_end - v2_begin <= 1) {
        std::merge(v1_begin, v1_end, v2_begin, v2_end, std::back_inserter(result), comparator);
        return;
    }
    std::vector<int> firstPart{}, secondPart{};
    const auto v1_middle = v1_begin + (v1_end - v1_begin) / 2;
    const auto v2_middle = std::lower_bound(v2_begin, v2_end, *v1_middle);
    merge_rec(v1_begin, v1_middle, v2_begin, v2_middle, firstPart, comparator);
    merge_rec(v1_middle, v1_end, v2_middle, v2_end, secondPart, comparator);
    concat(firstPart, secondPart, result);

}

void merge_rec(const int &threadsNo,
               const std::vector<int>::const_iterator &v1_begin,
               const std::vector<int>::const_iterator &v1_end,
               const std::vector<int>::const_iterator &v2_begin,
               const std::vector<int>::const_iterator &v2_end,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator = [](const int &a,
                                                                                    const int &b) -> bool {
                   return a < b;
               }) {
    if (threadsNo <= 1) {
        merge_rec(v1_begin, v1_end, v2_begin, v2_end, result, comparator);
        return;
    }
    if (v1_end - v2_begin <= 1 || v2_end - v2_begin <= 1) {
        std::merge(v1_begin, v1_end, v2_begin, v2_end, std::back_inserter(result), comparator);
        return;
    }
    std::vector<int> firstPart{}, secondPart{};
    const auto v1_middle = v1_begin + (v1_end - v1_begin) / 2;
    const auto v2_middle = std::lower_bound(v2_begin, v2_end, *v1_middle);
    std::future<void> future = std::async(std::launch::async, [=, &firstPart]() -> void {
        merge_rec(threadsNo / 2, v1_begin, v1_middle, v2_begin, v2_middle, firstPart, comparator);
    });
    merge_rec(threadsNo - threadsNo / 2, v1_middle, v1_end, v2_middle, v2_end, secondPart, comparator);
    future.wait();
    concat(firstPart, secondPart, result);

}

void merge(const std::vector<int> &v1,
           const std::vector<int> &v2,
           std::vector<int> &result,
           const std::function<bool(const int &, const int &)> &comparator = [](const int &a, const int &b) -> bool {
               return a < b;
           }) {
    merge(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), result, comparator);
}

void merge_rec(const std::vector<int> &v1,
               const std::vector<int> &v2,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator = [](const int &a,
                                                                                    const int &b) -> bool {
                   return a < b;
               }) {
    merge_rec(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), result, comparator);
}

void merge_rec(const int &threadsNo,
               const std::vector<int> &v1,
               const std::vector<int> &v2,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator = [](const int &a,
                                                                                    const int &b) -> bool {
                   return a < b;
               }) {
    merge_rec(threadsNo, v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), result, comparator);
}


void merge_sort(std::vector<int> &v,
                const std::function<bool(const int &, const int &)> &comparator = [](const int &a,
                                                                                     const int &b) -> bool {
                    return a < b;
                }) {
    if (v.size() <= 1) {
        return;
    }
    std::vector<int> firstHalf{}, secondHalf{};
    halve(v, firstHalf, secondHalf);
    merge_sort(firstHalf, comparator);
    merge_sort(secondHalf, comparator);
    v.resize(0);
    merge_rec(firstHalf, secondHalf, v, comparator);
}

void merge_sort(const int &threadsNo,
                std::vector<int> &v,
                const std::function<bool(const int &, const int &)> &comparator = [](const int &a,
                                                                                     const int &b) -> bool {
                    return a < b;
                }) {
    if (threadsNo <= 1) {
        merge_sort(v, comparator);
        return;
    }
    if (v.size() <= 1) {
        return;
    }
    std::vector<int> firstHalf{}, secondHalf{};
    halve(v, firstHalf, secondHalf);
    std::future<void> future = std::async(std::launch::async, [=, &firstHalf]() -> void {
        merge_sort(threadsNo / 2, firstHalf, comparator);
    });
    merge_sort(threadsNo - threadsNo / 2, secondHalf, comparator);
    v.resize(0);
    future.wait();
    merge_rec(threadsNo, firstHalf, secondHalf, v, comparator);
}

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
    merge_sort(2, vertices,
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
    Graph g = generateGraph(10000, 70000000);
    auto endTIme = std::chrono::high_resolution_clock::now();

    beginTime = std::chrono::high_resolution_clock::now();
    std::cout << g.greedyColoring().second << "\n";
    endTIme = std::chrono::high_resolution_clock::now();
    std::cout << "run in: " +
                 std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(endTIme - beginTime).count()) +
                 "\n";
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

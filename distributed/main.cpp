#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <mpi.h>
#include "graph/Graph.h"
#include "merge-sort/merge_sort.h"

void distribute(
        const int &me,
        const int &processNo,
        std::vector<int> &v,
        const int &threadsNo = 10,
        const std::function<bool(const int &, const int &)> &comparator =
        [](const int &a, const int &b) -> bool {
            return a < b;
        }
) {
    int v_size = (int) v.size();
    MPI_Status status{};
    if (v.size() <= 1) {
        if (me + 1 < processNo) {
            MPI_Ssend(&v_size, 1, MPI_INT, me + 1, 1, MPI_COMM_WORLD);
            MPI_Ssend(v.data(), v_size, MPI_INT, me + 1, 2, MPI_COMM_WORLD);
            MPI_Recv(&v_size, 1, MPI_INT, me + 1, 3, MPI_COMM_WORLD, &status);
            v.resize(v_size);
            MPI_Recv(v.data(), v_size, MPI_INT, me + 1, 4, MPI_COMM_WORLD, &status);
        }
    } else {
        std::vector<int> firstHalf{}, secondHalf{};
        int firstHalf_size{};
        halve(v, firstHalf, secondHalf);
        if (me + 1 < processNo) {
            firstHalf_size = (int) firstHalf.size();
            MPI_Ssend(&firstHalf_size, 1, MPI_INT, me + 1, 1, MPI_COMM_WORLD);
            MPI_Ssend(firstHalf.data(), firstHalf_size, MPI_INT, me + 1, 2, MPI_COMM_WORLD);
        } else {
            merge_sort(firstHalf, comparator);
        }
        merge_sort(secondHalf, comparator);
        v.resize(0);
        if (me + 1 < processNo) {
            MPI_Recv(&firstHalf_size, 1, MPI_INT, me + 1, 3, MPI_COMM_WORLD, &status);
            firstHalf.resize(firstHalf_size);
            MPI_Recv(firstHalf.data(), firstHalf_size, MPI_INT, me + 1, 4, MPI_COMM_WORLD, &status);
        }
        merge_rec(threadsNo, firstHalf, secondHalf, v, comparator);
    }
}

void worker(
        const int &me,
        const int &processNo,
        const int &threadsNo = 10,
        const std::function<bool(const int &, const int &)> &comparator =
        [](const int &a, const int &b) -> bool {
            return a < b;
        }
) {
    int v_size{};
    std::vector<int> v{};
    MPI_Status status{};
    MPI_Recv(&v_size, 1, MPI_INT, me - 1, 1, MPI_COMM_WORLD, &status);
    v.resize(v_size);
    MPI_Recv(v.data(), v_size, MPI_INT, me - 1, 2, MPI_COMM_WORLD, &status);
    if (v.size() <= 1) {
        if (me + 1 < processNo) {
            MPI_Ssend(&v_size, 1, MPI_INT, me + 1, 1, MPI_COMM_WORLD);
            MPI_Ssend(v.data(), v_size, MPI_INT, me + 1, 2, MPI_COMM_WORLD);
            MPI_Recv(&v_size, 1, MPI_INT, me + 1, 3, MPI_COMM_WORLD, &status);
            v.resize(v_size);
            MPI_Recv(v.data(), v_size, MPI_INT, me + 1, 4, MPI_COMM_WORLD, &status);
        }
    } else {
        std::vector<int> firstHalf{}, secondHalf{};
        int firstHalf_size{};
        halve(v, firstHalf, secondHalf);
        if (me + 1 < processNo) {
            firstHalf_size = (int) firstHalf.size();
            MPI_Ssend(&firstHalf_size, 1, MPI_INT, me + 1, 1, MPI_COMM_WORLD);
            MPI_Ssend(firstHalf.data(), firstHalf_size, MPI_INT, me + 1, 2, MPI_COMM_WORLD);
        } else {
            merge_sort(firstHalf, comparator);
        }
        merge_sort(secondHalf, comparator);
        v.resize(0);
        if (me + 1 < processNo) {
            MPI_Recv(&firstHalf_size, 1, MPI_INT, me + 1, 3, MPI_COMM_WORLD, &status);
            firstHalf.resize(firstHalf_size);
            MPI_Recv(firstHalf.data(), firstHalf_size, MPI_INT, me + 1, 4, MPI_COMM_WORLD, &status);
        }
        merge_rec(threadsNo, firstHalf, secondHalf, v, comparator);
    }
    v_size = (int) v.size();
    MPI_Ssend(&v_size, 1, MPI_INT, me - 1, 3, MPI_COMM_WORLD);
    MPI_Ssend(v.data(), v_size, MPI_INT, me - 1, 4, MPI_COMM_WORLD);
}

int main() {
    MPI_Init(nullptr, nullptr);
    int me, processNo;
    MPI_Comm_size(MPI_COMM_WORLD, &processNo);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    Graph g{};
    size_t vertexNo{};
    std::vector<std::vector<int>> adj{};
    if (me == 0) {
        vertexNo = 1000;
        g = generateGraph((int) vertexNo, 70000);
//        g = Graph{vertexNo};
//        g.addEdge(0, 1);
//        g.addEdge(0, 2);
//        g.addEdge(1, 2);
//        g.addEdge(1, 4);
//        g.addEdge(2, 4);
//        g.addEdge(4, 3);

        adj = g.getAdj();
    }
    MPI_Bcast(&vertexNo, 1, MPI_INT, 0, MPI_COMM_WORLD);
    adj.resize(vertexNo);
    for (auto &el: adj) {
        el.resize(vertexNo);
        MPI_Bcast(el.data(), (int) vertexNo, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);
    }
    g = Graph{vertexNo, adj};

    if (me == 0) {
        const std::function<void(std::vector<int> &, const std::function<bool(const int &, const int &)> &)> sort = [=](
                std::vector<int> &vertices, const std::function<bool(const int &, const int &)> &comparator) -> void {
            distribute(me, processNo, vertices, 10, comparator);
        };
        auto beginTime = std::chrono::high_resolution_clock::now();
        std::cout << g.welshPowellColoring(sort).second << "\n";
//        std::cout<<g.optimizedPowellColoring(merge-sort).second <<"\n";
        auto endTIme = std::chrono::high_resolution_clock::now();
        std::cout << "run in: " +
                     std::to_string(
                             std::chrono::duration_cast<std::chrono::microseconds>(endTIme - beginTime).count()) +
                     "\n";
    } else {
        worker(me, processNo, 10, [g](const int &v1, const int &v2) -> bool { return g.compareValenceDesc(v1, v2); });
    }
    MPI_Finalize();
    return 0;
}

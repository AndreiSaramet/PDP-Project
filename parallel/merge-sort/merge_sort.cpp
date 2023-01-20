#include "merge_sort.h"

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
           const std::function<bool(const int &, const int &)> &comparator) {
    std::merge(v1_begin, v1_end, v2_begin, v2_end, std::back_inserter(result), comparator);
}

void merge_rec(const std::vector<int>::const_iterator &v1_begin,
               const std::vector<int>::const_iterator &v1_end,
               const std::vector<int>::const_iterator &v2_begin,
               const std::vector<int>::const_iterator &v2_end,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator) {
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
               const std::function<bool(const int &, const int &)> &comparator) {
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
           const std::function<bool(const int &, const int &)> &comparator) {
    merge(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), result, comparator);
}

void merge_rec(const std::vector<int> &v1,
               const std::vector<int> &v2,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator) {
    merge_rec(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), result, comparator);
}

void merge_rec(const int &threadsNo,
               const std::vector<int> &v1,
               const std::vector<int> &v2,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator) {
    merge_rec(threadsNo, v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), result, comparator);
}


void merge_sort(std::vector<int> &v,
                const std::function<bool(const int &, const int &)> &comparator) {
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
                const std::function<bool(const int &, const int &)> &comparator) {
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
#ifndef PARALLEL_MERGE_SORT_H
#define PARALLEL_MERGE_SORT_H

#include <vector>

void concat(const std::vector<int> &v1,
            const std::vector<int> &v2,
            std::vector<int> &res);

void halve(const std::vector<int>::const_iterator &source_begin,
           const std::vector<int>::const_iterator &source_end,
           std::vector<int> &firstHalf,
           std::vector<int> &secondHalf);

void halve(const std::vector<int> &source,
           std::vector<int> &firstHalf,
           std::vector<int> &secondHalf);

void merge(const std::vector<int>::const_iterator &v1_begin,
           const std::vector<int>::const_iterator &v1_end,
           const std::vector<int>::const_iterator &v2_begin,
           const std::vector<int>::const_iterator &v2_end,
           std::vector<int> &result,
           const std::function<bool(const int &, const int &)> &comparator =
           [](const int &a, const int &b) -> bool {
               return a < b;
           });

void merge_rec(const std::vector<int>::const_iterator &v1_begin,
               const std::vector<int>::const_iterator &v1_end,
               const std::vector<int>::const_iterator &v2_begin,
               const std::vector<int>::const_iterator &v2_end,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator =
               [](const int &a, const int &b) -> bool {
                   return a < b;
               });

void merge_rec(const int &threadsNo,
               const std::vector<int>::const_iterator &v1_begin,
               const std::vector<int>::const_iterator &v1_end,
               const std::vector<int>::const_iterator &v2_begin,
               const std::vector<int>::const_iterator &v2_end,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator =
               [](const int &a, const int &b) -> bool {
                   return a < b;
               });

void merge(const std::vector<int> &v1,
           const std::vector<int> &v2,
           std::vector<int> &result,
           const std::function<bool(const int &, const int &)> &comparator =
           [](const int &a, const int &b) -> bool {
               return a < b;
           });

void merge_rec(const std::vector<int> &v1,
               const std::vector<int> &v2,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator =
               [](const int &a, const int &b) -> bool {
                   return a < b;
               });

void merge_rec(const int &threadsNo,
               const std::vector<int> &v1,
               const std::vector<int> &v2,
               std::vector<int> &result,
               const std::function<bool(const int &, const int &)> &comparator =
               [](const int &a, const int &b) -> bool {
                   return a < b;
               });

void merge_sort(std::vector<int> &v,
                const std::function<bool(const int &, const int &)> &comparator =
                [](const int &a, const int &b) -> bool {
                    return a < b;
                });

void merge_sort(const int &threadsNo,
                std::vector<int> &v,
                const std::function<bool(const int &, const int &)> &comparator =
                [](const int &a, const int &b) -> bool {
                    return a < b;
                });

#endif //PARALLEL_MERGE_SORT_H

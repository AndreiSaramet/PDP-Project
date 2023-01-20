cmake_minimum_required(VERSION 3.24)
project(parallel)

set(CMAKE_CXX_STANDARD 17)

add_executable(parallel main.cpp graph/Graph.cpp graph/Graph.h merge-sort/merge_sort.cpp merge-sort/merge_sort.h)

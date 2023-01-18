cmake_minimum_required(VERSION 3.24)
project(distributed)

set(CMAKE_CXX_STANDARD 17)
find_package(MPI REQUIRED)

add_executable(distributed main.cpp)
target_link_libraries(distributed PUBLIC MPI::MPI_CXX)

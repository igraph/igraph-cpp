
cmake_minimum_required(VERSION 3.18)

project(igraph-cpp)

find_package(igraph REQUIRED)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

add_library(igraph-cpp INTERFACE)

add_executable(test test.cpp)
target_link_libraries(test PUBLIC igraph::igraph)

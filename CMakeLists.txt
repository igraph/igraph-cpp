
cmake_minimum_required(VERSION 3.18)

project(igraph-cpp)

find_package(igraph REQUIRED)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

add_library(igraph-cpp INTERFACE)

add_executable(ex_basic ex_basic.cpp)
target_link_libraries(ex_basic PUBLIC igraph::igraph)

add_executable(ex_matrix ex_matrix.cpp)
target_link_libraries(ex_matrix PUBLIC igraph::igraph)

add_executable(ex_graph ex_graph.cpp)
target_link_libraries(ex_graph PUBLIC igraph::igraph)

add_executable(ex_list ex_list.cpp)
target_link_libraries(ex_list PUBLIC igraph::igraph)

add_executable(ex_decompose ex_decompose.cpp)
target_link_libraries(ex_decompose PUBLIC igraph::igraph)

add_executable(ex_error ex_error.cpp)
target_link_libraries(ex_error PUBLIC igraph::igraph)

add_executable(ex_views ex_views.cpp)
target_link_libraries(ex_views PUBLIC igraph::igraph)

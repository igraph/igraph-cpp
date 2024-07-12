
cmake_minimum_required(VERSION 3.18)

project(igraph-cpp)

find_package(igraph REQUIRED)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

add_library(igraph-cpp INTERFACE)
target_include_directories(igraph-cpp INTERFACE include)
target_link_libraries(igraph-cpp INTERFACE igraph::igraph)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/igraph-cpp)

add_executable(ex_basic examples/ex_basic.cpp)
target_link_libraries(ex_basic PUBLIC igraph-cpp)

add_executable(ex_matrix examples/ex_matrix.cpp)
target_link_libraries(ex_matrix PUBLIC igraph-cpp)

add_executable(ex_graph examples/ex_graph.cpp)
target_link_libraries(ex_graph PUBLIC igraph-cpp)

add_executable(ex_list examples/ex_list.cpp)
target_link_libraries(ex_list PUBLIC igraph-cpp)

add_executable(ex_decompose examples/ex_decompose.cpp)
target_link_libraries(ex_decompose PUBLIC igraph-cpp)

add_executable(ex_error examples/ex_error.cpp)
target_link_libraries(ex_error PUBLIC igraph-cpp)

add_executable(ex_views examples/ex_views.cpp)
target_link_libraries(ex_views PUBLIC igraph-cpp)

add_executable(ex_igraph_tutorial examples/ex_igraph_tutorial.cpp)
target_link_libraries(ex_igraph_tutorial PUBLIC igraph-cpp)

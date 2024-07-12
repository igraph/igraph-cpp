
cmake_minimum_required(VERSION 3.18)

project(igraph-cpp)

find_package(igraph REQUIRED)

include(GNUInstallDirs)
include(CTest)

set(CMAKE_CXX_STANDARD 11)

add_library(igraph-cpp INTERFACE)
target_include_directories(igraph-cpp INTERFACE include)
target_link_libraries(igraph-cpp INTERFACE igraph::igraph)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/igraph-cpp)

add_subdirectory(examples)

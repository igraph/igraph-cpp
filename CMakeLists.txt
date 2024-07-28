
cmake_minimum_required(VERSION 3.18)

project(
  igraph-cpp
  VERSION 0.1.0
  DESCRIPTION "C++ wrappers for igraph data structures"
  LANGUAGES CXX
)

find_package(igraph 0.10.0 REQUIRED)

include(GNUInstallDirs)
include(CTest)

add_library(igraph-cpp INTERFACE)
target_include_directories(igraph-cpp INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/igraph-cpp>
)
target_compile_features(igraph-cpp INTERFACE cxx_std_14)
target_link_libraries(igraph-cpp INTERFACE igraph::igraph)

# Provide an igraph-cpp-config.cmake file in the installation directory so
# users can find the installed igraph library with FIND_PACKAGE(igraph-cpp)
# from their CMakeLists.txt files
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/igraph-cpp-config.cmake.in
  ${PROJECT_BINARY_DIR}/igraph-cpp-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/igraph-cpp
)
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/igraph-cpp-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion
)

install(TARGETS igraph-cpp
        EXPORT igraph-cpp-targets)
install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/igraph-cpp)
install(FILES ${PROJECT_BINARY_DIR}/igraph-cpp-config.cmake
              ${PROJECT_BINARY_DIR}/igraph-cpp-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/igraph-cpp)
install(EXPORT igraph-cpp-targets
        FILE igraph-cpp-targets.cmake
        NAMESPACE igraph::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/igraph-cpp)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(examples)
endif()

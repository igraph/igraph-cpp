# igraph-cpp

This is an experimental header-only library that provides RAII wrapper for igraph's data structures. This project is in constant flux and not ready for general use!!

## Goals

 - Make it more convenient to use igraph from C++.
 - Provide minimal RAII wrappers for common igraph data structures, to simplify memory management.
 - Provide some level of STL compatibility for igraph container types.
 - Convert igraph error codes to C++ exceptions, in support of easy memory management.

## Non-goals

 - Provide a full C++ wrapper to igraph's C API.

## Usage

Example programs, with explanatory comments, are found in the `examples` directory. They can be built using CMake:

```
mkdir build && cd build
cmake ..
cmake --build .
```

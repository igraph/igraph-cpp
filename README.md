# igraph-cpp

This is an experimental RAII wrapper for igraph's data structures. This project is in constant flux and not ready for general use!!

## Goals

 - Make it more convenient to use igraph from C++.
 - Provide minimal RAII wrappers for common igraph data structures, to simplify memory management.
 - Convert igraph error codes to C++ exceptions, in support of easy memory management.

## Non-goals

 - Provide a full C++ wrapper to igraph's C API.


#include "igraph.hpp"

#include <iostream>

// Everything lives in the ig:: namespace.
// We do not use igraph:: to avoid conflict with the igraphpp project.
using namespace ig;

int main() {

    // igRealVec is a RAII wrapper to igraph_vector_t.
    // When it goes out of scope, it is properly destroyed.
    // igRealVec is a convenience name for igVec<igraph_real_t>.
    igRealVec v;

    // initial size is zero
    std::cout << v.size() << std::endl;

    // igVec<XXX> is convertible to the corresponding igraph_vector_XXX_t * pointer.
    igCheck(igraph_vector_resize(v, 10));
    std::cout << v.size() << std::endl;

    // igVec<> has STL-compatible convenience member functions for common operations.
    v.resize(5);
    std::cout << v.size() << std::endl;

    // The indexing operator is available.
    v[3] = 1; v[4] = 2;

    // The STL comatibility provides support for range-based for loops.
    for (auto el : v)
        std::cout << el << ' ';
    std::cout << std::endl;

    return 0;
}
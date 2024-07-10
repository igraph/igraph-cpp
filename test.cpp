
#include "igraph.hpp"

#include <iostream>
#include <numeric>

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

    // The STL compatibility provides support for range-based for loops.
    for (auto el : v)
        std::cout << el << ' ';
    std::cout << std::endl;

    // Fill the vector with an increasing range of elements and print it.
    std::iota(v.begin(), v.end(), 3);
    for (auto el : v)
        std::cout << el << ' ';
    std::cout << std::endl;

    // Erase the 2nd and 3rd elements.
    v.erase(v.begin() + 1, v.begin() + 3);
    for (auto el : v)
        std::cout << el << ' ';
    std::cout << std::endl;

    return 0;
}
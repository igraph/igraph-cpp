
#include <igraph.hpp>
#include "ex_vector_print.hpp"

#include <algorithm>
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

    // The initial size is zero.
    std::cout << v.size() << std::endl;

    // igVec<XXX> is implicitly convertible to the corresponding igraph_vector_XXX_t * pointer.
    igraph_vector_resize(v, 10);
    std::cout << v.size() << std::endl;

    // igVec<> has STL-compatible convenience member functions for common operations.
    // These generally call the corresponding igraph function.
    v.resize(5);
    std::cout << v.size() << std::endl;

    // Note that igRealVec::resize() has a behaviour identical to that of igraph_vector_resize().
    // It does not set the newly added elements to zero, but we can easily do this using
    // igraph's standard facilities.
    igraph_vector_null(v);

    // The indexing operator is available.
    v[3] = 1; v[4] = 2;

    // The STL compatibility provides support for range-based for loops.
    for (auto el : v)
        std::cout << el << ' ';
    std::cout << std::endl;

    // Use standard C++ features to fill the vector with an increasing sequence.
    std::iota(v.begin(), v.end(), 3);

    // See ex_vector_print.hpp for stream support for igVec.
    std::cout << v << std::endl;

    // Use igraph features to fill the vector with an increasing sequence.
    igraph_vector_range(v, -3, 3);
    std::cout << v << std::endl;

    // Erase the 2nd and 3rd elements.
    v.erase(v.begin() + 1, v.begin() + 3);
    std::cout << v << std::endl;

    // Create an integer list initializer.
    // igIntVec is a convenience alias to igVec<igraph_integer_t>.
    igIntVec iv = {4, 3, 0, 9, -3};
    std::cout << "Original: " << iv << std::endl;

    // Sort it using STL facilities and print it.
    std::stable_sort(iv.begin(), iv.end());
    std::cout << "Sorted: " << iv << std::endl;

    // Create a boolean vector using a list initializer and print it.
    igBoolVec bv = {true, false, true, false};
    std::cout << "Boolean vector: " << bv << std::endl;

    return 0;
}
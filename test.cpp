
#include "igraph.hpp"

#include <iostream>
#include <numeric>

// Everything lives in the ig:: namespace.
// We do not use igraph:: to avoid conflict with the igraphpp project.
using namespace ig;

// Helper function to print igVec objects. Since igVec is templated, it is easy
// to use a single definition for all of its variants.
template<typename T>
std::ostream & operator << (std::ostream &out, const igVec<T> &v) {
    for (auto it = v.begin(); it != v.end() - 1; ++it) {
        std::cout << *it << ' ';
    }
    if (v.size() > 0) {
        std::cout << v.back();
    }
    return out;
}

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

    // Use standard C++ features to fill the vector with an increasing sequence.
    std::iota(v.begin(), v.end(), 3);
    std::cout << v << std::endl;

    // Use igraph features to fill the vector with an increasing sequence.
    igraph_vector_range(v, -3, 3);
    std::cout << v << std::endl;

    // Erase the 2nd and 3rd elements.
    v.erase(v.begin() + 1, v.begin() + 3);
    std::cout << v << std::endl;

    // Create an integer and a boolean vector using a list initializer.
    igIntVec iv = {4, 3, 0};
    igBoolVec bv = {true, false, true, false};
    std::cout << iv << std::endl;
    std::cout << bv << std::endl;

    // Create a matrix.
    igRealMat mat = {{1, 2, 3},
                     {4, 5, 6}};

    // Print the sum of its elements
    std::cout << igraph_matrix_sum(mat) << std::endl;

    return 0;
}
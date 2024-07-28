
#include <igraph.hpp>
#include "ex_vector_print.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>

// Everything lives in the ig:: namespace.
using namespace ig;

int main() {

    // RealVec is a RAII wrapper to igraph_vector_t.
    // When it goes out of scope, it is properly destroyed.
    // RealVec is a convenience name for Vec<igraph_real_t>.
    RealVec v;

    // The initial size is zero.
    std::cout << v.size() << std::endl;

    // Vec<XXX> is implicitly convertible to the corresponding igraph_vector_XXX_t * pointer,
    // and can be used directly in igraph functions.
    igraph_vector_resize(v, 10);
    std::cout << v.size() << std::endl;

    // Vec<> has STL-compatible convenience member functions for common operations.
    // These generally call the corresponding igraph function.
    v.resize(5);
    std::cout << v.size() << std::endl;

    // Note that RealVec::resize() has a behaviour identical to that of igraph_vector_resize().
    // It does not set the newly added elements to zero, but we can easily do this using
    // igraph's standard facilities.
    igraph_vector_null(v);

    // The indexing operator is available, and can be used to either read or set values.
    v[3] = 1; v[4] = 2;

    // The STL compatibility provides support for range-based for loops.
    for (auto el : v)
        std::cout << el << ' ';
    std::cout << std::endl;

    // Use standard C++ features to fill the vector with an increasing sequence.
    std::iota(v.begin(), v.end(), 3);

    // See ex_vector_print.hpp for stream support for Vec.
    std::cout << v << std::endl;

    // Use igraph features to fill the vector with an increasing sequence.
    igraph_vector_range(v, -3, 3);
    std::cout << v << std::endl;

    // Erase the 2nd and 3rd elements using standard C++ features.
    v.erase(v.begin() + 1, v.begin() + 3);
    std::cout << v << std::endl;

    // A wrapper is available for each igraph vector specialization:
    //   RealVec    = Vec<igraph_real_t>         igraph_vector_t
    //   IntVec     = Vec<igraph_integer_t>      igraph_vector_int_t
    //   BoolVec    = Vec<igraph_bool_t>         igraph_vector_bool_t
    //   ComplexVec = Vec<std::complex<double>>  igraph_vector_complex_t
    // See ex_complex.cpp for more on ComplexVec.

    // Vectors can be conveniently created using an initializer list syntax.
    IntVec iv = {4, 3, 0, 9, -3};
    std::cout << "Original: " << iv << std::endl;

    // Sort it using STL facilities and print it.
    std::stable_sort(iv.begin(), iv.end());
    std::cout << "Sorted: " << iv << std::endl;

    // Create a boolean vector using an initializer list and print it.
    BoolVec bv = {true, false, true, false};
    std::cout << "Boolean vector: " << bv << std::endl;

    return 0;
}
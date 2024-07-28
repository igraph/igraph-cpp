
#include <igraph.hpp>

#include <iostream>

using namespace ig;

// ig::Mat is a C++ wrapper for igraph_matrix_t and its various specializations.
//   igRealMat    = igMat<igraph_real_t>                igraph_matrix_t
//   igIntMat     = igMat<igraph_integer_t>             igraph_matrix_int_t
//   igBoolMat    = igMat<igraph_bool_t>                igraph_matrix_bool_t
//   igComplexMat = igMat<std::complex<igraph_real_t>>  igraph_matrix_complex_t

int main() {

    // Create a matrix using a list initializer.
    RealMat mat = {{1, 2, 3},
                   {4, 5, 6}};

    // Print the matrix using igraph features.
    std::cout << "Matrix:" << std::endl;
    igraph_matrix_print(mat);

    // Print the sum of its elements.
    std::cout << "Matrix sum: " << igraph_matrix_sum(mat) << std::endl;

    // Transpose and print again.
    igraph_matrix_transpose(mat);
    std::cout << "Transposed matrix:" << std::endl;
    igraph_matrix_print(mat);

    std::cout << "Matrix size after transpose: " << mat.nrow() << 'x' << mat.ncol() << std::endl;

    // Matrices can be indexed according to rows and columns with parentheses ( )
    // or treated as a flat vector and indexed using  [ ].
    std::cout << "Element (2,1) is " << mat(2, 1) << std::endl;

    // ig::Mat can be accessed as a flat-storage STL-compatible container.
    // Print all elements in flat storage order. igraph uses column-major storage.
    std::cout << "Matrix elements in flat storage order:\n";
    for (auto el : mat)
        std::cout << el << ' ';
    std::cout << std::endl;

    return 0;
}
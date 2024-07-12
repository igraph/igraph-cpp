
#include "igraph.hpp"

#include <iostream>

using namespace ig;

int main() {

    // Create a matrix using a list initializer.
    igRealMat mat = {{1, 2, 3},
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

    return 0;
}
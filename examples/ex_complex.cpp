
#include <igraph.hpp>
#include "ex_vector_print.hpp"

using namespace ig;

int main() {
    ComplexVec v;

    using namespace std::complex_literals;

    // The member functions of ComplexVec all use std::complex<double>
    // instead of igraph_complex_t.

    v.push_back(1.0 + 2.0i);
    v.push_back(3.0 + 2.0i);
    v.push_back(-1.0i);

    v[1] *= 0.5i;

    std::cout << v << std::endl;

    // ig::complex_cast interprets an std::complex<double> (or a pointer to it)
    // as an igraph_complex_t ...
    igraph_vector_complex_scale(v, complex_cast(-2.0));

    std::cout << v << std::endl;

    // ... and can also work in reverse, interpreting an igraph_complex_t as
    // an std::complex<double>
    std::cout << "Popping the last element, which is: "
              << complex_cast(igraph_vector_complex_pop_back(v))
              << std::endl;

    // Compute the spectrum of a non-symmetric matrix

    RealMat mat = {{1, 1, 2},
                   {0, 0, 0},
                   {-1, 1, -1}};

    igraph_eigen_which_t which;
    which.pos = IGRAPH_EIGEN_LM;
    which.howmany = mat.nrow();

    ComplexVec values;
    ComplexMat vectors;
    igraph_eigen_matrix(mat, nullptr, nullptr, mat.nrow(), nullptr,
                        IGRAPH_EIGEN_LAPACK, &which, nullptr, nullptr,
                        values, vectors);

    std::cout << "Eigenvalues: ";
    igraph_vector_complex_zapsmall(values, 0); // replace tiny numbers by 0.0
    igraph_vector_complex_print(values);

    std::cout << "Eigenvectors:\n";
    vectors.transpose(); // transpose so that eigenvectors become matrix rows
    igraph_matrix_complex_print(vectors);

    return 0;
}
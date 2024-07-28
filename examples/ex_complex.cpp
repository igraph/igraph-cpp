
#include <igraph.hpp>
#include "ex_vector_print.hpp"

using namespace ig;

int main() {

    // ig::ComplexVec is an alias for Vec<std::complex<igraph_real_t>>
    // It wraps igraph_vector_complex_t.
    // It is slightly different from other specializations of Vec in that
    // it operates with std::complex<igraph_real_t> instead of igraph_complex_t.

    ComplexVec v;

    using namespace std::complex_literals;

    v.push_back(1.0 + 2.0i);
    v.push_back(3.0 + 2.0i);
    v.push_back(-1.0i);

    v[1] *= 0.5i;

    std::cout << v << std::endl;

    // std::complex<igraph_real_t> and igraph_complex_t are bitwise equivalent.
    // ig::complex_cast serves to cast between the two (or pointers to them)
    // in either direction. Here we use it to convert a C++ complex (or even real)
    // to an igraph_complex_t, which is what igraph function expect.
    igraph_vector_complex_scale(v, complex_cast(-2.0));

    std::cout << v << std::endl;

    // The same complex_cast works in the opposite direction as well, to convert
    // an igraph_complex_t into an easier-to-manipulate C++ complex.
    std::cout << "Popping the last element, which is: "
              << complex_cast(igraph_vector_complex_pop_back(v))
              << std::endl;

    // Example application of complex vectors and matrices:
    // Compute the spectrum of a non-symmetric matrix.

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
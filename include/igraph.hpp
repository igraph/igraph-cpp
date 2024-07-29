
#ifndef IGCPP_IGRAPH_HPP
#define IGCPP_IGRAPH_HPP

#include <igraph.h>

#if !(IGRAPH_VERSION_MAJOR == 0 && IGRAPH_VERSION_MINOR == 10)
#error "This version of igraph-cpp requires igraph 0.10."
#endif

#include <cassert>
#include <complex>
#include <iterator>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace ig {

// Error handling and exceptions

struct Exception : std::runtime_error {
    igraph_error_t error;

    explicit Exception(igraph_error_t error_) :
            std::runtime_error(igraph_strerror(error_)),
            error(error_) { }
};

inline void check(igraph_error_t error) {
    if (error != IGRAPH_SUCCESS)
        throw Exception{error};
}

// Casts between igraph_complex_t and std::complex<double>.
// complex_cast() can convert in both directions between these two types.

std::complex<double> &complex_cast(igraph_complex_t &c) {
    return reinterpret_cast<std::complex<double> &>(c);
}

const std::complex<double> &complex_cast(const igraph_complex_t &c) {
    return reinterpret_cast<const std::complex<double> &>(c);
}

std::complex<double> *complex_cast(igraph_complex_t *c) {
    return reinterpret_cast<std::complex<double> *>(c);
}

const std::complex<double> *complex_cast(const igraph_complex_t *c) {
    return reinterpret_cast<const std::complex<double> *>(c);
}

igraph_complex_t &complex_cast(std::complex<double> &c) {
    return reinterpret_cast<igraph_complex_t &>(c);
}

const igraph_complex_t &complex_cast(const std::complex<double> &c) {
    return reinterpret_cast<const igraph_complex_t &>(c);
}

igraph_complex_t *complex_cast(std::complex<double> *c) {
    return reinterpret_cast<igraph_complex_t *>(c);
}

const igraph_complex_t *complex_cast(const std::complex<double> *c) {
    return reinterpret_cast<const igraph_complex_t *>(c);
}

// Support structures

template<typename T>
struct CaptureType {
    T &obj;
    explicit CaptureType(T &obj) : obj(obj) { }
};

template<typename T>
inline CaptureType<typename std::remove_reference<T>::type>
Capture(T &&obj) { return CaptureType<typename std::remove_reference<T>::type>(obj); }

template<typename T>
struct AliasType {
    T &obj;
    explicit AliasType(T &obj) : obj(obj) { }
};

template<typename T>
inline AliasType<T> Alias(T &obj) { return AliasType<T>(obj); }

// Main data structures

template<typename T> class Vec;
template<typename T> class Mat;
template<typename T> class VecList;
template<typename T> class MatList;
class StrVec;
class Graph;
class GraphList;

#define BASE_IGRAPH_REAL
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#include "vec_list_pmt.hpp"
#include "mat_list_pmt.hpp"
#undef BASE_IGRAPH_REAL
using RealVec = Vec<igraph_real_t>;
using RealMat = Mat<igraph_real_t>;
using RealVecList = VecList<igraph_real_t>;
using RealMatList = MatList<igraph_real_t>;

#define BASE_INT
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#include "vec_list_pmt.hpp"
#undef BASE_INT
using IntVec = Vec<igraph_integer_t>;
using IntMat = Mat<igraph_integer_t>;
using IntVecList = VecList<igraph_integer_t>;

#define BASE_BOOL
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#undef BASE_BOOL
using BoolVec = Vec<igraph_bool_t>;
using BoolMat = Mat<igraph_bool_t>;

#define BASE_COMPLEX
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#undef BASE_COMPLEX
using ComplexVec = Vec<std::complex<igraph_real_t>>;
using ComplexMat = Mat<std::complex<igraph_real_t>>;

#include "strvec.hpp"

#include "bitset.hpp"

#define BASE_BITSET
#include "bitset_list_pmt.hpp"
#undef BASE_BITSET

#include "graph.hpp"

#define BASE_GRAPH
#include "graph_list_pmt.hpp"
#undef BASE_GRAPH

#include "rng_scope.hpp"

} // namespace ig

#endif // IGCPP_IGRAPH_HPP

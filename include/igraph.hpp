
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

// Error handling

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

// StrVec
#include "strvec.hpp"

#include "rng_scope.hpp"

class GraphList;

class Graph {
    using igraph_type = igraph_t;

    igraph_t graph;
    igraph_t *ptr = &graph;

    bool is_alias() const { return ptr != &graph; }

    friend class GraphList;

public:
    explicit Graph(CaptureType<igraph_t> g) : graph(g.obj) { }
    explicit Graph(AliasType<igraph_t> g) : ptr(&g.obj) { }

    explicit Graph(const igraph_t *g) {
        check(igraph_copy(ptr, g));
    }

    explicit Graph(igraph_integer_t n = 0, bool directed = false) {
        check(igraph_empty(ptr, n, directed));
    }

    explicit Graph(const igraph_vector_int_t *edges, igraph_integer_t n = 0, bool directed = false) {
        check(igraph_create(ptr, edges, n, directed));
    }

    Graph(const Graph &g) {
        check(igraph_copy(ptr, g.ptr));
    }

    Graph(Graph &&other) noexcept {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            graph = other.graph;
        }
        other.ptr = nullptr;
    }

    Graph & operator = (const Graph &) = delete;

    Graph & operator = (Graph &&other) noexcept {
        if (! is_alias())
            igraph_destroy(ptr);
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            graph = other.graph;
            ptr = &graph;
        }
        other.ptr = nullptr;
        return *this;
    }

    Graph & operator = (CaptureType<igraph_t> g) {
        if (! is_alias())
            igraph_destroy(ptr);
        graph = g.obj;
        ptr = &graph;
        return *this;
    }

    Graph & operator = (AliasType<igraph_t> g) {
        if (! is_alias())
            igraph_destroy(ptr);
        ptr = &g.obj;
        return *this;
    }

    ~Graph() {
        if (! is_alias())
            igraph_destroy(ptr);
    }

    operator igraph_t *() { return ptr; }
    operator const igraph_t *() const { return ptr; }

    friend void swap(Graph &g1, Graph &g2) noexcept {
        igraph_t tmp = *g1.ptr;
        *g1.ptr = *g2.ptr;
        *g2.ptr = tmp;
    }

    // Necessary to allow some STL algorithms to work on GraphList,
    // whose iterator dereferences to an aliasing Graph.
    friend void swap(Graph &&g1, Graph &&g2) noexcept {
        igraph_t tmp = *g1.ptr;
        *g1.ptr = *g2.ptr;
        *g2.ptr = tmp;
    }

    bool is_directed() const { return igraph_is_directed(ptr); }
    igraph_integer_t vcount() const { return igraph_vcount(ptr); }
    igraph_integer_t ecount() const { return igraph_ecount(ptr); }

    // Convenience access to basic properties (mostly those that are cached).

    bool is_connected(igraph_connectedness_t mode = IGRAPH_WEAK) const {
        igraph_bool_t res;
        check(igraph_is_connected(ptr, &res, mode));
        return res;
    }

    bool is_simple() const {
        igraph_bool_t res;
        check(igraph_is_simple(ptr, &res));
        return res;
    }

    bool has_loop() const {
        igraph_bool_t res;
        check(igraph_has_loop(ptr, &res));
        return res;
    }

    bool has_multiple() const {
        igraph_bool_t res;
        check(igraph_has_multiple(ptr, &res));
        return res;
    }

    bool has_mutual(bool loops = false) const {
        igraph_bool_t res;
        check(igraph_has_mutual(ptr, &res, loops));
        return res;
    }

    bool is_tree(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        check(igraph_is_tree(ptr, &res, nullptr, mode));
        return res;
    }

    bool is_forest(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        check(igraph_is_forest(ptr, &res, nullptr, mode));
        return res;
    }

    bool is_dag() const {
        igraph_bool_t res;
        check(igraph_is_dag(ptr, &res));
        return res;
    }

    bool is_acyclic() const {
        igraph_bool_t res;
        check(igraph_is_acyclic(ptr, &res));
        return res;
    }

    void invalidate_cache() const {
        igraph_invalidate_cache(ptr);
    }

    // Note that the comparison is between labelled graphs, i.e. it does not test
    // for isomorphism. It also ignores attributes.
    friend bool operator == (const Graph &lhs, const Graph &rhs) {
        igraph_bool_t res;
        check(igraph_is_same_graph(lhs, rhs, &res));
        return res;
    }

    friend bool operator != (const Graph &lhs, const Graph &rhs) {
        return ! (lhs == rhs);
    }
};

#define BASE_GRAPH
#include "graph_list_pmt.hpp"
#undef BASE_GRAPH

} // namespace ig

#endif // IGCPP_IGRAPH_HPP

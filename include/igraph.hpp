
#ifndef IGCPP_IGRAPH_HPP
#define IGCPP_IGRAPH_HPP

#include <igraph.h>

#if !(IGRAPH_VERSION_MAJOR == 0 && IGRAPH_VERSION_MINOR == 10)
#error "This version of igraph-cpp requires igraph 0.10."
#endif

#include <cassert>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace ig {

// Error handling

struct igException : std::runtime_error {
    igraph_error_t error;

    explicit igException(igraph_error_t error_) :
            std::runtime_error(igraph_strerror(error_)),
            error(error_) { }
};

inline void igCheck(igraph_error_t error) {
    if (error != IGRAPH_SUCCESS)
        throw igException{error};
}

// Support structures

template<typename T>
struct igCaptureType {
    T &obj;
    explicit igCaptureType(T &obj) : obj(obj) { }
};

template<typename T>
inline igCaptureType<typename std::remove_reference<T>::type>
igCapture(T &&obj) { return igCaptureType<typename std::remove_reference<T>::type>(obj); }

template<typename T>
struct igAliasType {
    T &obj;
    explicit igAliasType(T &obj) : obj(obj) { }
};

template<typename T>
inline igAliasType<T> igAlias(T &obj) { return igAliasType<T>(obj); }

// Main data structures

template<typename T> class igVec;
template<typename T> class igMat;
template<typename T> class igVecList;
template<typename T> class igMatList;

#define BASE_IGRAPH_REAL
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#include "vec_list_pmt.hpp"
#include "mat_list_pmt.hpp"
#undef BASE_IGRAPH_REAL
using igRealVec = igVec<igraph_real_t>;
using igRealMat = igMat<igraph_real_t>;
using igRealVecList = igVecList<igraph_real_t>;
using igRealMatList = igMatList<igraph_real_t>;

#define BASE_INT
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#include "vec_list_pmt.hpp"
#undef BASE_INT
using igIntVec = igVec<igraph_integer_t>;
using igIntMat = igMat<igraph_integer_t>;
using igIntVecList = igVecList<igraph_integer_t>;

#define BASE_BOOL
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#undef BASE_BOOL
using igBoolVec = igVec<igraph_bool_t>;
using igBoolMat = igMat<igraph_bool_t>;

class igGraphList;

class igGraph {
    using igraph_type = igraph_t;

    igraph_t graph;
    igraph_t *ptr = &graph;

    bool is_alias() const { return ptr != &graph; }

    friend class igGraphList;

public:
    explicit igGraph(igCaptureType<igraph_t> g) : graph(g.obj) { }
    explicit igGraph(igAliasType<igraph_t> g) : ptr(&g.obj) { }

    explicit igGraph(const igraph_t *g) {
        igCheck(igraph_copy(ptr, g));
    }

    explicit igGraph(igraph_integer_t n = 0, bool directed = false) {
        igCheck(igraph_empty(ptr, n, directed));
    }

    explicit igGraph(const igraph_vector_int_t *edges, igraph_integer_t n = 0, bool directed = false) {
        igCheck(igraph_create(ptr, edges, n, directed));
    }

    igGraph(const igGraph &g) {
        igCheck(igraph_copy(ptr, g.ptr));
    }

    igGraph(igGraph &&other) noexcept {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            graph = other.graph;
        }
        other.ptr = nullptr;
    }

    igGraph & operator = (const igGraph &) = delete;
    igGraph & operator = (igGraph &&) = delete;

    igGraph & operator = (igCaptureType<igraph_t> g) {
        if (! is_alias())
            igraph_destroy(ptr);
        graph = g.obj;
        ptr = &graph;
        return *this;
    }

    igGraph & operator = (igAliasType<igraph_t> g) {
        if (! is_alias())
            igraph_destroy(ptr);
        ptr = &g.obj;
        return *this;
    }

    ~igGraph() {
        if (! is_alias())
            igraph_destroy(ptr);
    }

    operator igraph_t *() { return ptr; }
    operator const igraph_t *() const { return ptr; }

    friend void swap(igGraph &g1, igGraph &g2) noexcept {
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
        igCheck(igraph_is_connected(ptr, &res, mode));
        return res;
    }

    bool is_simple() const {
        igraph_bool_t res;
        igCheck(igraph_is_simple(ptr, &res));
        return res;
    }

    bool has_loop() const {
        igraph_bool_t res;
        igCheck(igraph_has_loop(ptr, &res));
        return res;
    }

    bool has_multiple() const {
        igraph_bool_t res;
        igCheck(igraph_has_multiple(ptr, &res));
        return res;
    }

    bool has_mutual(bool loops = false) const {
        igraph_bool_t res;
        igCheck(igraph_has_mutual(ptr, &res, loops));
        return res;
    }

    bool is_tree(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        igCheck(igraph_is_tree(ptr, &res, nullptr, mode));
        return res;
    }

    bool is_forest(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        igCheck(igraph_is_forest(ptr, &res, nullptr, mode));
        return res;
    }

    bool is_dag() const {
        igraph_bool_t res;
        igCheck(igraph_is_dag(ptr, &res));
        return res;
    }

    bool is_acyclic() const {
        igraph_bool_t res;
        igCheck(igraph_is_acyclic(ptr, &res));
        return res;
    }

    void invalidate_cache() const {
        igraph_invalidate_cache(ptr);
    }
};

#define BASE_GRAPH
#include "graph_list_pmt.hpp"
#undef BASE_GRAPH

} // namespace ig

#endif // IGCPP_IGRAPH_HPP

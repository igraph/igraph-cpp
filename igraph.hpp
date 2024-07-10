
#ifndef IGCPP_IGRAPH_HPP
#define IGCPP_IGRAPH_HPP

#include <igraph.h>

#include <cassert>
#include <stdexcept>
#include <utility>

namespace ig {

// Error handling

struct igException : std::runtime_error {
    igraph_error_t errno;

    explicit igException(igraph_error_t errno) :
            std::runtime_error(igraph_strerror(errno)),
            errno(errno) {}
};

inline void igCheck(igraph_error_t error) {
    if (error != IGRAPH_SUCCESS)
        throw igException{error};
}

// Data structures

template<typename T> class igVec;
template<typename T> class igMat;

#define BASE_IGRAPH_REAL
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#undef BASE_IGRAPH_REAL
using igRealVec = igVec<igraph_real_t>;
using igRealMat = igMat<igraph_real_t>;

#define BASE_INT
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#undef BASE_INT
using igIntVec = igVec<igraph_integer_t>;
using igIntMat = igMat<igraph_integer_t>;

#define BASE_BOOL
#include "vec_pmt.hpp"
#include "mat_pmt.hpp"
#undef BASE_BOOL
using igBoolVec = igVec<igraph_bool_t>;
using igBoolMat = igMat<igraph_bool_t>;


class igGraph {
    igraph_t graph;
    bool active = true;

public:
    // NOTE! Takes ownership of the graph, which must already be initialized.
    explicit igGraph(igraph_t &g) : graph(g) { }

    explicit igGraph(igraph_integer_t n = 0, bool directed = false) {
        igCheck(igraph_empty(&graph, n, directed));
    }

    explicit igGraph(const igraph_vector_int_t *edges, igraph_integer_t n = 0, bool directed = false) {
        igCheck(igraph_create(&graph, edges, n, directed));
    }

    igGraph(const igGraph &g) {
        igCheck(igraph_copy(&graph, &g.graph));
    }

    igGraph(igGraph &&g) {
        graph = g.graph;
        active = false;
    }

    ~igGraph() {
        if (active)
            igraph_destroy(&graph);
    }

    operator igraph_t *() { return &graph; }
    operator const igraph_t *() const { return &graph; }

    bool is_directed() const { return igraph_is_directed(&graph); }
    igraph_integer_t vcount() const { return igraph_vcount(&graph); }
    igraph_integer_t ecount() const { return igraph_ecount(&graph); }

    // Convenience access to basic properties (mostly those that are cached).

    bool is_connected(igraph_connectedness_t mode = IGRAPH_WEAK) const {
        igraph_bool_t res;
        igCheck(igraph_is_connected(&graph, &res, mode));
        return res;
    }

    bool is_simple() const {
        igraph_bool_t res;
        igCheck(igraph_is_simple(&graph, &res));
        return res;
    }

    bool has_loop() const {
        igraph_bool_t res;
        igCheck(igraph_has_loop(&graph, &res));
        return res;
    }

    bool has_multi() const {
        igraph_bool_t res;
        igCheck(igraph_has_multiple(&graph, &res));
        return res;
    }

    bool has_mutual(bool loops = false) const {
        igraph_bool_t res;
        igCheck(igraph_has_mutual(&graph, &res, loops));
        return res;
    }

    bool is_tree(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        igCheck(igraph_is_tree(&graph, &res, nullptr, mode));
        return res;
    }

    bool is_forest(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        igCheck(igraph_is_forest(&graph, &res, nullptr, mode));
        return res;
    }

    bool is_dag() const {
        igraph_bool_t res;
        igCheck(igraph_is_dag(&graph, &res));
        return res;
    }

    bool is_acyclic() const {
        igraph_bool_t res;
        igCheck(igraph_is_acyclic(&graph, &res));
        return res;
    }

    igraph_real_t mean_degree(bool loops = true) const {
        igraph_real_t res;
        igCheck(igraph_mean_degree(&graph, &res, loops));
        return res;
    }
};

} // namespace ig

#endif // IGCPP_IGRAPH_HPP

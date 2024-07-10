
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

    bool is_directed() const { return igraph_is_directed(&graph); }
    igraph_integer_t vcount() const { return igraph_vcount(&graph); }
    igraph_integer_t ecount() const { return igraph_ecount(&graph); }
};

} // namespace ig

#endif // IGCPP_IGRAPH_HPP

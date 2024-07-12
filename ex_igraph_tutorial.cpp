
#include "igraph.hpp"

#include <iostream>

using namespace ig;

// This is a translation to C++ of the second tutorial program included in igraph's
// manual, found in examples/tuturial/turorial2.c in igraph's source tree.

int main() {
    igraph_rng_seed(igraph_rng_default(), 42); // seed RNG before first use

    igraph_t igraph;
    igraph_square_lattice(&igraph, igIntVec({30,30}),
                          0, IGRAPH_UNDIRECTED,
                          /* mutual= */ false, igBoolVec({true, true}));
    igGraph graph(igCapture(igraph));

    igraph_real_t avg_path_len;
    igraph_average_path_length(graph, &avg_path_len, nullptr, IGRAPH_UNDIRECTED, /* unconn= */ true);
    std::cout << "Average path length (lattice): " << avg_path_len << std::endl;

    igIntVec edges(20);
    for (auto &el : edges)
        el = RNG_INTEGER(0, graph.vcount());

    igraph_add_edges(graph, edges, nullptr);

    igraph_average_path_length(graph, &avg_path_len, nullptr, IGRAPH_UNDIRECTED, /* unconn= */ true);
    std::cout << "Average path length (randomized lattice): " << avg_path_len << std::endl;

    return 0;
}
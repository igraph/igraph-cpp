
#include "igraph.hpp"
#include "ex_vector_print.hpp"

#include <iostream>

using namespace ig;

// Helper function to print a graph.
std::ostream & operator << (std::ostream &out, const igGraph &g) {
    out << "directed: " << (g.is_directed() ? "true" : "false") << std::endl;
    out << "vcount: " << g.vcount() << std::endl;

    igIntVec edges;
    igraph_edges(g, igraph_ess_all(IGRAPH_EDGEORDER_ID), edges);
    out << "edges: " << edges << std::endl;
    return out;
}

int main() {

    // Create a directed graph from its edge list.
    igGraph g(igIntVec{0,1, 1,2, 3,4, 5,6}, 8, IGRAPH_DIRECTED);

    // igGraphList is a wrapper for igraph_graph_list_t.
    igGraphList list;

    // Decompose the graph into its weakly connected components.
    igraph_decompose(g, list, IGRAPH_WEAK, -1, -1);

    // Print each component.
    std::cout << "\nGraph components:\n" << std::endl;
    for (const auto &el : list)
        std::cout << el << std::endl;

    return 0;
}
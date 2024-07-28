
#include <igraph.hpp>
#include "ex_vector_print.hpp"

#include <iostream>

using namespace ig;

// ig::Graph is a wrapper for igraph_t. This file illustrates its use.

int main() {

    // Basic graph handling example.
    {
        // Create a graph from an edge list.
        const Graph g(IntVec{0,1, 0,2, 2,3});
        std::cout << "Vertex count: " << g.vcount() << ", edge count: " << g.ecount() << std::endl;

        // Print its degrees.
        IntVec deg;
        igraph_degree(g, deg, igraph_vss_all(), IGRAPH_ALL, IGRAPH_LOOPS);
        std::cout << "Degrees: " << deg << std::endl;

        // Print some of its properties. There are convenience member functions for the most
        // commonly used graph properties, most of which are also cached in the igraph_t.
        std::cout << "Tree? " << (g.is_tree() ? "Yes" : "No") << std::endl;
        std::cout << "Connected? " << (g.is_connected() ? "Yes" : "No") << std::endl;
        std::cout << "Has loops? " << (g.has_loop() ? "Yes" : "No") << std::endl;

        // Prints its adjacency matrix.
        RealMat am;
        igraph_get_adjacency(g, am, IGRAPH_GET_ADJACENCY_BOTH, nullptr, IGRAPH_LOOPS_TWICE);
        std::cout << "Adjacency matrix:" << std::endl;
        igraph_matrix_print(am);
    }

    // How do we use arbitrary graph generators?
    {
        // First create a raw igraph_t object.
        igraph_t ig;
        igraph_kary_tree(&ig, 5, 2, IGRAPH_TREE_UNDIRECTED);

        // Then associate it with an ig::Graph. The ig::Capture wrapper indicates
        // that from now on the ig::Graph owns the igraph_t, i.e. it captures it
        // and becomes responsible for its destruction.
        const Graph g(Capture(ig));

        // Compute and print the closeness centrality.
        RealVec closeness;
        igraph_closeness(g, closeness, nullptr, nullptr, igraph_vss_all(), IGRAPH_ALL, nullptr, true);

        std::cout << "Closeness: " << closeness << std::endl;
    }

    return 0;
}

#include <igraph.hpp>
#include "ex_vector_print.hpp"

#include <iostream>

using namespace ig;

int main() {

    // Basic graph handling example.
    {
        // Create a graph from an edge list.
        const igGraph g(igIntVec{0,1, 0,2, 2,3});
        std::cout << "Vertex count: " << g.vcount() << ", edge count: " << g.ecount() << std::endl;

        // Print its degrees.
        igIntVec deg;
        igraph_degree(g, deg, igraph_vss_all(), IGRAPH_ALL, IGRAPH_LOOPS);
        std::cout << "Degrees: " << deg << std::endl;

        // Print some of its properties.
        std::cout << "Mean degree: " << g.mean_degree() << std::endl;
        std::cout << "Tree? " << (g.is_tree() ? "Yes" : "No") << std::endl;
        std::cout << "Connected? " << (g.is_connected() ? "Yes" : "No") << std::endl;
        std::cout << "Has loops? " << (g.has_loop() ? "Yes" : "No") << std::endl;

        // Prints its adjacency matrix.
        igRealMat am;
        igraph_get_adjacency(g, am, IGRAPH_GET_ADJACENCY_BOTH, nullptr, IGRAPH_LOOPS_TWICE);
        std::cout << "Adjacency matrix:" << std::endl;
        igraph_matrix_print(am);
    }

    // How do we use arbitrary graph generators?
    {
        // First create a raw igraph_t object.
        igraph_t ig;
        igraph_kary_tree(&ig, 5, 2, IGRAPH_TREE_UNDIRECTED);

        // NOTE! Here 'g' "captures" 'ig', i.e. takes ownership of it,
        // and will destroy it when it goes out of scope.
        const igGraph g(igCapture(ig));

        // Compute and print the closeness centrality.
        igRealVec closeness;
        igraph_closeness(g, closeness, nullptr, nullptr, igraph_vss_all(), IGRAPH_ALL, nullptr, true);

        std::cout << "Closeness: " << closeness << std::endl;
    }

    return 0;
}
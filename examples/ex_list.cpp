
#include <igraph.hpp>
#include "ex_vector_print.hpp"

#include <iostream>

using namespace ig;

int main() {

    // Since we will use stochastic graph generators, we must set the random seed.
    igraph_rng_seed(igraph_rng_default(), 42);

    igraph_t ig;
    igraph_erdos_renyi_game_gnp(&ig, 10, 0.5, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);
    igGraph g(igCapture(ig));

    // igIntVecList is a wrapper for igraph_vector_int_list.
    // It is a convenience alias to igIntVec<igraph_integer_t>.
    igIntVecList list;

    // Compute maximal cliques and write the result into 'list'.
    igraph_maximal_cliques(g, list, -1, -1);

    // Print the cliques.
    std::cout << "\nMaximal cliques:" << std::endl;
    for (const auto &vec : list)
        std::cout << '(' << vec << ')' << std::endl;

    // Remove the last element of the list.
    list.pop_back();

    // Add a new empty element to the list.
    list.push_back_new();

    // Add another element.
    list.push_back(igIntVec{1,2,3});

    // Print the list again.
    std::cout << "\nModified vector list:" << std::endl;
    for (const auto &vec : list)
        std::cout << '(' << vec << ')' << std::endl;

    return 0;
}
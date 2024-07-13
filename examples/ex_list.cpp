
#include <igraph.hpp>
#include "ex_vector_print.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace ig;

// Support for printing vector lists.
template<typename T>
std::ostream & operator << (std::ostream &out, const igVecList<T> &list) {
    for (const auto &vec : list)
        std::cout << '(' << vec << ')' << std::endl;
    return out;
}

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
    std::cout << "\nMaximal cliques:\n" << list << std::endl;

    // Remove the last element of the list.
    list.pop_back();

    // Add a new empty element to the list.
    list.push_back_new();

    // Check that it is actually empty.
    assert(list.back().empty());

    // Add another element.
    list.push_back(igIntVec{1,2,3});

    // Reverse the list.
    std::reverse(list.begin(), list.end());

    // Print the list again.
    std::cout << "\nModified vector list:\n" << list << std::endl;

    return 0;
}
#include <iostream>
#include <igraph.hpp>
#include "ex_vector_print.hpp"

int main() {
    // Check that we can influence the RNG.
    {
        ig::RNGScope filled(1000);
        std::cout << "New RNG scope:" << std::endl;
        std::cout << "  Draw from [0, 100]: " << igraph_rng_get_integer(igraph_rng_default(), 0, 100) << std::endl;
        std::cout << "  Draw from [0, 100]: " << igraph_rng_get_integer(igraph_rng_default(), 0, 100) << std::endl;
    }

    // Check that the destructor correctly unsets the RNG.
    {
        ig::RNGScope filled(1000);
        std::cout << "New RNG scope:" << std::endl;
        std::cout << "  Draw from [0, 100]: " << igraph_rng_get_integer(igraph_rng_default(), 0, 100) << std::endl;

        {
            ig::RNGScope nested; // uses a default seed and type.
            std::cout << "  New RNG scope:" << std::endl;
            std::cout << "    Draw from [0, 100]: " << igraph_rng_get_integer(igraph_rng_default(), 0, 100) << std::endl;
            std::cout << "    Draw from [0, 100]: " << igraph_rng_get_integer(igraph_rng_default(), 0, 100) << std::endl;
        }

        std::cout << "  Draw from [0, 100]: " << igraph_rng_get_integer(igraph_rng_default(), 0, 100) << std::endl;
    }

    return 0;
}

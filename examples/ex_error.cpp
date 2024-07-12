
#include <igraph.hpp>

#include <iostream>

using namespace ig;

int main() {

    // igraph's default error handler aborts the program on error. We can turn
    // off this behaviour, and convert error codes to exceptions using igCheck().
    igraph_set_error_handler(igraph_error_handler_printignore);

    try {
        // Trying to construct a graph from invalid vertex IDs.
        igGraph g{igIntVec{-1,0, 0,1}};
    } catch (const std::exception &ex) {
        std::cerr << "Caught exception: " << ex.what() << std::endl;
    }

    try {
        // Using invalid (negative) weights in a betweennes calculation.
        igGraph g{igIntVec{0,1, 1,2}};
        igRealVec weights = {1.0, -2.0};
        igRealVec betweenness;

        igCheck(igraph_betweenness(g, betweenness, igraph_vss_all(), IGRAPH_DIRECTED, weights));
    } catch (const std::exception &ex) {
        std::cerr << "Caught exception: " << ex.what() << std::endl;
    }

    return 0;
}
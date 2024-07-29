
#include <igraph.hpp>
#include <iostream>

using namespace ig;

int main() {

    {
        Bitset bs1 = {true, false, true};
        Bitset bs2 = {false, false, true};

        igraph_bitset_print(bs1);
        std::cout << std::endl;
        igraph_bitset_print(bs2);
        std::cout << std::endl;

        igraph_bitset_and(bs1, bs2, bs1);
        igraph_bitset_print(bs1);
        std::cout << std::endl;
    }

    {
        RNGScope rng(42);

        igraph_t ig;
        igraph_erdos_renyi_game_gnm(&ig, 20, 30, IGRAPH_DIRECTED, false);
        Graph g(Capture(ig));

        IntVec membership;
        BitsetList reach;
        igraph_reachability(g, membership, nullptr, nullptr, reach, IGRAPH_OUT);

        for (auto el: reach) {
            igraph_bitset_print(el);
            std::cout << std::endl;
        }
    }

    return 0;
}
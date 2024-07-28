
#include <igraph.hpp>
#include <iostream>

// This example illustrates the use of ig::RNGScope, which provides a localized
// random state within the current scope.

int main() {
    // Check that we can influence the RNG.
    {
        ig::RNGScope filled(42);
        std::cout << "New RNG scope:" << std::endl;
        std::cout << "  Draw from [0, 100]: " << RNG_INTEGER(0, 100) << std::endl;
        std::cout << "  Draw from [0, 100]: " << RNG_INTEGER(0, 100) << std::endl;
    }

    // Check that the destructor correctly unsets the RNG.
    {
        ig::RNGScope filled(42);
        std::cout << "New RNG scope:" << std::endl;
        std::cout << "  Draw from [0, 100]: " << RNG_INTEGER(0, 100) << std::endl;

        {
            ig::RNGScope nested; // uses a default type and a seed generated via std::random_device
            std::cout << "  New RNG scope:" << std::endl;
            std::cout << "    Draw from [0, 100]: " << RNG_INTEGER(0, 100) << std::endl;
            std::cout << "    Draw from [0, 100]: " << RNG_INTEGER(0, 100) << std::endl;
        }

        std::cout << "  Draw from [0, 100]: " << RNG_INTEGER(0, 100) << std::endl;
    }

    return 0;
}

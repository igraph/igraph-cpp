
#include <igraph.hpp>
#include <iostream>

using namespace ig;

int main() {
    StrVec sv = {"foo", "bar"};

    sv.push_back("baz");
    sv.resize(5);
    sv[4] = "xyz";

    for (auto el : sv)
        std::cout << '"' << el << '"' << '\n';

    return 0;
}

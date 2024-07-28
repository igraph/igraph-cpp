
#include <igraph.hpp>
#include <iostream>

using namespace ig;

// This example illustrates the use of ig::StrVec, a wrapper for igraph_strvector_t
// Its interface is very similar to that of ig::Vec, but somewhat more limited due
// to the lesser capabilities of igraph_strvector_t.

int main() {
    StrVec sv = {"foo", "bar"};

    // Equivalent to igraph_strvector_push_back():
    sv.push_back("baz");

    // Equivalent to igraph_strvector_push_back_len():
    sv.push_back("only first 4 characters taken from long string", 4);

    // Equivalent to igraph_strvector_resize(), adds empty strings:
    sv.resize(6);

    // Equivalent to igraph_strvector_set():
    sv[5] = "xyz";

    for (auto el : sv)
        std::cout << '"' << el << '"' << '\n';

    // Note tha this class uses proxy iterator and reference classes, therefore it is
    // only possible to _read_ it (but not _write_) using a range-based for loop with
    // the auto keyword. The following is invalid:

    /*
    for (auto &el : sv)
        el = "abc";
    */

    return 0;
}

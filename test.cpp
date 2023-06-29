
#include "igraph.hpp"

#include <iostream>

using namespace std;

int main() {

    // igRealVec is a RAII wrapper to igraph_vector_t.
    // When it goes out of scope, it is properly destroyed.
    // igRealVec is a convenience name for igVec<igraph_real_t>.
    igRealVec v;

    // initial size is zero
    cout << v.size() << endl;

    // igVec<XXX> is convertible to the corresponding igraph_vector_XXX_t * pointer.
    igCheck(igraph_vector_resize(v, 10));
    cout << v.size() << endl;

    // igVec<> has STL-compatible convenience member functions for common operations.
    v.resize(5);
    cout << v.size() << endl;

    // The indexing operator is available.
    v[3] = 1; v[4] = 2;

    // The STL comatibility provides support for range-based for loops.
    for (auto el : v)
        cout << el << ' ';
    cout << endl;

    return 0;
}
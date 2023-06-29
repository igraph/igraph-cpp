
#include "igraph.hpp"

#include <iostream>

using namespace std;

int main() {
    igRealVec v;

    cout << v.size() << endl;

    igraph_vector_resize(v, 10);

    cout << v.size() << endl;

    return 0;
}
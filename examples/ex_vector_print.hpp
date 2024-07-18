
#include <igraph.hpp>
#include <iostream>

using namespace ig;

// Helper function to print Vec objects. Since Vec is templated, it is easy
// to use a single definition for all of its specializations.
template<typename T>
std::ostream & operator << (std::ostream &out, const Vec<T> &v) {
    for (auto it = v.begin(); it < v.end() - 1; ++it) {
        out << *it << ' ';
    }
    if (v.size() > 0) {
        out << v.back();
    }
    return out;
}


#include <igraph.hpp>
#include "ex_vector_print.hpp"

using namespace ig;

int main() {
    igraph_vector_t ig_vec1;
    igraph_vector_init_range(&ig_vec1, 0, 5);

    // Here 'vec1' takes over the ownership of 'ig_vec1', and will free the underlying
    // storage when 'vec1' goes out of scope. From this point on, 'ig_vec1' should not
    // be used anymore.
    RealVec vec1(Capture(ig_vec1));
    std::cout << vec1 << std::endl;

    igraph_real_t values[] = {1.2, 2.3, 3.4};
    igraph_vector_t ig_vec2;
    igraph_vector_view(&ig_vec2, values, sizeof(values) / sizeof(values[0]));

    // Here 'vec2' simply holds a reference to 'ig_vec2', i.e. it aliases it.
    // 'vec2' provides a simple interface to the igraph vector, but it will not free the
    // underlying storage. This is necessary in some cases, such as when a vector is a
    // view into some existing buffer. If 'ig_vec2' ceases to exist, 'vec2' will no longer
    // be valid.
    const RealVec vec2(Alias(ig_vec2));
    std::cout << vec2 << std::endl;

    return 0;
}
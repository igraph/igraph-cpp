
#ifndef IGCPP_IGRAPH_HPP
#define IGCPP_IGRAPH_HPP

#include <igraph.h>
#include <stdexcept>

namespace ig {

    template<typename T>
    class igVec;

    struct igException : std::runtime_error {
        igraph_error_t errno;

        explicit igException(igraph_error_t errno) :
                std::runtime_error(igraph_strerror(errno)),
                errno(errno) {}
    };

    inline void igCheck(igraph_error_t error) {
        if (error != IGRAPH_SUCCESS)
            throw igException{error};
    }

#define BASE_IGRAPH_REAL

#include "igraph_pmt.hpp"
#undef BASE_IGRAPH_REAL
typedef igVec<igraph_real_t> igRealVec;

#define BASE_INT
#include "igraph_pmt.hpp"
#undef BASE_INT
typedef igVec<igraph_integer_t> igIntVec;

#define BASE_BOOL
#include "igraph_pmt.hpp"

#undef BASE_BOOL
typedef igVec<igraph_bool_t> igBoolVec;

} // namespace ig

#endif // IGCPP_IGRAPH_HPP

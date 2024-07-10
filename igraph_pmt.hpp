
#include <igraph_pmt.h>

template<> class igVec<BASE> {
    TYPE(igraph_vector) vec;

public:
    using value_type = BASE;
    using size_type = igraph_integer_t;
    using difference_type = igraph_integer_t;

    explicit igVec<BASE>(igraph_integer_t n = 0) {
        FUNCTION(igraph_vector, init)(&vec, n);
    }

    igVec<BASE>(igVec<BASE> &&other) {
        vec = other.vec;
        other.vec.stor_begin = nullptr;
    }

    igVec<BASE>(const igVec<BASE> &other) {
        FUNCTION(igraph_vector, init_copy)(&vec, &other.vec);
    }

    igVec<BASE>(const TYPE(igraph_vector) *v) {
        FUNCTION(igraph_vector, init_copy)(&vec, v);
    }

    igVec<BASE> & operator = (const igVec<BASE> &other) {
        igCheck(FUNCTION(igraph_vector, update)(&vec, &other.vec));
        return *this;
    }

    ~igVec<BASE>() {
        FUNCTION(igraph_vector, destroy)(&vec);
    }

    operator TYPE(igraph_vector) *() { return &vec; }

    BASE *begin() { return vec.stor_begin; }
    BASE *end() { return vec.end; }

    const BASE *begin() const { return vec.stor_begin; }
    const BASE *end() const { return vec.end; }

    BASE *data() { return begin(); }

    BASE &back() { return *(vec.end-1); }
    const BASE &back() const { return *(vec.end-1); }

    size_type size() const { return vec.end - vec.stor_begin; }
    size_type capacity() const { return vec.stor_end - vec.stor_begin; }

    BASE & operator [] (size_type i) { return begin()[i]; }
    const BASE & operator [] (size_type i) const { return begin()[i]; }

    void clear() { FUNCTION(igraph_vector, clear)(&vec); }
    void resize(size_type size) { igCheck(FUNCTION(igraph_vector, resize)(&vec, size)); }
    void reserve(size_type capacity) { igCheck(FUNCTION(igraph_vector, reserve)(&vec, capacity)); }
    void shrink_to_fit() { FUNCTION(igraph_vector, resize_min)(&vec); }

    void push_back(BASE elem) { igCheck(FUNCTION(igraph_vector, push_back)(&vec, elem)); }
    BASE pop_back() { return FUNCTION(igraph_vector, pop_back)(&vec); }

    BASE *erase(const BASE *pos) {
        FUNCTION(igraph_vector, remove)(&vec, pos - vec.stor_begin);
        return const_cast<BASE *>(pos);
    }

    BASE *erase(const BASE *first, const BASE *last) {
        FUNCTION(igraph_vector, remove_section)(&vec, first - vec.stor_begin, last - vec.stor_begin);
        return const_cast<BASE *>(first);
    }

    void swap(igVec<BASE> &other) { igCheck(FUNCTION(igraph_vector, swap)(&vec, &other.vec)); }
};

#include <igraph_pmt_off.h>

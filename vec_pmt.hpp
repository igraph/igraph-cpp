
#include <igraph_pmt.h>

template<> class igVec<BASE> {
    TYPE(igraph_vector) vec;
    TYPE(igraph_vector) *ptr = &vec;

    bool is_alias() const { return ptr != &vec; }

public:
    using value_type = BASE;
    using size_type = igraph_integer_t;
    using difference_type = igraph_integer_t;

    explicit igVec<BASE>(igCaptureType<TYPE(igraph_vector)> v) : vec(v.obj) { }
    explicit igVec<BASE>(igAliasType<TYPE(igraph_vector)> v) : ptr(&v.obj) { }

    explicit igVec<BASE>(size_type n = 0) {
        igCheck(FUNCTION(igraph_vector, init)(ptr, n));
    }

    igVec<BASE>(igVec<BASE> &&other) {
        vec = other.vec;
        other.ptr = nullptr;
    }

    igVec<BASE>(const igVec<BASE> &other) {
        igCheck(FUNCTION(igraph_vector, init_copy)(ptr, other.ptr));
    }

    igVec<BASE>(const TYPE(igraph_vector) *v) {
        igCheck(FUNCTION(igraph_vector, init_copy)(ptr, v));
    }

    igVec<BASE>(std::initializer_list<BASE> list) {
        igCheck(FUNCTION(igraph_vector, init_array)(ptr, list.begin(), list.size()));
    }

    igVec<BASE> & operator = (const igVec<BASE> &other) {
        igCheck(FUNCTION(igraph_vector, update)(ptr, other.ptr));
        return *this;
    }

    ~igVec<BASE>() {
        if (! is_alias())
            FUNCTION(igraph_vector, destroy)(ptr);
    }

    operator TYPE(igraph_vector) *() { return ptr; }
    operator const TYPE(igraph_vector) *() const { return ptr; }

    BASE *begin() { return ptr->stor_begin; }
    BASE *end() { return ptr->end; }

    const BASE *begin() const { return ptr->stor_begin; }
    const BASE *end() const { return ptr->end; }

    BASE *data() { return begin(); }

    BASE &back() { return *(ptr->end-1); }
    const BASE &back() const { return *(ptr->end-1); }

    size_type size() const { return ptr->end - ptr->stor_begin; }
    size_type capacity() const { return ptr->stor_end - ptr->stor_begin; }

    BASE & operator [] (size_type i) { return begin()[i]; }
    const BASE & operator [] (size_type i) const { return begin()[i]; }

    void clear() { FUNCTION(igraph_vector, clear)(ptr); }
    void resize(size_type size) { igCheck(FUNCTION(igraph_vector, resize)(ptr, size)); }
    void reserve(size_type capacity) { igCheck(FUNCTION(igraph_vector, reserve)(ptr, capacity)); }
    void shrink_to_fit() { FUNCTION(igraph_vector, resize_min)(ptr); }

    void push_back(BASE elem) { igCheck(FUNCTION(igraph_vector, push_back)(ptr, elem)); }
    BASE pop_back() { return FUNCTION(igraph_vector, pop_back)(ptr); }

    BASE *erase(const BASE *pos) {
        FUNCTION(igraph_vector, remove)(ptr, pos - ptr->stor_begin);
        return const_cast<BASE *>(pos);
    }

    BASE *erase(const BASE *first, const BASE *last) {
        FUNCTION(igraph_vector, remove_section)(ptr, first - ptr->stor_begin, last - ptr->stor_begin);
        return const_cast<BASE *>(first);
    }

    void swap(igVec<BASE> &other) { igCheck(FUNCTION(igraph_vector, swap)(ptr, other.ptr)); }
};

#include <igraph_pmt_off.h>

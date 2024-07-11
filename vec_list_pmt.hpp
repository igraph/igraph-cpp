
#define VECTOR_LIST
#include <igraph_pmt.h>

template<> class igVecList<BASE> {
    TYPE list;
    TYPE *ptr = &list;

    bool is_alias() const { return ptr != &list; }

public:
    using value_type = igVec<BASE>;
    using size_type = igraph_integer_t;
    using difference_type = igraph_integer_t;

    explicit igVecList<BASE>(igCaptureType<TYPE> vl) : list(vl.obj) { }
    explicit igVecList<BASE>(igAliasType<TYPE> vl) : ptr(&vl.obj) { }

    explicit igVecList<BASE>(size_type n = 0) {
        igCheck(FUNCTION(init)(ptr, n));
    }

    igVecList<BASE>(igVecList<BASE> &&other) {
        list = other.list;
        other.ptr = nullptr;
    }

    igVecList<BASE>(const igVecList<BASE> &other) = delete;

    igVecList<BASE> & operator = (const igVecList<BASE> &other) = delete;

    ~igVecList<BASE>() {
        if (! is_alias())
            FUNCTION(destroy)(ptr);
    }

    operator TYPE *() { return ptr; }
    operator const TYPE *() const { return ptr; }

    size_type size() const { return ptr->end - ptr->stor_begin; }
    size_type capacity() const { return ptr->stor_end - ptr->stor_begin; }

    value_type operator [] (size_type i) { return value_type(igAlias(ptr->stor_begin[i])); }
    const value_type operator [] (size_type i) const { return value_type(igAlias(ptr->stor_begin[i])); }

    void clear() { FUNCTION(clear)(ptr); }
    void resize(size_type size) { igCheck(FUNCTION(resize)(ptr, size)); }
    void reserve(size_type capacity) { igCheck(FUNCTION(reserve)(ptr, capacity)); }

    void swap(igVecList<BASE> &other) { igCheck(FUNCTION(swap)(ptr, other.ptr)); }
};

#include <igraph_pmt_off.h>
#undef VECTOR_LIST

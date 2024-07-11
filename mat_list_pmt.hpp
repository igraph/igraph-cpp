
#define MATRIX_LIST
#include <igraph_pmt.h>

template<> class igMatList<BASE> {
    using igraph_type = TYPE;

    igraph_type list;
    igraph_type *ptr = &list;

    bool is_alias() const { return ptr != &list; }

public:
    using value_type = igMat<BASE>;
    using size_type = igraph_integer_t;
    using difference_type = igraph_integer_t;

    explicit igMatList<BASE>(igCaptureType<igraph_type> vl) : list(vl.obj) { }
    explicit igMatList<BASE>(igAliasType<igraph_type> vl) : ptr(&vl.obj) { }

    explicit igMatList<BASE>(size_type n = 0) {
        igCheck(FUNCTION(init)(ptr, n));
    }

    igMatList<BASE>(igMatList<BASE> &&other) {
        list = other.list;
        other.ptr = nullptr;
    }

    igMatList<BASE>(const igMatList<BASE> &other) = delete;

    igMatList<BASE> & operator = (const igMatList<BASE> &other) = delete;

    ~igMatList<BASE>() {
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

    void swap(igMatList<BASE> &other) { igCheck(FUNCTION(swap)(ptr, other.ptr)); }
};

#include <igraph_pmt_off.h>
#undef MATRIX_LIST

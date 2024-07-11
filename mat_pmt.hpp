
#include <igraph_pmt.h>

template<> class igMat<BASE> {
    TYPE(igraph_matrix) mat;
    TYPE(igraph_matrix) *ptr = &mat;

    bool is_alias() const { return ptr != &mat; }

public:
    using value_type = BASE;
    using size_type = igraph_integer_t;
    using difference_type = igraph_integer_t;

    explicit igMat<BASE>(igCaptureType<TYPE(igraph_matrix)> m) : mat(m.obj) { }
    explicit igMat<BASE>(igAliasType<TYPE(igraph_matrix)> m) : ptr(&m.obj) { }

    explicit igMat<BASE>(size_type n = 0, size_type m = 0) {
        igCheck(FUNCTION(igraph_matrix, init)(ptr, n, m));
    }

    igMat<BASE>(igMat<BASE> &&other) {
        mat = other.mat;
        other.ptr = nullptr;
    }

    igMat<BASE>(const igMat<BASE> &other) {
        igCheck(FUNCTION(igraph_matrix, init_copy)(ptr, other.ptr));
    }

    igMat<BASE>(const TYPE(igraph_matrix) *v) {
        igCheck(FUNCTION(igraph_matrix, init_copy)(ptr, v));
    }

    igMat<BASE> & operator = (const igMat<BASE> &other) {
        igCheck(FUNCTION(igraph_matrix, update)(ptr, other.ptr));
        return *this;
    }

    igMat<BASE>(std::initializer_list<std::initializer_list<BASE>> values) {
        size_type n = values.size();
        size_type m = n > 0 ? values.begin()->size() : 0;
        igCheck(FUNCTION(igraph_matrix, init)(ptr, n, m));

        size_type i = 0;
        for (const auto &row : values) {
            assert(row.size() == m);
            size_type j = 0;
            for (const auto &el : row) {
                MATRIX(*ptr, i, j) = el;
                j++;
            }
            i++;
        }
    }

    ~igMat<BASE>() {
        if (!is_alias()) {
            FUNCTION(igraph_matrix, destroy)(ptr);
        }
    }

    operator TYPE(igraph_matrix) *() { return ptr; }
    operator const TYPE(igraph_matrix) *() const { return ptr; }

    BASE *begin() { return ptr->data.stor_begin; }
    BASE *end() { return ptr->data.end; }

    const BASE *begin() const { return ptr->data.stor_begin; }
    const BASE *end() const { return ptr->data.end; }

    BASE *data() { return begin(); }

    size_type size() const { return ptr->data.end - ptr->data.stor_begin; }
    size_type capacity() const { return ptr->data.stor_end - ptr->data.stor_begin; }

    size_type nrow() const { return ptr->nrow; }
    size_type ncol() const { return ptr->ncol; }

    BASE & operator [] (size_type i) { return begin()[i]; }
    const BASE & operator [] (size_type i) const { return begin()[i]; }

    BASE & operator () (size_type i, size_type j) { return MATRIX(*ptr, i, j); }
    const BASE & operator () (size_type i, size_type j) const { return MATRIX(*ptr, i, j); }

    void resize(size_type n, size_type m) { igCheck(FUNCTION(igraph_matrix, resize)(ptr, n, m)); }
    void shrink_to_fit() { FUNCTION(igraph_matrix, resize_min)(ptr); }

    void swap(igMat<BASE> &other) { igCheck(FUNCTION(igraph_matrix, swap)(ptr, other.ptr)); }
};

#include <igraph_pmt_off.h>

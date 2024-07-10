
#include <igraph_pmt.h>

template<> class igMat<BASE> {
    TYPE(igraph_matrix) mat;

public:
    using value_type = BASE;
    using size_type = igraph_integer_t;
    using difference_type = igraph_integer_t;

    explicit igMat<BASE>(size_type n = 0, size_type m = 0) {
        FUNCTION(igraph_matrix, init)(&mat, n, m);
    }

    igMat<BASE>(igMat<BASE> &&other) {
        mat = other.mat;
        other.mat.data.stor_begin = nullptr;
    }

    igMat<BASE>(const igMat<BASE> &other) {
        FUNCTION(igraph_matrix, init_copy)(&mat, &other.mat);
    }

    igMat<BASE>(const TYPE(igraph_matrix) *v) {
        FUNCTION(igraph_matrix, init_copy)(&mat, v);
    }

    igMat<BASE> & operator = (const igMat<BASE> &other) {
        igCheck(FUNCTION(igraph_matrix, update)(&mat, &other.mat));
        return *this;
    }

    igMat<BASE>(std::initializer_list<std::initializer_list<BASE>> values) {
        size_type n = values.size();
        size_type m = n > 0 ? values.begin()->size() : 0;
        FUNCTION(igraph_matrix, init)(&mat, n, m);

        size_type i = 0;
        for (const auto &row : values) {
            assert(row.size() == m);
            size_type j = 0;
            for (const auto &el : row) {
                MATRIX(mat, i, j) = el;
                j++;
            }
            i++;
        }
    }

    ~igMat<BASE>() {
        FUNCTION(igraph_matrix, destroy)(&mat);
    }

    operator TYPE(igraph_matrix) *() { return &mat; }

    BASE *begin() { return mat.data.stor_begin; }
    BASE *end() { return mat.data.end; }

    const BASE *begin() const { return mat.data.stor_begin; }
    const BASE *end() const { return mat.data.end; }

    BASE *data() { return begin(); }

    size_type size() const { return mat.data.end - mat.data.stor_begin; }
    size_type capacity() const { return mat.data.stor_end - mat.data.stor_begin; }

    BASE & operator [] (size_type i) { return begin()[i]; }
    const BASE & operator [] (size_type i) const { return begin()[i]; }

    BASE & operator () (size_type i, size_type j) { return MATRIX(mat, i, j); }
    const BASE & operator () (size_type i, size_type j) const { return MATRIX(mat, i, j); }

    void resize(size_type n, size_type m) { igCheck(FUNCTION(igraph_matrix, resize)(&mat, n, m)); }
    void shrink_to_fit() { FUNCTION(igraph_matrix, resize_min)(&mat); }

    void swap(igMat<BASE> &other) { igCheck(FUNCTION(igraph_matrix, swap)(&mat, &other.mat)); }
};

#include <igraph_pmt_off.h>

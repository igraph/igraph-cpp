
#include <igraph_pmt.h>

template<> class LIST_TYPE {
    using igraph_type = TYPE;

    igraph_type list;
    igraph_type *ptr = &list;

    bool is_alias() const { return ptr != &list; }

public:
    using value_type = ELEM_TYPE;
    using size_type = igraph_integer_t;
    using difference_type = igraph_integer_t;

    class iterator;

    explicit LIST_TYPE(igCaptureType<igraph_type> tl) : list(tl.obj) { }
    explicit LIST_TYPE(igAliasType<igraph_type> tl) : ptr(&tl.obj) { }

    explicit LIST_TYPE(size_type n = 0) {
        igCheck(FUNCTION(init)(ptr, n));
    }

    LIST_TYPE(LIST_TYPE &&other) {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            list = other.list;
        }
        other.ptr = nullptr;
    }

    LIST_TYPE(const LIST_TYPE &other) = delete;

    LIST_TYPE & operator = (const LIST_TYPE &other) = delete;

    LIST_TYPE & operator = (LIST_TYPE &&other) = delete;

    ~LIST_TYPE() {
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

    iterator begin();
    iterator end();

    void push_back(value_type &t) {
        igCheck(FUNCTION(push_back)(ptr, t));
        t.ptr = FUNCTION(tail_ptr)(ptr); // set as alias
    }

    void push_back(value_type &&t) {
        igCheck(FUNCTION(push_back)(ptr, t));
        t.ptr = nullptr;
    }

    void push_back(value_type::igraph_type *t) {
        igCheck(FUNCTION(push_back)(ptr, t));
    }

    void push_back_copy(const value_type::igraph_type *t) {
        igCheck(FUNCTION(push_back_copy)(ptr, t));
    }

    value_type push_back_new() {
        value_type::igraph_type *t;
        igCheck(FUNCTION(push_back_new)(ptr, &t));
        return value_type(igAlias(*t));
    }

    value_type pop_back() {
        return value_type(igCapture(FUNCTION(pop_back)(ptr)));
    }

    void swap(LIST_TYPE &other) { igCheck(FUNCTION(swap)(ptr, other.ptr)); }
};

class LIST_TYPE::iterator : public std::iterator<std::random_access_iterator_tag,
                                                        value_type,
                                                        difference_type,
                                                        value_type *,
                                                        value_type>
{
    value_type::igraph_type *p;

    friend class LIST_TYPE;
    iterator(value_type::igraph_type *p) : p(p) { }

public:

    reference operator * () const { return value_type(igAlias(*p)); }
    reference operator [] (difference_type i) const { return value_type(igAlias(p[i])); }

    iterator & operator ++ () { ++p; return *this; }
    iterator operator ++ (int) { ++p; return *this; }
    iterator & operator -- () { --p; return *this; }
    iterator operator -- (int) { --p; return *this; }

    iterator & operator += (difference_type n) { p += n; return *this; }
    iterator & operator -= (difference_type n) { p -= n; return *this; }

    friend bool operator == (const iterator &lhs, const iterator &rhs) {
        return lhs.p == rhs.p;
    }

    friend bool operator != (const iterator &lhs, const iterator &rhs) {
        return lhs.p != rhs.p;
    }

    friend bool operator < (const iterator &lhs, const iterator &rhs) {
        return lhs.p < rhs.p;
    }

    friend bool operator > (const iterator &lhs, const iterator &rhs) {
        return lhs.p > rhs.p;
    }

    friend bool operator <= (const iterator &lhs, const iterator &rhs) {
        return lhs.p <= rhs.p;
    }

    friend bool operator >= (const iterator &lhs, const iterator &rhs) {
        return lhs.p >= rhs.p;
    }

    friend iterator operator + (const iterator &it, difference_type n) {
        return it.p + n;
    }

    friend iterator operator - (const iterator &it, difference_type n) {
        return it.p - n;
    }

    friend difference_type operator - (const iterator &lhs, const iterator &rhs) {
        return lhs.p - rhs.p;
    }
};

LIST_TYPE::iterator LIST_TYPE::begin() { return ptr->stor_begin; }
LIST_TYPE::iterator LIST_TYPE::end() { return ptr->end; }

#include <igraph_pmt_off.h>

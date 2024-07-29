
#include <igraph_pmt.h>

#if !defined(GRAPH_LIST) && !defined(BITSET_LIST)
template<>
#endif
class LIST_TYPE_TEMPL {
    template<typename ValueType, typename Reference> class base_iterator;

public:
    using igraph_type = TYPE;

    using value_type = ELEM_TYPE;
    using reference = value_type;
    using const_reference = const reference;
    using difference_type = igraph_integer_t;
    using size_type = igraph_integer_t;

    using iterator = base_iterator<value_type, reference>;
    using const_iterator = base_iterator<const value_type, const_reference>;

private:
    igraph_type list;
    igraph_type *ptr = &list;

    bool is_alias() const { return ptr != &list; }

public:
    explicit LIST_TYPE(CaptureType<igraph_type> tl) : list(tl.obj) { }
    explicit LIST_TYPE(AliasType<igraph_type> tl) : ptr(&tl.obj) { }

    explicit LIST_TYPE(size_type n = 0) {
        check(FUNCTION(init)(ptr, n));
    }

    LIST_TYPE(LIST_TYPE &&other) noexcept {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            list = other.list;
        }
        other.ptr = nullptr;
    }

    LIST_TYPE(const LIST_TYPE &other) = delete;

    LIST_TYPE & operator = (const LIST_TYPE &other) = delete;

    LIST_TYPE & operator = (LIST_TYPE &&other) && noexcept {
        if (! is_alias())
            FUNCTION(destroy)(ptr);
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            list = other.list;
            ptr = &list;
        }
        other.ptr = nullptr;
        return *this;
    }

    ~LIST_TYPE() {
        if (! is_alias())
            FUNCTION(destroy)(ptr);
    }

    operator TYPE *() { return ptr; }
    operator const TYPE *() const { return ptr; }

    size_type size() const { return ptr->end - ptr->stor_begin; }
    constexpr size_type max_size() const { return IGRAPH_INTEGER_MAX; }
    size_type capacity() const { return ptr->stor_end - ptr->stor_begin; }

    bool empty() const { return ptr->end == ptr->stor_begin; }

    reference operator [] (size_type i) { return value_type(Alias(ptr->stor_begin[i])); }
    const_reference operator [] (size_type i) const { return value_type(Alias(ptr->stor_begin[i])); }

    void clear() { FUNCTION(clear)(ptr); }
    void resize(size_type size) { check(FUNCTION(resize)(ptr, size)); }
    void reserve(size_type capacity) { check(FUNCTION(reserve)(ptr, capacity)); }

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reference back() { return value_type(Alias(*FUNCTION(tail_ptr)(ptr))); }
    const_reference back() const { return value_type(Alias(*FUNCTION(tail_ptr)(ptr))); }

    // List takes ownership of t
    void set(igraph_integer_t pos, value_type &t) {
        FUNCTION(set)(ptr, pos, t);
        t.ptr = &ptr->stor_begin[pos]; // set as alias
    }

    // List takes ownership of t
    void set(igraph_integer_t pos, value_type &&t) {
        FUNCTION(set)(ptr, pos, t);
        t.ptr = nullptr;
    }

    // List takes ownership of t
    void set(igraph_integer_t pos, value_type::igraph_type *t) {
        FUNCTION(set)(ptr, pos, t);
    }

    // List takes ownership of t
    void push_back(value_type &t) {
        check(FUNCTION(push_back)(ptr, t));
        t.ptr = FUNCTION(tail_ptr)(ptr); // set as alias
    }

    // List takes ownership of t
    void push_back(value_type &&t) {
        check(FUNCTION(push_back)(ptr, t));
        t.ptr = nullptr;
    }

    // List takes ownership of t
    void push_back(value_type::igraph_type *t) {
        check(FUNCTION(push_back)(ptr, t));
    }

    void push_back_copy(const value_type::igraph_type *t) {
        check(FUNCTION(push_back_copy)(ptr, t));
    }

    reference push_back_new() {
        value_type::igraph_type *t;
        check(FUNCTION(push_back_new)(ptr, &t));
        return value_type(Alias(*t));
    }

    value_type pop_back() {
        return value_type(Capture(FUNCTION(pop_back)(ptr)));
    }

    friend void swap(LIST_TYPE &t1, LIST_TYPE &t2) noexcept {
        FUNCTION(swap)(t1.ptr, t2.ptr);
    }

    friend bool operator == (const LIST_TYPE &lhs, const LIST_TYPE &rhs) {
        if (lhs.ptr == rhs.ptr)
            return true;
        size_type n = lhs.size();
        if (rhs.size() != n)
            return false;
        for (size_type i = 0; i < n; ++i)
            if (lhs[i] != rhs[i])
                return false;
        return true;
    }

    friend bool operator != (const LIST_TYPE &lhs, const LIST_TYPE &rhs) {
        return ! (lhs == rhs);
    }

#ifdef GRAPH_LIST
    void set_directed(bool directed) {
        igraph_graph_list_set_directed(ptr, directed);
    }
#endif
};

template<typename ValueType, typename Reference>
class LIST_TYPE_TEMPL::base_iterator {
public:
    using value_type = ValueType;
    using difference_type = igraph_integer_t;
    using pointer = void;
    using reference = Reference;
    using iterator_category = std::random_access_iterator_tag;

private:
    typename value_type::igraph_type *p;

    friend class LIST_TYPE;
    base_iterator(typename value_type::igraph_type *p_) : p(p_) { }

public:

    base_iterator() = default;
    base_iterator(base_iterator &&) = default;
    base_iterator & operator = (const base_iterator &) = default;
    base_iterator & operator = (base_iterator &&) = default;

    // Make iterator convertible to const_iterator
    base_iterator(const base_iterator<typename std::remove_const<ValueType>::type,
                                      typename std::remove_const<Reference>::type> &it) :
        p(it.p) { }

    reference operator * () const { return value_type(Alias(*p)); }
    reference operator [] (difference_type i) const { return value_type(Alias(p[i])); }

    base_iterator & operator ++ () { ++p; return *this; }
    base_iterator operator ++ (int) { ++p; return *this; }
    base_iterator & operator -- () { --p; return *this; }
    base_iterator operator -- (int) { --p; return *this; }

    base_iterator & operator += (difference_type n) { p += n; return *this; }
    base_iterator & operator -= (difference_type n) { p -= n; return *this; }

    friend bool operator == (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.p == rhs.p;
    }

    friend bool operator != (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.p != rhs.p;
    }

    friend bool operator < (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.p < rhs.p;
    }

    friend bool operator > (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.p > rhs.p;
    }

    friend bool operator <= (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.p <= rhs.p;
    }

    friend bool operator >= (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.p >= rhs.p;
    }

    friend base_iterator operator + (const base_iterator &it, difference_type n) {
        return it.p + n;
    }

    friend base_iterator operator + (difference_type n, const base_iterator &it) {
        return it.p + n;
    }

    friend base_iterator operator - (const base_iterator &it, difference_type n) {
        return it.p - n;
    }

    friend difference_type operator - (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.p - rhs.p;
    }

    friend void iter_swap(base_iterator it1, base_iterator it2) {
        typename value_type::igraph_type tmp = *it1.p;
        *it1.p = *it2.p;
        *it2.p = tmp;
    }
};

LIST_TYPE_TEMPL::iterator LIST_TYPE_TEMPL::begin() { return ptr->stor_begin; }
LIST_TYPE_TEMPL::iterator LIST_TYPE_TEMPL::end() { return ptr->end; }

LIST_TYPE_TEMPL::const_iterator LIST_TYPE_TEMPL::begin() const { return ptr->stor_begin; }
LIST_TYPE_TEMPL::const_iterator LIST_TYPE_TEMPL::end() const { return ptr->end; }

LIST_TYPE_TEMPL::const_iterator LIST_TYPE_TEMPL::cbegin() const { return ptr->stor_begin; }
LIST_TYPE_TEMPL::const_iterator LIST_TYPE_TEMPL::cend() const { return ptr->end; }

#include <igraph_pmt_off.h>

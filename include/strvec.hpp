
class StrVec {
    using igraph_type = igraph_strvector_t;

    igraph_type vec;
    igraph_type *ptr = &vec;

    bool is_alias() const { return ptr != &vec; }

public:
    using value_type = const char *;
    using difference_type = igraph_integer_t;
    using size_type = igraph_integer_t;

    class reference {
        friend class StrVec;
        StrVec::igraph_type *ptr;
        StrVec::size_type index;

        reference(StrVec::igraph_type *ptr_,StrVec::size_type index_) : ptr(ptr_), index(index_) { }

    public:

        operator StrVec::value_type () const { return igraph_strvector_get(ptr, index); }

        // See https://stackoverflow.com/a/66931919/695132 for why the ref-qualifier is used.
        reference & operator = (const char *chr) && {
            check(igraph_strvector_set(ptr, index, chr));
            return *this;
        }
    };

    using const_reference = const reference;

    class iterator;
    using const_iterator = iterator;

    explicit StrVec(CaptureType<igraph_type> v) : vec(v.obj) { }
    explicit StrVec(AliasType<igraph_type> v) : ptr(&v.obj) { }

    explicit StrVec(size_type n = 0) {
        check(igraph_strvector_init(ptr, n));
    }

    StrVec(StrVec &&other) noexcept {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            vec = other.vec;
        }
        other.ptr = nullptr;
    }

    StrVec(const StrVec &other) {
        check(igraph_strvector_init_copy(ptr, other.ptr));
    }

    StrVec(const igraph_type *v) {
        check(igraph_strvector_init_copy(ptr, v));
    }

    StrVec(std::initializer_list<const char *> list) {
        check(igraph_strvector_init(ptr, 0));
        check(igraph_strvector_reserve(ptr, list.size()));
        for (auto el : list) {
            check(igraph_strvector_push_back(ptr, el));
        }
    }

    StrVec & operator = (const StrVec &other) = delete; // TODO 1.0
    StrVec & operator = (StrVec &&other) noexcept = delete;

    ~StrVec() {
        if (! is_alias())
            igraph_strvector_destroy(ptr);
    }

    operator igraph_type *() { return ptr; }
    operator const igraph_type *() const { return ptr; }

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reference operator [] (size_type i) { return {ptr, i}; }
    const_reference operator [] (size_type i) const { return {ptr, i}; }

    void clear() { igraph_strvector_clear(ptr); }
    void resize(size_type size) { check(igraph_strvector_resize(ptr, size)); }
    void reserve(size_type capacity) { check(igraph_strvector_reserve(ptr, capacity)); }
    // void shrink_to_fit() { igraph_strvector_resize_min(ptr); } // TODO 1.0

    void push_back(value_type elem) { check(igraph_strvector_push_back(ptr, elem)); }
    void push_back(value_type elem, igraph_integer_t len) { check(igraph_strvector_push_back_len(ptr, elem, len)); }

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    // There is no igraph_strvector_pop_back(), so we skip it here as well.

    // TODO swap() 1.0
};

class StrVec::iterator {
public:
    using value_type = StrVec::value_type;
    using difference_type = StrVec::difference_type;
    using pointer = void;
    using reference = StrVec::reference;
    using iterator_category = std::random_access_iterator_tag;

    friend class StrVec;

private:
    igraph_strvector_t *ptr;
    igraph_integer_t index;

    iterator(igraph_strvector_t *ptr_, igraph_integer_t index_) : ptr(ptr_), index(index_) { }

public:

    iterator() = default;
    iterator(const iterator &) = default;
    iterator(iterator &&) = default;
    iterator & operator = (const iterator &) = default;
    iterator & operator = (iterator &&) = default;

    reference operator * () { return {ptr, index}; }
    reference operator * () const { return {ptr, index}; }
    reference operator [] (difference_type i) const { return {ptr, index}; }

    iterator & operator ++ () { ++index; return *this; }
    iterator operator ++ (int) { ++index; return *this; }
    iterator & operator -- () { --index; return *this; }
    iterator operator -- (int) { --index; return *this; }

    iterator & operator += (difference_type n) { index += n; return *this; }
    iterator & operator -= (difference_type n) { index -= n; return *this; }

    friend bool operator == (const iterator &lhs, const iterator &rhs) {
        return lhs.index == rhs.index;
    }

    friend bool operator != (const iterator &lhs, const iterator &rhs) {
        return ! (lhs == rhs);
    }

    friend bool operator < (const iterator &lhs, const iterator &rhs) {
        return lhs.index < rhs.index;
    }

    friend bool operator > (const iterator &lhs, const iterator &rhs) {
        return lhs.index > rhs.index;
    }

    friend bool operator <= (const iterator &lhs, const iterator &rhs) {
        return lhs.index <= rhs.index;
    }

    friend bool operator >= (const iterator &lhs, const iterator &rhs) {
        return lhs.index >= rhs.index;
    }

    friend iterator operator + (const iterator &it, difference_type n) {
        return iterator(it.ptr, it.index + n);
    }

    friend iterator operator + (difference_type n, const iterator &it) {
        return iterator(it.ptr, it.index + n);
    }

    friend iterator operator - (const iterator &it, difference_type n) {
        return iterator(it.ptr, it.index - n);
    }

    friend difference_type operator - (const iterator &lhs, const iterator &rhs) {
        return lhs.index - rhs.index;
    }

    // TODO iter_swap() 1.0
};

StrVec::iterator StrVec::begin() {
    return iterator(ptr, 0);
}

StrVec::iterator StrVec::end() {
    return iterator(ptr, ptr->end - ptr->stor_begin);
}

StrVec::const_iterator StrVec::begin() const {
    return iterator(ptr, 0);
}

StrVec::const_iterator StrVec::end() const {
    return iterator(ptr, ptr->end - ptr->stor_begin);
}

StrVec::const_iterator StrVec::cbegin() const {
    return begin();
}

StrVec::const_iterator StrVec::cend() const {
    return end();
}

StrVec::iterator StrVec::erase(const_iterator first, const_iterator last) {
    igraph_strvector_remove_section(ptr, first.index, last.index);
    return first;
}

StrVec::iterator StrVec::erase(const_iterator pos) {
    igraph_strvector_remove(ptr, pos.index);
    return pos;
}

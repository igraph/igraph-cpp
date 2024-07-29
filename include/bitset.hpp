
class Bitset {
    template<typename Reference> class base_iterator;

public:
    using igraph_type = igraph_bitset_t;

    using value_type = bool;
    using difference_type = igraph_integer_t;
    using size_type = igraph_integer_t;

    class reference;
    using const_reference = const reference;

    using iterator = base_iterator<reference>;
    using const_iterator = base_iterator<const_reference>;

private:
    igraph_type vec;
    igraph_type *ptr = &vec;

    bool is_alias() const { return ptr != &vec; }

    friend class BitsetList;

public:
    explicit Bitset(CaptureType<igraph_type> v) : vec(v.obj) { }
    explicit Bitset(AliasType<igraph_type> v) : ptr(&v.obj) { }

    explicit Bitset(size_type n = 0) {
        check(igraph_bitset_init(ptr, n));
    }

    Bitset(Bitset &&other) noexcept {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            vec = other.vec;
        }
        other.ptr = nullptr;
    }

    Bitset(const Bitset &other) {
        check(igraph_bitset_init_copy(ptr, other.ptr));
    }

    Bitset(const igraph_type *v) {
        check(igraph_bitset_init_copy(ptr, v));
    }

    Bitset(std::initializer_list<value_type> list);

    Bitset & operator = (const Bitset &other) = delete;
    Bitset & operator = (Bitset &&other) noexcept = delete;

    ~Bitset() {
        if (! is_alias())
            igraph_bitset_destroy(ptr);
    }

    operator igraph_type *() { return ptr; }
    operator const igraph_type *() const { return ptr; }

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reference operator [] (size_type i);
    const_reference operator [] (size_type i) const;

    reference back();
    const_reference back() const;

    size_type size() const { return igraph_bitset_size(ptr); }
    constexpr size_type max_size() const { return IGRAPH_INTEGER_MAX; }
    size_type capacity() const { return igraph_bitset_capacity(ptr); }

    bool empty() const { return size() == 0; }

    void resize(size_type size) { check(igraph_bitset_resize(ptr, size)); }
    void reserve(size_type capacity) { check(igraph_bitset_reserve(ptr, capacity)); }
};

class Bitset::reference {
    friend class Bitset;
    Bitset::igraph_type *ptr;
    Bitset::size_type index;

    reference(Bitset::igraph_type *ptr_, Bitset::size_type index_) : ptr(ptr_), index(index_) { }

public:

    operator Bitset::value_type () const {
        return IGRAPH_BIT_TEST(*ptr, index);
    }

    // See https://stackoverflow.com/a/66931919/695132 for why the ref-qualifier is used.
    reference & operator = (Bitset::value_type val) && {
        if (val)
            IGRAPH_BIT_SET(*ptr, index);
        else
            IGRAPH_BIT_CLEAR(*ptr, index);
        return *this;
    }
};

template<typename Reference>
class Bitset::base_iterator {
public:
    using value_type = Bitset::value_type;
    using difference_type = Bitset::difference_type;
    using pointer = void;
    using reference = Reference;
    using iterator_category = std::random_access_iterator_tag;

    friend class Bitset;

private:
    igraph_bitset_t *ptr;
    igraph_integer_t index;

    base_iterator(igraph_bitset_t *ptr_, igraph_integer_t index_) : ptr(ptr_), index(index_) { }

public:

    base_iterator() = default;

    // Make iterator convertible to const_iterator
    base_iterator(const base_iterator<typename std::remove_const<Reference>::type> &it) :
        ptr(it.ptr), index(it.index) { }

    base_iterator(base_iterator &&) = default;
    base_iterator & operator = (const base_iterator &) = default;
    base_iterator & operator = (base_iterator &&) = default;

    reference operator * () { return {ptr, index}; }
    reference operator * () const { return {ptr, index}; }
    reference operator [] (difference_type i) const { return {ptr, index + i}; }

    base_iterator & operator ++ () { ++index; return *this; }
    base_iterator operator ++ (int) { ++index; return *this; }
    base_iterator & operator -- () { --index; return *this; }
    base_iterator operator -- (int) { --index; return *this; }

    base_iterator & operator += (difference_type n) { index += n; return *this; }
    base_iterator & operator -= (difference_type n) { index -= n; return *this; }

    friend bool operator == (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.index == rhs.index;
    }

    friend bool operator != (const base_iterator &lhs, const base_iterator &rhs) {
        return ! (lhs == rhs);
    }

    friend bool operator < (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.index < rhs.index;
    }

    friend bool operator > (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.index > rhs.index;
    }

    friend bool operator <= (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.index <= rhs.index;
    }

    friend bool operator >= (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.index >= rhs.index;
    }

    friend base_iterator operator + (const base_iterator &it, difference_type n) {
        return base_iterator(it.ptr, it.index + n);
    }

    friend base_iterator operator + (difference_type n, const base_iterator &it) {
        return base_iterator(it.ptr, it.index + n);
    }

    friend base_iterator operator - (const base_iterator &it, difference_type n) {
        return base_iterator(it.ptr, it.index - n);
    }

    friend difference_type operator - (const base_iterator &lhs, const base_iterator &rhs) {
        return lhs.index - rhs.index;
    }
};

Bitset::Bitset(std::initializer_list<value_type> list) : Bitset(list.size()) {
    size_t i = 0;
    for (auto el : list)
    (*this)[i++] = el;
}

Bitset::reference Bitset::operator [] (Bitset::size_type i) {
    return {ptr, i};
}

Bitset::const_reference Bitset::operator [] (Bitset::size_type i) const {
    return {ptr, i};
}

Bitset::iterator Bitset::begin() {
    return iterator(ptr, 0);
}

Bitset::iterator Bitset::end() {
    return iterator(ptr, size());
}

Bitset::const_iterator Bitset::begin() const {
    return const_iterator(ptr, 0);
}

Bitset::const_iterator Bitset::end() const {
    return const_iterator(ptr, size());
}

Bitset::const_iterator Bitset::cbegin() const {
    return begin();
}

Bitset::const_iterator Bitset::cend() const {
    return end();
}

Bitset::reference Bitset::back() {
    return *(end() - 1);
}

Bitset::const_reference Bitset::back() const {
    return *(end() - 1);
}


#include <igraph_pmt.hpp>

// This wrapper class can operate in two modes, and can switch between them dynamically
// as needed.
//  - It can own an igraph_vector_t, meaning that it is responsible for destroying it.
//    In this case ptr points to the internal vec object, which is initialized.
//  - It can alias an igraph_vector_t, essentially act as a reference to it. In this
//    case ptr is pointing to the external vector, and the destructor does not do anything.
// To create a Vec that aliases v, use Vec(Alias(v)). To take over the ownership of
// v's data, use Vec(Capture(v)). In the latter case, v must no longer be used directly.
template<> class Vec<OBASE> {
public:
    using igraph_type = TYPE(igraph_vector);

    using value_type = OBASE;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = value_type *;
    using const_iterator = const value_type *;
    using difference_type = igraph_integer_t;
    using size_type = igraph_integer_t;

private:
    igraph_type vec;
    igraph_type *ptr = &vec;

    bool is_alias() const { return ptr != &vec; }

    friend class VecList<OBASE>;

public:
    explicit Vec(CaptureType<igraph_type> v) : vec(v.obj) { }
    explicit Vec(AliasType<igraph_type> v) : ptr(&v.obj) { }

    explicit Vec(size_type n = 0) {
        check(FUNCTION(igraph_vector, init)(ptr, n));
    }

    Vec(Vec &&other) noexcept {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            vec = other.vec;
        }
        other.ptr = nullptr;
    }

    Vec(const Vec &other) {
        check(FUNCTION(igraph_vector, init_copy)(ptr, other.ptr));
    }

    Vec(const igraph_type *v) {
        check(FUNCTION(igraph_vector, init_copy)(ptr, v));
    }

    Vec(std::initializer_list<value_type> list) {
        check(FUNCTION(igraph_vector, init_array)(ptr, INVPTRCAST(list.begin()), list.size()));
    }

    Vec & operator = (const Vec &other) {
        check(FUNCTION(igraph_vector, update)(ptr, other.ptr));
        return *this;
    }

    Vec & operator = (Vec &&other) && noexcept {
        if (! is_alias())
            FUNCTION(igraph_vector, destroy)(ptr);
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            vec = other.vec;
            ptr = &vec;
        }
        other.ptr = nullptr;
        return *this;
    }

    ~Vec() {
        if (! is_alias())
            FUNCTION(igraph_vector, destroy)(ptr);
    }

    operator igraph_type *() { return ptr; }
    operator const igraph_type *() const { return ptr; }

    iterator begin() { return PTRCAST(ptr->stor_begin); }
    iterator end() { return PTRCAST(ptr->end); }

    const_iterator begin() const { return PTRCAST(ptr->stor_begin); }
    const_iterator end() const { return PTRCAST(ptr->end); }

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    value_type *data() { return begin(); }

    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }

    size_type size() const { return ptr->end - ptr->stor_begin; }
    constexpr size_type max_size() const { return IGRAPH_INTEGER_MAX; }
    size_type capacity() const { return ptr->stor_end - ptr->stor_begin; }

    bool empty() const { return ptr->end == ptr->stor_begin; }

    reference operator [] (size_type i) { return begin()[i]; }
    const_reference operator [] (size_type i) const { return begin()[i]; }

    void clear() { FUNCTION(igraph_vector, clear)(ptr); }
    void resize(size_type size) { check(FUNCTION(igraph_vector, resize)(ptr, size)); }
    void reserve(size_type capacity) { check(FUNCTION(igraph_vector, reserve)(ptr, capacity)); }
    void shrink_to_fit() { FUNCTION(igraph_vector, resize_min)(ptr); }

    void push_back(value_type elem) { check(FUNCTION(igraph_vector, push_back)(ptr, REFCAST(elem))); }
    value_type pop_back() { return REFCAST(FUNCTION(igraph_vector, pop_back)(ptr)); }

    iterator erase(const_iterator pos) {
        FUNCTION(igraph_vector, remove)(ptr, INVPTRCAST(pos) - ptr->stor_begin);
        return const_cast<iterator>(pos);
    }

    iterator erase(const_iterator first, const_iterator last) {
        FUNCTION(igraph_vector, remove_section)(ptr, INVPTRCAST(first) - ptr->stor_begin, INVPTRCAST(last) - ptr->stor_begin);
        return const_cast<iterator>(first);
    }

    friend void swap(Vec &v1, Vec &v2) noexcept {
        FUNCTION(igraph_vector, swap)(v1.ptr, v2.ptr);
    }

    // Necessary to allow some STL algorithms to work on VecList,
    // whose iterator dereferences to an aliasing Vec.
    friend void swap(Vec &&v1, Vec &&v2) noexcept {
        FUNCTION(igraph_vector, swap)(v1.ptr, v2.ptr);
    }

    friend bool operator == (const Vec &lhs, const Vec &rhs) {
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

    friend bool operator != (const Vec &lhs, const Vec &rhs) {
        return ! (lhs == rhs);
    }
};

#include <igraph_pmt_off.hpp>

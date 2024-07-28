
class Graph {
public:
    using igraph_type = igraph_t;

private:
    igraph_t graph;
    igraph_t *ptr = &graph;

    bool is_alias() const { return ptr != &graph; }

    friend class GraphList;

public:
    explicit Graph(CaptureType<igraph_t> g) : graph(g.obj) { }
    explicit Graph(AliasType<igraph_t> g) : ptr(&g.obj) { }

    explicit Graph(const igraph_t *g) {
        check(igraph_copy(ptr, g));
    }

    explicit Graph(igraph_integer_t n = 0, bool directed = false) {
        check(igraph_empty(ptr, n, directed));
    }

    explicit Graph(const igraph_vector_int_t *edges, igraph_integer_t n = 0, bool directed = false) {
        check(igraph_create(ptr, edges, n, directed));
    }

    Graph(const Graph &g) {
        check(igraph_copy(ptr, g.ptr));
    }

    Graph(Graph &&other) noexcept {
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            graph = other.graph;
        }
        other.ptr = nullptr;
    }

    Graph & operator = (const Graph &) = delete;

    Graph & operator = (Graph &&other) noexcept {
        if (! is_alias())
            igraph_destroy(ptr);
        if (other.is_alias()) {
            ptr = other.ptr;
        } else {
            graph = other.graph;
            ptr = &graph;
        }
        other.ptr = nullptr;
        return *this;
    }

    Graph & operator = (CaptureType<igraph_t> g) {
        if (! is_alias())
            igraph_destroy(ptr);
        graph = g.obj;
        ptr = &graph;
        return *this;
    }

    Graph & operator = (AliasType<igraph_t> g) {
        if (! is_alias())
            igraph_destroy(ptr);
        ptr = &g.obj;
        return *this;
    }

    ~Graph() {
        if (! is_alias())
            igraph_destroy(ptr);
    }

    operator igraph_t *() { return ptr; }
    operator const igraph_t *() const { return ptr; }

    friend void swap(Graph &g1, Graph &g2) noexcept {
        igraph_t tmp = *g1.ptr;
        *g1.ptr = *g2.ptr;
        *g2.ptr = tmp;
    }

    // Necessary to allow some STL algorithms to work on GraphList,
    // whose iterator dereferences to an aliasing Graph.
    friend void swap(Graph &&g1, Graph &&g2) noexcept {
        igraph_t tmp = *g1.ptr;
        *g1.ptr = *g2.ptr;
        *g2.ptr = tmp;
    }

    bool is_directed() const { return igraph_is_directed(ptr); }
    igraph_integer_t vcount() const { return igraph_vcount(ptr); }
    igraph_integer_t ecount() const { return igraph_ecount(ptr); }

    // Convenience access to basic properties (mostly those that are cached).

    bool is_connected(igraph_connectedness_t mode = IGRAPH_WEAK) const {
        igraph_bool_t res;
        check(igraph_is_connected(ptr, &res, mode));
        return res;
    }

    bool is_simple() const {
        igraph_bool_t res;
        check(igraph_is_simple(ptr, &res));
        return res;
    }

    bool has_loop() const {
        igraph_bool_t res;
        check(igraph_has_loop(ptr, &res));
        return res;
    }

    bool has_multiple() const {
        igraph_bool_t res;
        check(igraph_has_multiple(ptr, &res));
        return res;
    }

    bool has_mutual(bool loops = false) const {
        igraph_bool_t res;
        check(igraph_has_mutual(ptr, &res, loops));
        return res;
    }

    bool is_tree(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        check(igraph_is_tree(ptr, &res, nullptr, mode));
        return res;
    }

    bool is_forest(igraph_neimode_t mode = IGRAPH_ALL) const {
        igraph_bool_t res;
        check(igraph_is_forest(ptr, &res, nullptr, mode));
        return res;
    }

    bool is_dag() const {
        igraph_bool_t res;
        check(igraph_is_dag(ptr, &res));
        return res;
    }

    bool is_acyclic() const {
        igraph_bool_t res;
        check(igraph_is_acyclic(ptr, &res));
        return res;
    }

    void invalidate_cache() const {
        igraph_invalidate_cache(ptr);
    }

    // Note that the comparison is between labelled graphs, i.e. it does not test
    // for isomorphism. It also ignores attributes.
    friend bool operator == (const Graph &lhs, const Graph &rhs) {
        igraph_bool_t res;
        check(igraph_is_same_graph(lhs, rhs, &res));
        return res;
    }

    friend bool operator != (const Graph &lhs, const Graph &rhs) {
        return ! (lhs == rhs);
    }
};

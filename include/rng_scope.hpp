
class RNGScope {
    igraph_rng_t *previous;
    igraph_rng_t current;

    constexpr const static igraph_rng_type_t *default_type = &igraph_rngtype_pcg32;

public:
    explicit RNGScope(igraph_uint_t seed) : RNGScope(seed, default_type) {}

    RNGScope(igraph_uint_t seed, const igraph_rng_type_t *type) {
        check(igraph_rng_init(&current, type));

        igraph_error_t errcode = igraph_rng_seed(&current, seed);
        if (errcode != IGRAPH_SUCCESS) {
            igraph_rng_destroy(&current);
            throw Exception(errcode);
        }

        // TODO for igraph 1.0 where igraph_rng_set_default() returns the previous generator.
        previous = igraph_rng_default();
        igraph_rng_set_default(&current);
    }

    explicit RNGScope(const igraph_rng_type_t *type) : RNGScope(std::random_device{}(), type) { }

    RNGScope() : RNGScope(default_type) { }

public:
    // We shouldn't be copying or moving an RNGScope object, as this defeats the logic provided by scoping.
    RNGScope(const RNGScope &) = delete;
    RNGScope & operator = (const RNGScope&) = delete;
    RNGScope(RNGScope &&) = delete;
    RNGScope & operator = (RNGScope &&) = delete;

    ~RNGScope() {
        igraph_rng_set_default(previous);
        igraph_rng_destroy(&current);
    }
};

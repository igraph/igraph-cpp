class RNGScope {
public:
    RNGScope(igraph_uint_t seed) : RNGScope(seed, &igraph_rngtype_pcg32) {}

    RNGScope(igraph_uint_t seed, const igraph_rng_type_t* type) {
        igCheck(igraph_rng_init(&current, type));

        auto errcode = igraph_rng_seed(&current, seed);
        if (errcode != IGRAPH_SUCCESS) {
            igraph_rng_destroy(&current);
            throw igException(errcode);
        }

        previous = *(igraph_rng_default());
        igraph_rng_set_default(&current);
    }

    RNGScope(const igraph_rng_type_t* type) {
        igCheck(igraph_rng_init(&current, type));
        previous = *(igraph_rng_default());
        igraph_rng_set_default(&current);
    }

    RNGScope() : RNGScope(&igraph_rngtype_pcg32) {}

public:
    // We shouldn't be copying or moving an RNGscope object, as this defeats the logic provided by scoping.
    RNGScope(const RNGScope&) = delete;
    RNGScope& operator=(const RNGScope&) = delete;
    RNGScope(RNGScope&&) = delete;
    RNGScope& operator=(RNGScope&&) = delete;

    ~RNGScope() {
        igraph_rng_set_default(&previous);
        igraph_rng_destroy(&current);
    }

private:
    igraph_rng_t previous;
    igraph_rng_t current;
};

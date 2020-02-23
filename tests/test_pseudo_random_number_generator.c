/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("average of uniform distribution") {
    double sum = 0;
    uint64_t i;
    struct mliMT19937 prng = mliMT19937_init(0u);
    for (i = 0; i < 1000000; i++) {
        sum += mliMT19937_uniform(&prng);
    }
    CHECK_MARGIN(sum, 5e5, 2e2);
}

CASE("uniform population of histogram") {
    uint64_t i;
    struct mliMT19937 prng = mliMT19937_init(0u);
    double bin_edges[100];
    const uint64_t num_bin_edges = 100;
    const uint64_t num_bins = num_bin_edges - 1u;
    uint64_t bins[99];
    uint64_t overflow_bin, underflow_bin;
    overflow_bin = 0u;
    underflow_bin = 0u;
    mli_zeros_uint64_t(bins, num_bins);
    mli_linspace(0., 1., bin_edges, num_bin_edges);
    for (i = 0; i < 100000u; i++) {
        mli_histogram(
            bin_edges,
            num_bin_edges,
            &underflow_bin,
            bins,
            &overflow_bin,
            mliMT19937_uniform(&prng));
    }
    CHECK(underflow_bin == 0u);
    CHECK(overflow_bin == 0u);
    for (i = 0; i < num_bins; i++) {
        CHECK_MARGIN((double)bins[i], 1000., 100.);
    }
}

CASE("throwing Pi") {
    uint64_t num_throws = 1000000u;
    uint64_t i;
    uint64_t num_in_circle = 0u;
    double pi_estimate;
    struct mliMT19937 prng = mliMT19937_init(0u);
    for (i = 0; i < num_throws; i++) {
        const double x = mliMT19937_uniform(&prng);
        const double y = mliMT19937_uniform(&prng);
        const double r_sq = x*x + y*y;
        if (r_sq <= 1.) {
            num_in_circle += 1u;
        }
    }
    pi_estimate = 4*(double)num_in_circle/(double)num_throws;
    CHECK_MARGIN(pi_estimate, MLI_PI, 1e-3);
}

CASE("normal, Irwin Hall approximation") {
    struct mliMT19937 prng = mliMT19937_init(0u);

    const uint64_t num_scenarios = 6;
    const uint64_t num_throws = 100u*1000u;
    const double target_mean[6] = {1., -1., 0., 1e-5, 1e5, -1e3};
    const double taregt_std[6] = {.1, 0., 1e-3, 1e-8, 1e2, 1e0};
    double throws[1000*1000];

    uint64_t i, s;
    for (s = 0; s < num_scenarios; s++ ) {
        double mean = 0.;
        double std = 0.;
        for (i = 0; i < num_throws; i++) {
            throws[i] = mliMT19937_normal_Irwin_Hall_approximation(
                &prng,
                target_mean[s],
                taregt_std[s]);
        }
        mean = mli_mean(throws, num_throws);
        std = mli_std(throws, num_throws, mean);

        CHECK_MARGIN(mean, target_mean[s], 1e3+fabs(target_mean[s]*1e-2));
        CHECK_MARGIN(std, taregt_std[s], 1e-3+fabs(taregt_std[s]*1e-2));
    }
}
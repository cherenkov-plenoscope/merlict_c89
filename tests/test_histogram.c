/* Copyright 2019-2020 Sebastian Achim Mueller                                */

/* histogram 1D */
{
    const double bin_edges[3] = {1., 2., 3.};
    const uint64_t num_bin_edges = 3;
    uint64_t bins[2] = {0u, 0u};
    uint64_t overflow_bin, underflow_bin;
    overflow_bin = 0u;
    underflow_bin = 0u;
    mli_histogram(
        bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 2.5);
    CHECK(underflow_bin == 0u);
    CHECK(bins[0] == 0u);
    CHECK(bins[1] == 1u);
    CHECK(overflow_bin == 0u);

    mli_histogram(
        bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 0.5);
    CHECK(underflow_bin == 1u);
    CHECK(bins[0] == 0u);
    CHECK(bins[1] == 1u);
    CHECK(overflow_bin == 0u);

    mli_histogram(
        bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 3.5);
    CHECK(underflow_bin == 1u);
    CHECK(bins[0] == 0u);
    CHECK(bins[1] == 1u);
    CHECK(overflow_bin == 1u);

    mli_histogram(
        bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 1.0);
    CHECK(underflow_bin == 1u);
    CHECK(bins[0] == 1u);
    CHECK(bins[1] == 1u);
    CHECK(overflow_bin == 1u);

    mli_histogram(
        bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 1.9);
    CHECK(underflow_bin == 1u);
    CHECK(bins[0] == 2u);
    CHECK(bins[1] == 1u);
    CHECK(overflow_bin == 1u);

    mli_histogram(
        bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 2.0);
    CHECK(underflow_bin == 1u);
    CHECK(bins[0] == 2u);
    CHECK(bins[1] == 2u);
    CHECK(overflow_bin == 1u);

    mli_histogram(
        bin_edges, num_bin_edges, &underflow_bin, bins, &overflow_bin, 3.0);
    CHECK(underflow_bin == 1u);
    CHECK(bins[0] == 2u);
    CHECK(bins[1] == 2u);
    CHECK(overflow_bin == 2u);
}

/* mli_bin_edges_linspace */
{
    double bin_edges[3];
    const uint64_t num_bin_edges = 3;
    mli_linspace(0., 1., bin_edges, num_bin_edges);
    CHECK_MARGIN(bin_edges[0], 0., 1e-6);
    CHECK_MARGIN(bin_edges[1], 0.5, 1e-6);
    CHECK_MARGIN(bin_edges[2], 1., 1e-6);
}

/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "prng_testing.h"

CASE("average of uniform distribution")
{
        double sum = 0;
        uint64_t i;
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        for (i = 0; i < 1000000; i++) {
                sum += mli_Prng_uniform(&prng);
        }
        CHECK_MARGIN(sum, 5e5, 2e2);
}

CASE("uniform population of histogram")
{
        uint64_t i;
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        double bin_edges[100];
        const uint64_t num_bin_edges = 100;
        const uint64_t num_bins = num_bin_edges - 1u;
        uint64_t bins[99];
        uint64_t overflow_bin, underflow_bin;
        overflow_bin = 0u;
        underflow_bin = 0u;
        MLI_MATH_ARRAY_SET(bins, 0u, num_bins);
        mli_math_linspace(0., 1., bin_edges, num_bin_edges);
        for (i = 0; i < 100000u; i++) {
                mli_math_histogram(
                        bin_edges,
                        num_bin_edges,
                        &underflow_bin,
                        bins,
                        &overflow_bin,
                        mli_Prng_uniform(&prng));
        }
        CHECK(underflow_bin == 0u);
        CHECK(overflow_bin == 0u);
        for (i = 0; i < num_bins; i++) {
                CHECK_MARGIN((double)bins[i], 1000., 100.);
        }
}

CASE("throwing Pi")
{
        uint64_t num_throws = 1000u * 1000u;
        uint64_t i;
        uint64_t num_in_circle = 0u;
        double pi_estimate;
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        for (i = 0; i < num_throws; i++) {
                const double x = mli_Prng_uniform(&prng);
                const double y = mli_Prng_uniform(&prng);
                const double r_sq = x * x + y * y;
                if (r_sq <= 1.) {
                        num_in_circle += 1u;
                }
        }
        pi_estimate = 4 * (double)num_in_circle / (double)num_throws;
        CHECK_MARGIN(pi_estimate, MLI_MATH_PI, 1e-3);
}

CASE("normal, Irwin Hall approximation")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);

        const uint64_t num_scenarios = 6;
        const uint64_t num_throws = 100u * 1000u;
        const double target_mean[6] = {1., -1., 0., 1e-5, 1e5, -1e3};
        const double taregt_std[6] = {.1, 0., 1e-3, 1e-8, 1e2, 1e0};
        double throws[1000 * 1000];

        uint64_t i, s;
        for (s = 0; s < num_scenarios; s++) {
                double mean = 0.;
                double std = 0.;
                for (i = 0; i < num_throws; i++) {
                        throws[i] = mli_Prng_normal_Irwin_Hall_approximation(
                                &prng, target_mean[s], taregt_std[s]);
                }
                mean = mli_math_mean(throws, num_throws);
                std = mli_math_std(throws, num_throws, mean);

                CHECK_MARGIN(
                        mean,
                        target_mean[s],
                        1e3 + fabs(target_mean[s] * 1e-2));
                CHECK_MARGIN(
                        std, taregt_std[s], 1e-3 + fabs(taregt_std[s] * 1e-2));
        }
}

CASE("uniform_0_to_1_stddev")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        const uint64_t num_samples = 42 * 1337;
        struct mtl_VectorDoubles samples = mtl_VectorDoubles_init();
        double mean, std;
        uint64_t i = 0;

        CHECK(mtl_VectorDoubles_malloc(&samples, num_samples));
        for (i = 0; i < num_samples; i++) {
                mtl_VectorDoubles_push_back(&samples, mli_Prng_uniform(&prng));
        }
        mean = mli_math_mean(samples.array, samples.size);
        std = mli_math_std(samples.array, samples.size, mean);

        CHECK_MARGIN(1.0 / sqrt(12.0), std, 1e-3);
        mtl_VectorDoubles_free(&samples);
}

CASE("draw_from_poisson_distribution")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        double sum = 0.0;
        const double rate = 1e6;
        uint64_t i = 0;
        for (i = 0; i < rate; i++)
                sum += mli_Prng_expovariate(&prng, rate);
        CHECK_MARGIN(1.0, sum, 1e-3);
}

CASE("Generator")
{
        uint32_t i;
        struct mli_Prng prng = mli_Prng_init_MT19937(42);
        struct mli_prng_MT19937 mt_prng = mli_prng_MT19937_init(42);
        struct mli_prng_PCG32 pcg_prng = mli_prng_PCG32_init(42);

        for (i = 0; i < 10; i++) {
                CHECK(mli_Prng_generate_uint32(&prng) ==
                      mli_prng_MT19937_generate_uint32(&mt_prng));
        }

        mli_Prng_reinit(&prng, 1337);
        mli_prng_MT19937_reinit(&mt_prng, 1337);

        for (i = 0; i < 10; i++) {
                CHECK(mli_Prng_generate_uint32(&prng) ==
                      mli_prng_MT19937_generate_uint32(&mt_prng));
        }

        prng = mli_Prng_init_PCG32(42);

        for (i = 0; i < 10; i++) {
                CHECK(mli_Prng_generate_uint32(&prng) ==
                      mli_prng_PCG32_generate_uint32(&pcg_prng));
        }

        mli_Prng_reinit(&prng, 1337);
        mli_prng_PCG32_reinit(&pcg_prng, 1337);

        for (i = 0; i < 10; i++) {
                CHECK(mli_Prng_generate_uint32(&prng) ==
                      mli_prng_PCG32_generate_uint32(&pcg_prng));
        }
}

/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("average of uniform distribution")
{
        double sum = 0;
        uint64_t i;
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        for (i = 0; i < 1000000; i++) {
                sum += mli_random_uniform(&prng);
        }
        CHECK_MARGIN(sum, 5e5, 2e2);
}

CASE("uniform population of histogram")
{
        uint64_t i;
        struct mliPrng prng = mliPrng_init_MT19937(0u);
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
                        mli_random_uniform(&prng));
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
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        for (i = 0; i < num_throws; i++) {
                const double x = mli_random_uniform(&prng);
                const double y = mli_random_uniform(&prng);
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
        struct mliPrng prng = mliPrng_init_MT19937(0u);

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
                        throws[i] = mli_random_normal_Irwin_Hall_approximation(
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
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        const uint64_t num_samples = 42 * 1337;
        struct mliDynDouble samples = mliDynDouble_init();
        double mean, std;
        uint64_t i = 0;

        CHECK(mliDynDouble_malloc(&samples, num_samples));
        for (i = 0; i < num_samples; i++) {
                mliDynDouble_push_back(&samples, mli_random_uniform(&prng));
        }
        mean = mli_math_mean(samples.array, samples.size);
        std = mli_math_std(samples.array, samples.size, mean);

        CHECK_MARGIN(1.0 / sqrt(12.0), std, 1e-3);
        mliDynDouble_free(&samples);
}

CASE("draw_from_poisson_distribution")
{
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        double sum = 0.0;
        const double rate = 1e6;
        uint64_t i = 0;
        for (i = 0; i < rate; i++)
                sum += mli_random_expovariate(&prng, rate);
        CHECK_MARGIN(1.0, sum, 1e-3);
}

CASE("full_sphere")
{
        const uint64_t n = 1000 * 1000;
        uint64_t i = 0;
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        const struct mliRandomUniformRange azimuth =
                mliRandomUniformRange_set(0.0, 2.0 * MLI_MATH_PI);
        const struct mliRandomZenithRange zenith =
                mliRandomZenithRange_set(0.0, MLI_MATH_PI);
        struct mliVec mean_position = mliVec_init(0., 0., 0.);
        CHECK_MARGIN(zenith.z_min, 1.0, 1e-6);
        CHECK_MARGIN(zenith.z_range, -1.0, 1e-6);

        for (i = 0; i < n; i++) {
                mean_position = mliVec_add(
                        mean_position,
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, &prng));
        }
        mean_position = mliVec_multiply(mean_position, 1.0 / (double)n);
        CHECK_MARGIN(mean_position.x, 0.0, 1e-3);
        CHECK_MARGIN(mean_position.y, 0.0, 1e-3);
        CHECK_MARGIN(mean_position.z, 0.0, 1e-3);
}

CASE("octo_sphere")
{
        const uint64_t n = 1000 * 1000;
        uint64_t i = 0;
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        const struct mliRandomUniformRange azimuth =
                mliRandomUniformRange_set(0.0, MLI_MATH_PI / 2.0);
        const struct mliRandomZenithRange zenith =
                mliRandomZenithRange_set(0., MLI_MATH_PI / 2.0);
        struct mliVec mean_position = mliVec_init(0., 0., 0.);
        CHECK_MARGIN(zenith.z_min, 1.0, 1e-6);
        CHECK_MARGIN(zenith.z_range, -0.5, 1e-6);
        for (i = 0; i < n; i++) {
                mean_position = mliVec_add(
                        mean_position,
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, &prng));
        }
        mean_position = mliVec_multiply(mean_position, 1.0 / (double)n);
        CHECK_MARGIN(mean_position.x, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.y, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.z, 0.5, 1e-3);
}

CASE("octo_sphere_minus_z")
{
        const uint64_t n = 1000 * 1000;
        uint64_t i = 0;
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        const struct mliRandomUniformRange azimuth =
                mliRandomUniformRange_set(0.0, MLI_MATH_PI / 2.0);
        const struct mliRandomZenithRange zenith =
                mliRandomZenithRange_set(MLI_MATH_PI / 2.0, MLI_MATH_PI);
        struct mliVec mean_position = mliVec_init(0., 0., 0.);
        CHECK_MARGIN(zenith.z_min, 0.5, 1e-6);
        CHECK_MARGIN(zenith.z_range, -0.5, 1e-6);
        for (i = 0; i < n; i++) {
                mean_position = mliVec_add(
                        mean_position,
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, &prng));
        }
        mean_position = mliVec_multiply(mean_position, 1.0 / (double)n);
        CHECK_MARGIN(mean_position.x, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.y, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.z, -0.5, 1e-3);
}

CASE("position_on_disc")
{
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        uint64_t n_points = 1e6;
        double disc_radius = 1.337;
        double evaluation_disc_radius = disc_radius / 5.0;
        struct mliDynVec points = mliDynVec_init();
        struct mliDynDouble counts_in_evaluation_bins = mliDynDouble_init();
        uint64_t i = 0;
        struct mliVec mean;
        double r, phi;
        double mean_count, std_count;

        CHECK(mliDynVec_malloc(&points, n_points));
        for (i = 0; i < n_points; i++)
                CHECK(mliDynVec_push_back(
                        &points,
                        mli_random_position_on_disc(disc_radius, &prng)));

        /* mean position */
        mean = mliVec_init(0., 0., 0.);
        for (i = 0; i < points.size; i++)
                mean = mliVec_add(mean, points.array[i]);
        mean = mliVec_multiply(mean, 1.0 / (double)points.size);

        CHECK_MARGIN(mean.x, 0.0, 1e-3);
        CHECK_MARGIN(mean.y, 0.0, 1e-3);
        CHECK_MARGIN(mean.z, 0.0, 1e-3);
        /* distibution is evenly spread */

        mliDynDouble_malloc(&counts_in_evaluation_bins, 0);
        for (r = evaluation_disc_radius;
             r < disc_radius - evaluation_disc_radius;
             r = r + evaluation_disc_radius) {
                for (phi = 0; phi < 2.0 * MLI_MATH_PI;
                     phi = phi + MLI_MATH_PI / 3.0) {
                        double counts_in_evaluation_bin = 0.0;
                        struct mliVec eval_disc_pos =
                                mliVec_init(r * cos(phi), r * sin(phi), 0.0);
                        for (i = 0; i < points.size; i++) {
                                if (mliVec_norm_between(
                                            eval_disc_pos, points.array[i]) <=
                                    evaluation_disc_radius) {
                                        counts_in_evaluation_bin++;
                                }
                        }
                        CHECK(mliDynDouble_push_back(
                                &counts_in_evaluation_bins,
                                counts_in_evaluation_bin));
                }
        }

        /* Expect num. counts in evaluation-bins to be similar */
        mean_count = mli_math_mean(
                counts_in_evaluation_bins.array,
                counts_in_evaluation_bins.size);
        std_count = mli_math_std(
                counts_in_evaluation_bins.array,
                counts_in_evaluation_bins.size,
                mean_count);
        CHECK(std_count / mean_count < 1e-2);
        mliDynVec_free(&points);
}

CASE("Generator")
{
        uint32_t i;
        struct mliPrng prng = mliPrng_init_MT19937(42);
        struct mliMT19937 mt_prng = mliMT19937_init(42);
        struct mliPCG32 pcg_prng = mliPCG32_init(42);

        for (i = 0; i < 10; i++) {
                CHECK(mliPrng_generate_uint32(&prng) ==
                      mliMT19937_generate_uint32(&mt_prng));
        }

        mliPrng_reinit(&prng, 1337);
        mliMT19937_reinit(&mt_prng, 1337);

        for (i = 0; i < 10; i++) {
                CHECK(mliPrng_generate_uint32(&prng) ==
                      mliMT19937_generate_uint32(&mt_prng));
        }

        prng = mliPrng_init_PCG32(42);

        for (i = 0; i < 10; i++) {
                CHECK(mliPrng_generate_uint32(&prng) ==
                      mliPCG32_generate_uint32(&pcg_prng));
        }

        mliPrng_reinit(&prng, 1337);
        mliPCG32_reinit(&pcg_prng, 1337);

        for (i = 0; i < 10; i++) {
                CHECK(mliPrng_generate_uint32(&prng) ==
                      mliPCG32_generate_uint32(&pcg_prng));
        }
}

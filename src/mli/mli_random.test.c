/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("full_sphere")
{
        const uint64_t n = 1000 * 1000;
        uint64_t i = 0;
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        const struct mli_prng_UniformRange azimuth =
                mli_prng_UniformRange_set(0.0, 2.0 * MLI_MATH_PI);
        const struct mli_prng_ZenithRange zenith =
                mli_prng_ZenithRange_set(0.0, MLI_MATH_PI);
        struct mli_Vec mean_position = mli_Vec_init(0., 0., 0.);
        CHECK_MARGIN(zenith.z_min, 1.0, 1e-6);
        CHECK_MARGIN(zenith.z_range, -1.0, 1e-6);

        for (i = 0; i < n; i++) {
                mean_position = mli_Vec_add(
                        mean_position,
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, &prng));
        }
        mean_position = mli_Vec_multiply(mean_position, 1.0 / (double)n);
        CHECK_MARGIN(mean_position.x, 0.0, 1e-3);
        CHECK_MARGIN(mean_position.y, 0.0, 1e-3);
        CHECK_MARGIN(mean_position.z, 0.0, 1e-3);
}

CASE("octo_sphere")
{
        const uint64_t n = 1000 * 1000;
        uint64_t i = 0;
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        const struct mli_prng_UniformRange azimuth =
                mli_prng_UniformRange_set(0.0, MLI_MATH_PI / 2.0);
        const struct mli_prng_ZenithRange zenith =
                mli_prng_ZenithRange_set(0., MLI_MATH_PI / 2.0);
        struct mli_Vec mean_position = mli_Vec_init(0., 0., 0.);
        CHECK_MARGIN(zenith.z_min, 1.0, 1e-6);
        CHECK_MARGIN(zenith.z_range, -0.5, 1e-6);
        for (i = 0; i < n; i++) {
                mean_position = mli_Vec_add(
                        mean_position,
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, &prng));
        }
        mean_position = mli_Vec_multiply(mean_position, 1.0 / (double)n);
        CHECK_MARGIN(mean_position.x, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.y, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.z, 0.5, 1e-3);
}

CASE("octo_sphere_minus_z")
{
        const uint64_t n = 1000 * 1000;
        uint64_t i = 0;
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        const struct mli_prng_UniformRange azimuth =
                mli_prng_UniformRange_set(0.0, MLI_MATH_PI / 2.0);
        const struct mli_prng_ZenithRange zenith =
                mli_prng_ZenithRange_set(MLI_MATH_PI / 2.0, MLI_MATH_PI);
        struct mli_Vec mean_position = mli_Vec_init(0., 0., 0.);
        CHECK_MARGIN(zenith.z_min, 0.5, 1e-6);
        CHECK_MARGIN(zenith.z_range, -0.5, 1e-6);
        for (i = 0; i < n; i++) {
                mean_position = mli_Vec_add(
                        mean_position,
                        mli_random_draw_direction_in_zenith_azimuth_range(
                                zenith, azimuth, &prng));
        }
        mean_position = mli_Vec_multiply(mean_position, 1.0 / (double)n);
        CHECK_MARGIN(mean_position.x, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.y, 0.5, 1e-3);
        CHECK_MARGIN(mean_position.z, -0.5, 1e-3);
}

CASE("position_on_disc")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        uint64_t n_points = 1e6;
        double disc_radius = 1.337;
        double evaluation_disc_radius = disc_radius / 5.0;
        struct mliDynVec points = mliDynVec_init();
        struct mli_DoubleVector counts_in_evaluation_bins =
                mli_DoubleVector_init();
        uint64_t i = 0;
        struct mli_Vec mean;
        double r, phi;
        double mean_count, std_count;

        CHECK(mliDynVec_malloc(&points, n_points));
        for (i = 0; i < n_points; i++)
                CHECK(mliDynVec_push_back(
                        &points,
                        mli_random_position_on_disc(disc_radius, &prng)));

        /* mean position */
        mean = mli_Vec_init(0., 0., 0.);
        for (i = 0; i < points.size; i++)
                mean = mli_Vec_add(mean, points.array[i]);
        mean = mli_Vec_multiply(mean, 1.0 / (double)points.size);

        CHECK_MARGIN(mean.x, 0.0, 1e-3);
        CHECK_MARGIN(mean.y, 0.0, 1e-3);
        CHECK_MARGIN(mean.z, 0.0, 1e-3);
        /* distibution is evenly spread */

        mli_DoubleVector_malloc(&counts_in_evaluation_bins, 0);
        for (r = evaluation_disc_radius;
             r < disc_radius - evaluation_disc_radius;
             r = r + evaluation_disc_radius) {
                for (phi = 0; phi < 2.0 * MLI_MATH_PI;
                     phi = phi + MLI_MATH_PI / 3.0) {
                        double counts_in_evaluation_bin = 0.0;
                        struct mli_Vec eval_disc_pos =
                                mli_Vec_init(r * cos(phi), r * sin(phi), 0.0);
                        for (i = 0; i < points.size; i++) {
                                if (mli_Vec_norm_between(
                                            eval_disc_pos, points.array[i]) <=
                                    evaluation_disc_radius) {
                                        counts_in_evaluation_bin++;
                                }
                        }
                        CHECK(mli_DoubleVector_push_back(
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

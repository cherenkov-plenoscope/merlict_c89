/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("lambertian cosine law, populate histogram, check cosine law")
{
        uint64_t n;
        const uint64_t NUM_TRAILS = 1000 * 100;
        struct mliVec unit_z = mliVec_init(0, 0, 1);
        struct mtl_Prng prng = mtl_Prng_init_MT19937(0);

        double hist_bin_edges[25 + 1];
        const uint64_t hist_num_bin_edges = 25 + 1;
        const uint64_t hist_num_bins = hist_num_bin_edges - 1u;
        uint64_t hist_bins[25];
        double hist_bins_norm[25];
        uint64_t hist_overflow_bin = 0u;
        uint64_t hist_underflow_bin = 0u;
        MLI_MATH_ARRAY_SET(hist_bins, 0, hist_num_bins);
        mli_math_linspace(
                0., MLI_MATH_PI / 2.0, hist_bin_edges, hist_num_bin_edges);

        for (n = 0; n < NUM_TRAILS; n++) {
                double theta;
                struct mliVec lambertian_reflex;
                lambertian_reflex = mli_draw_lambertian_direction_wrt_z(&prng);
                theta = mliVec_angle_between(unit_z, lambertian_reflex);
                mli_math_histogram(
                        hist_bin_edges,
                        hist_num_bin_edges,
                        &hist_underflow_bin,
                        hist_bins,
                        &hist_overflow_bin,
                        theta);
        }
        CHECK(hist_underflow_bin == 0);
        CHECK(hist_overflow_bin == 0);

        /* normalize histogram */
        for (n = 0; n < hist_num_bins; n++) {
                hist_bins_norm[n] = (double)hist_bins[n] / (double)hist_bins[0];
        }

        /* check with cosine */
        for (n = 0; n < hist_num_bins; n++) {
                double theta_bin_center =
                        0.5 * (hist_bin_edges[n] + hist_bin_edges[n + 1]);
                double cos_theta_expected = cos(theta_bin_center);
                double cos_theta_reconstructed = hist_bins_norm[n];
                CHECK_MARGIN(cos_theta_reconstructed, cos_theta_expected, 0.03);
        }
}

CASE("lambertian cosine law, relative to surface normal")
{
        uint64_t n, m, o;
        const uint64_t NUM_NORMALS = 25;
        const uint64_t NUM_TRAILS = 5000;
        double theta;
        struct mliVec surface_normal;
        struct mtl_Prng prng = mtl_Prng_init_MT19937(1);

        double hist_bin_edges[10 + 1];
        const uint64_t hist_num_bin_edges = 10 + 1;
        const uint64_t hist_num_bins = hist_num_bin_edges - 1u;
        uint64_t hist_bins[10];
        uint64_t hist_overflow_bin = 0u;
        uint64_t hist_underflow_bin = 0u;
        MLI_MATH_ARRAY_SET(hist_bins, 0, hist_num_bins);
        mli_math_linspace(
                0., MLI_MATH_PI / 2.0, hist_bin_edges, hist_num_bin_edges);

        for (n = 0; n < NUM_NORMALS; n++) {
                surface_normal = mliVec_init(
                        mtl_Prng_uniform(&prng) - .5,
                        mtl_Prng_uniform(&prng) - .5,
                        mtl_Prng_uniform(&prng) - .5);
                surface_normal = mliVec_normalized(surface_normal);

                MLI_MATH_ARRAY_SET(hist_bins, 0, hist_num_bins);
                for (m = 0; m < NUM_TRAILS; m++) {
                        struct mliVec lambertian_reflex =
                                mli_draw_lambertian_direction_wrt_surface_normal(
                                        &prng, surface_normal);
                        theta = mliVec_angle_between(
                                surface_normal, lambertian_reflex);
                        mli_math_histogram(
                                hist_bin_edges,
                                hist_num_bin_edges,
                                &hist_underflow_bin,
                                hist_bins,
                                &hist_overflow_bin,
                                theta);
                }
                CHECK(hist_underflow_bin == 0);
                CHECK(hist_overflow_bin == 0);

                /* check cosine distribution */
                for (o = 0; o < hist_num_bins; o++) {
                        double theta_bin_center = 0.5 * (hist_bin_edges[o] +
                                                         hist_bin_edges[o + 1]);
                        double cos_theta_expected = cos(theta_bin_center);
                        double cos_theta_reconstructed =
                                ((double)hist_bins[o] / (double)hist_bins[0]);
                        CHECK_MARGIN(
                                cos_theta_reconstructed,
                                cos_theta_expected,
                                0.15);
                }
        }
}

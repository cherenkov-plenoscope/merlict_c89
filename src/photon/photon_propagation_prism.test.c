/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *   /\ z
 *   |                              prism
 *   |                          /\
 *   |                       __/..\__
 *   - 0.0               __-- /    \ --__
 *   |               __--    /______\    --__       screen
 *   |            _--                        --__  |
 *   - a.o.a.  (x)                               --| absorption
 *   |        source                               |
 */

CASE("refraction_in_prism")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        struct mli_Scenery scenery = mli_Scenery_init();
        struct mli_PhotonInteractionVector photon_history =
                mli_PhotonInteractionVector_init();
        struct mli_prng_UniformRange wavelength_range;
        const uint64_t NUM_PHOTONS = 10 * 1000;
        const uint64_t MAX_INTERACTIONS = 16;
        uint64_t i;
        const double angle_of_attack = 0.385;
        double count_passing_prism = 0.0;
        double fraction_passing_prism = 0.0;

        double screen_bin_edges[33];
        double spectrum_colors[32] = {0.0};
        double spectrum_intensity[32] = {0.0};

        uint64_t screen_bin_idx = 0;

        mli_math_linspace(0.375, 0.44, screen_bin_edges, 33);

        CHECK(mli_Scenery__from_path_cstr(
                &scenery,
                "data/"
                "sceneries/"
                "optics_prism.tar"));

        /* visible light */
        wavelength_range.start = 380e-9;
        wavelength_range.range = 780e-9 - wavelength_range.start;

        for (i = 0; i < NUM_PHOTONS; i++) {
                struct mli_Photon photon;
                struct mli_PhotonInteraction final_intersection;
                uint64_t final_robj_id;

                photon.ray = mli_Ray_set(
                        mli_Vec_init(0.0, -1.0, -angle_of_attack),
                        mli_Vec_init(0.0, 1.0, angle_of_attack));
                photon.wavelength =
                        mli_prng_draw_uniform(wavelength_range, &prng);
                photon.id = i;

                CHECK(mli_PhotonInteractionVector_malloc(
                        &photon_history, MAX_INTERACTIONS));

                CHECK(mli_propagate_photon(
                        &scenery,
                        &photon_history,
                        &photon,
                        &prng,
                        MAX_INTERACTIONS));

                final_intersection =
                        photon_history.array[photon_history.size - 1];
                final_robj_id =
                        scenery.geometry.robject_ids[final_intersection
                                                             .geometry_id.robj];

                if (final_robj_id == 2) {
                        count_passing_prism += 1.0;

                        screen_bin_idx = mli_math_upper_compare_double(
                                screen_bin_edges,
                                33,
                                -final_intersection.position.z);

                        spectrum_colors[screen_bin_idx] += photon.wavelength;
                        spectrum_intensity[screen_bin_idx] += 1.0;
                }
        }

        for (i = 0; i < 32; i++) {
                spectrum_colors[i] /= spectrum_intensity[i];
        }

        for (i = 1; i < 32; i++) {
                if (spectrum_intensity[i] > 10.0 &&
                    spectrum_intensity[i - 1] > 10.0) {
                        CHECK(spectrum_colors[i] < spectrum_colors[i - 1]);
                }
        }

        fraction_passing_prism = count_passing_prism / (double)NUM_PHOTONS;

        CHECK(fraction_passing_prism < 0.9);
        CHECK(fraction_passing_prism > 0.8);

        mli_Scenery_free(&scenery);
        mli_PhotonInteractionVector_free(&photon_history);
}

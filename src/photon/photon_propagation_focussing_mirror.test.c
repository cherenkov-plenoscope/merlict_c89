/* Copyright 2019 Sebastian Achim Mueller                                     */
#include <math.h>

CASE("focussing_a_parallel_beam")
{
#define NUM_PIXEL 127
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        struct mli_Scenery scenery = mli_Scenery_init();
        struct mli_IO f = mli_IO_init();
        struct mli_PhotonInteractionVector photon_history =
                mli_PhotonInteractionVector_init();
        struct mli_prng_UniformRange wavelength_range;
        const uint64_t NUM_PHOTONS = 10 * 1000;
        const uint64_t MAX_INTERACTIONS = 16;
        uint64_t i;
        const double mirror_radius = 0.51;
        double count_reaching_screen = 0.0;
        double fraction_reaching_screen = 0.0;
        struct mli_Image screen_img = mli_Image_init();
        double screen_bin_edges[NUM_PIXEL + 1];
        struct mli_Color max_color;

        CHECK(mli_Image_malloc(&screen_img, NUM_PIXEL, NUM_PIXEL));
        mli_Image_set_all(&screen_img, mli_Color_set(20.0, 0.0, 0.0));
        mli_math_linspace(-2e-3, 2e-3, screen_bin_edges, NUM_PIXEL + 1);
        wavelength_range = mli_prng_UniformRange_set(380e-9, 700e-9);

        CHECK(mli_Scenery__from_path_cstr(
                &scenery,
                "data/"
                "sceneries/"
                "optics_focussing_mirror.tar"));

        for (i = 0; i < NUM_PHOTONS; i++) {
                struct mli_Photon photon;
                struct mli_PhotonInteraction final_intersection;
                uint64_t final_robj_id;

                photon.ray = mli_Ray_set(
                        mli_Vec_add(
                                mli_Vec_random_position_on_disc(
                                        mirror_radius, &prng),
                                mli_Vec_init(0.0, 0.0, 10.0)),
                        mli_Vec_init(0.0, 0.0, -1.0));
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

                /*
                mli_PhotonInteractionVector_print(&photon_history, &scenery);
                */

                if (final_robj_id == 42) {
                        count_reaching_screen += 1.0;

                        mli_Image_histogram(
                                &screen_img,
                                screen_bin_edges,
                                screen_bin_edges,
                                final_intersection.position.x,
                                final_intersection.position.y,
                                mli_Color_set(0.0, 1.0, 1.0));
                }
        }
        fraction_reaching_screen = count_reaching_screen / (double)NUM_PHOTONS;

        CHECK(fraction_reaching_screen < 0.99);
        CHECK(fraction_reaching_screen > 0.95);

        mli_Image_power(&screen_img, mli_Color_set(0.3, 0.3, 0.3));

        max_color = mli_Image_max(&screen_img);
        mli_Image_multiply(
                &screen_img,
                mli_Color_set(1.0, 255.0 / max_color.g, 255.0 / max_color.b));

        CHECK(mli_IO_open_file_cstr(
                &f,
                "data/"
                "optics_focussing_mirror-psf.ppm.tmp",
                "w"));
        CHECK(mli_Image_to_io(&screen_img, &f));
        mli_IO_close(&f);

        mli_Scenery_free(&scenery);
        mli_PhotonInteractionVector_free(&photon_history);
        mli_Image_free(&screen_img);
}

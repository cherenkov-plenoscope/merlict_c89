/* Copyright 2019 Sebastian Achim Mueller                                     */
#include <math.h>

/*

 */

CASE("focussing_a_parallel_beam")
{
        #define NUM_PIXEL 127
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        struct mliScenery scenery = mliScenery_init();
        struct mliDynPhotonInteraction photon_history =
                mliDynPhotonInteraction_init();
        struct mliRandomUniformRange wavelength_range;
        const uint64_t NUM_PHOTONS = 10 * 1000;
        const uint64_t MAX_INTERACTIONS = 16;
        uint64_t i;
        const double mirror_radius = 0.51;
        double count_reaching_screen = 0.0;
        double fraction_reaching_screen = 0.0;
        struct mliImage screen_img = mliImage_init();
        double screen_bin_edges[NUM_PIXEL + 1];
        struct mliColor max_color;

        CHECK(mliImage_malloc(
                &screen_img,
                NUM_PIXEL,
                NUM_PIXEL));
        mliImage_set_all_pixel(&screen_img, mliColor_set(20.0, 0.0, 0.0));
        mli_linspace(-2e-3, 2e-3, screen_bin_edges, NUM_PIXEL + 1);
        wavelength_range = mliRandomUniformRange_set(380e-9, 700e-9);

        CHECK(mliScenery_malloc_from_tar(
                &scenery,
                "tests/"
                "resources/"
                "sceneries/"
                "optics_focussing_mirror.tar"));

        for (i = 0; i < NUM_PHOTONS; i++) {
                struct mliPhoton photon;
                struct mliPhotonInteraction final_intersection;
                uint64_t final_robj_id;

                photon.ray = mliRay_set(
                        mliVec_add(
                                mli_random_position_on_disc(
                                        mirror_radius,
                                        &prng),
                                mliVec_set(0.0, 0.0, 10.0)
                        ),
                        mliVec_set(0.0, 0.0, -1.0)
                );
                photon.wavelength = mli_random_draw_uniform(
                        wavelength_range,
                        &prng);
                photon.id = i;

                CHECK(mliDynPhotonInteraction_malloc(
                        &photon_history, MAX_INTERACTIONS));

                CHECK(mli_propagate_photon(
                        &scenery,
                        &photon_history,
                        &photon,
                        &prng,
                        MAX_INTERACTIONS));

                final_intersection = photon_history.arr[
                        photon_history.dyn.size - 1];
                final_robj_id = scenery.geometry.robject_ids[
                        final_intersection.geometry_id.robj];

                if (final_robj_id == 42) {
                        count_reaching_screen += 1.0;

                        mliImage_histogram(
                                &screen_img,
                                screen_bin_edges,
                                screen_bin_edges,
                                final_intersection.position.x,
                                final_intersection.position.y,
                                mliColor_set(0.0, 1.0, 1.0));
                }
        }
        fraction_reaching_screen = count_reaching_screen/(double)NUM_PHOTONS;
        CHECK(fraction_reaching_screen < 0.99);
        CHECK(fraction_reaching_screen > 0.95);

        for (i = 0u; i < screen_img.num_rows * screen_img.num_cols; i++) {
                screen_img.raw[i].g = pow(screen_img.raw[i].g, 0.3);
                screen_img.raw[i].b = pow(screen_img.raw[i].b, 0.3);
        }
        max_color = mliImage_max(&screen_img);
        mliImage_multiply(
                &screen_img,
                mliColor_set(
                        1.0,
                        255.0 / max_color.g,
                        255.0 / max_color.b));
        CHECK(mliImage_write_to_ppm(
                &screen_img,
                "tests/resources/optics_focussing_mirror-psf.ppm.tmp"));

        mliScenery_free(&scenery);
        mliDynPhotonInteraction_free(&photon_history);
        mliImage_free(&screen_img);
}

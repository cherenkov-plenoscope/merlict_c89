/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with:
 *  gcc merlict_c89/mli_camera_server_app.c -o camera_server -lm
 */

#include <stdio.h>
#include "mli.h"
#include "mli.c"


int mliRenderConfig_from_channel(
        struct mliRenderConfig *control,
        FILE *fin,
        FILE *fout)
{
        struct mliStr jsonlstr = mliStr_init();
        struct mliJson json = mliJson_init();
        chk(mliStr_malloc(&jsonlstr));

        chk_msg(mliStr_add_line_from_file(&jsonlstr, fin, '\n'),
                "Can't read line from fin.");

        chk_msg(mliJson_malloc_from_c_str(&json, jsonlstr.c_str),
                "Can't parse line into json-tokens.");
        mliStr_free(&jsonlstr);

        chk_msg(mliRenderConfig_from_json(control, &json, 0),
                "Can't parse json-tokens into camera-control.");
        mliJson_free(&json);

        fprintf(fout, "{\"status\": 1}\n");
        return 1;
error:
        fprintf(fout, "{\"status\": 0}\n");
        return 0;
}


int main(int argc, char *argv[])
{
        struct mliScenery scenery = mliScenery_init();

        if (argc == 2) {
                if (mli_cstr_ends_with(argv[1], ".tar")) {
                        chk_msg(mliScenery_malloc_from_tar(&scenery, argv[1]),
                                "Can not read scenery from '.tar'.");
                } else if (mli_cstr_ends_with(argv[1], ".mli")) {
                        chk_msg(mliScenery_malloc_from_path(&scenery, argv[1]),
                                "Can not read scenery from '.mli'.");
                } else if (mli_cstr_ends_with(argv[1], ".obj")) {
                        chk_msg(mliScenery_malloc_minimal_from_wavefront(
                                        &scenery, argv[1]),
                                "Can not read scenery from '.obj'.");
                } else {
                        chk_bad("Scenery-format has to be either of "
                                "('.tar', '.mli', '.obj').");
                }
        } else {
                fprintf(stderr, "Usage: %s <scenery-path>\n", argv[0]);
                goto error;
        }

        setbuf(stdout, NULL);
        while (1) {
                struct mliPrng prng = mliPrng_init_PCG32(0);
                struct mliRenderConfig control = mliRenderConfig_init();
                struct mliImage image = mliImage_init();

                chk_msg(mliRenderConfig_from_channel(&control, stdin, stdout),
                        "Can't communicate camera-control.");

                mliPrng_reinit(&prng, control.random_seed);
                chk_msg(mliImage_malloc(
                        &image,
                        control.num_pixel_x,
                        control.num_pixel_y),
                        "Can't malloc image.");

                mliApertureCamera_render_image(
                        control.camera,
                        control.camera_to_root,
                        &scenery,
                        &image,
                        &control.tracer,
                        &prng);

                chk_msg(mliImage_write_ppm_to_file(&image, stdout),
                        "Can't write image to stdout.");

                mliImage_free(&image);
        }
        mliScenery_free(&scenery);
        return EXIT_SUCCESS;
error:
        return EXIT_FAILURE;
}

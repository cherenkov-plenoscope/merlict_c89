/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with:
 *  gcc merlict/mli_camera_server_app.c -o camera_server -lm
 */

#include <stdio.h>
#include "mli.h"
#include "mli.c"

int mliRenderConfig_from_channel(
        struct mliRenderConfig *control,
        FILE *fin,
        FILE *fout)
{
        struct mliIo jsonlstr = mliIo_init();
        struct mliJson json = mliJson_init();
        chk(mliIo_malloc(&jsonlstr));

        chk_msg(mliIo_add_line_from_file(&jsonlstr, fin, '\n'),
                "Can't read line from fin.");

        chk_msg(mliJson_malloc_from_cstr(&json, jsonlstr.cstr),
                "Can't parse line into json-tokens.");
        mliIo_free(&jsonlstr);

        chk_msg(mliRenderConfig_from_json(control, &json, 0),
                "Can't parse json-tokens into camera-control.");
        mliJson_free(&json);

        fprintf(fout, "{\"status\": 1}\n");
        return 1;
error:
        fprintf(fout, "{\"status\": 0}\n");
        return 0;
}

int fread_until_next_newline(
        FILE *fstream,
        const char newline,
        const uint64_t max_num_bytes)
{
        char c = '\0';
        uint64_t num_bytes = 0u;
        do {
                chk_fread(&c, sizeof(char), 1, fstream);
                num_bytes++;
                chk_msg(num_bytes < max_num_bytes, "Too many bytes.");
        } while (c != newline);

        return 1;
error:
        return 0;
}

int main(int argc, char *argv[])
{
        struct mliScenery scenery = mliScenery_init();
        setbuf(stdout, NULL);

        chk_msg(mliScenery_malloc_fread_tar(&scenery, stdin),
                "Can't malloc scenery from stdin tar.");

        chk_msg(fread_until_next_newline(stdin, '\n', 1000 * 1000),
                "Can't reach next newline.");

        while (1) {
                struct mliPrng prng = mliPrng_init_PCG32(0);
                struct mliRenderConfig control = mliRenderConfig_init();
                struct mliImage image = mliImage_init();

                chk_msg(mliRenderConfig_from_channel(&control, stdin, stdout),
                        "Can't communicate RenderConfig.");

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

                chk_msg(mliImage_fwrite(&image, stdout),
                        "Can't write image to stdout.");

                mliImage_free(&image);
        }
        mliScenery_free(&scenery);
        return EXIT_SUCCESS;
error:
        return EXIT_FAILURE;
}

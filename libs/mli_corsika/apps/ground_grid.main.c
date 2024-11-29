#include <stdio.h>
#include <stdlib.h>
#include "../src/mli_corsika_ray_voxel_overlap.h"
#include "../src/mli_corsika_CorsikaPhotonBunch.h"
#include "../src/mli_corsika_EventTape.h"
#include "../src/mli_corsika_Histogram2d.h"
#include "../../chk/src/chk.h"
#include "../../mli/src/mli_math.h"
#include "../../mli/src/mliIo.h"
#include "../../mli/src/mliTar.h"
#include "../../mli/src/mliTarIo.h"
#include "../../mli/src/mliStr_numbers.h"
#include "../../mtl/src/vec.h"
#include "../../mli/src/mli_ray_grid_traversal.h"

int mliAxisAlignedGrid_set_from_config(
        struct mliAxisAlignedGrid *grid,
        struct mliIo *text)
{
        struct mliVec lower;
        struct mliVec upper;
        struct mliIdx3 num_bins;
        struct mliStr line = mliStr_init();
        chk(mliStr_malloc(&line, 100));
        mliIo_rewind(text);
        /* X */
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_int64(&(num_bins.x), &line, 10));
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_double(&(lower.x), &line)); /* cm */
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_double(&(upper.x), &line)); /* cm */

        /* Y */
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_int64(&(num_bins.y), &line, 10));
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_double(&(lower.y), &line)); /* cm */
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_double(&(upper.y), &line)); /* cm */

        /* Z */
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_int64(&(num_bins.z), &line, 10));
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_double(&(lower.z), &line)); /* cm */
        chk(mli_readline(text, &line, '\n'));
        chk(mliStr_to_double(&(upper.z), &line)); /* cm */

        mliStr_free(&line);

        (*grid) = mliAxisAlignedGrid_set(mliAABB_set(lower, upper), num_bins);

        return 1;
chk_error:
        return 0;
}

#define HIST_TARGET_SIZE 10000

int main(int argc, char *argv[])
{
        struct mliTar arc = mliTar_init();
        struct mliTar out = mliTar_init();
        struct mliTarHeader arch = mliTarHeader_init();

        struct mliCorsikaHistogram2d hist = mliCorsikaHistogram2d_init();
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;

        float raw[8] = {0.0};
        struct mliCorsikaPhotonBunch bunch;
        struct mliRay ray;
        int i;

        chk(mliTar_read_begin(&arc, stdin));
        chk(mliTar_write_begin(&out, stdout));
        chk(mliCorsikaHistogram2d_malloc(&hist, HIST_TARGET_SIZE));

        while (mliTar_read_header(&arc, &arch)) {
                if (strcmp(arch.name, "init.txt") == 0) {
                        struct mliIo config_text = mliIo_init();

                        if (hist.dict.capacity > 10 * HIST_TARGET_SIZE) {
                                chk(mliCorsikaHistogram2d_malloc(
                                        &hist, HIST_TARGET_SIZE));
                        } else {
                                mliCorsikaHistogram2d_reset(&hist);
                        }
                        chk(mliIo_malloc(&config_text));
                        chk(mliTar_read_data_to_io(
                                &arc, &config_text, arch.size));
                        chk(mliAxisAlignedGrid_set_from_config(
                                &grid, &config_text));
                        mliIo_free(&config_text);
                } else if (strcmp(arch.name, "cer.x8.float32") == 0) {
                        const int num_bunches = arch.size / 32;
                        int num = 0;

                        for (i = 0; i < num_bunches; i++) {
                                chk_msg(mliTar_read_data(
                                                &arc, (void *)raw, sizeof(raw)),
                                        "Failed to read 'cer.x8.float32'.");
                                mliCorsikaPhotonBunch_set_from_raw(&bunch, raw);
                                ray.support.x = bunch.x_cm;
                                ray.support.y = bunch.y_cm;
                                ray.support.z = 0.0;
                                ray.direction.x =
                                        mli_corsika_ux_to_cx(bunch.ux);
                                ray.direction.y =
                                        mli_corsika_vy_to_cy(bunch.vy);
                                ray.direction.z =
                                        mli_corsika_restore_direction_z_component(
                                                ray.direction.x,
                                                ray.direction.y);

                                traversal = mliAxisAlignedGridTraversal_start(
                                        &grid, &ray);

                                while (traversal.valid) {
                                        int odd = 0;
                                        if (traversal.voxel.x < 0 ||
                                            traversal.voxel.x >=
                                                    grid.num_bins.x) {
                                                odd = 1;
                                        }
                                        if (traversal.voxel.y < 0 ||
                                            traversal.voxel.y >=
                                                    grid.num_bins.y) {
                                                odd = 1;
                                        }
                                        if (odd) {
                                                fprintf(stderr,
                                                        "-------------------"
                                                        "\n");
                                                mliRay_fprint(stderr, &ray);
                                                fprintf(stderr, "\n");
                                                mliAxisAlignedGridTraversal_fprint(
                                                        stderr, &traversal);
                                        }
                                        num += 1;
                                        chk(mliCorsikaHistogram2d_assign(
                                                &hist,
                                                traversal.voxel.x,
                                                traversal.voxel.y,
                                                bunch.weight_photons));
                                        mliAxisAlignedGridTraversal_next(
                                                &traversal);
                                }
                        }
                } else if (strcmp(arch.name, "export.txt") == 0) {
                        struct mliTarHeader outh = mliTarHeader_init();
                        struct mliDynCorsikaHistogram2dBin bins =
                                mliDynCorsikaHistogram2dBin_init();
                        chk_msg(arch.size == 0, "Expected zero size.");
                        chk(mliDynCorsikaHistogram2dBin_malloc(
                                &bins, mliCorsikaHistogram2d_len(&hist)));
                        chk(mliCorsikaHistogram2d_flatten(&hist, &bins));
                        strcpy(outh.name, "histogram.int32_int32_float64");
                        outh.size = bins.size *
                                    sizeof(struct mliCorsikaHistogram2dBin);
                        chk_msg(mliTar_write_header(&out, &outh),
                                "Failed to write tar header to stdout");
                        chk_msg(mliTar_write_data(
                                        &out,
                                        (const void *)&(bins.array[0]),
                                        outh.size),
                                "Failed to write histogram to tar in stdout.");
                        fflush(out.stream);
                } else {
                        chk_bad("Expected one of "
                                "['init.txt', 'cer.x8.float32', "
                                "'export.txt'].");
                }
        }

        mliCorsikaHistogram2d_free(&hist);
        mliTar_read_finalize(&arc);
        mliTar_write_finalize(&out);
        fflush(out.stream);

        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}

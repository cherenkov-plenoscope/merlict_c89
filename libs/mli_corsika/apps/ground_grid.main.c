#include <stdio.h>
#include <stdlib.h>
#include "../src/mli_corsika_ray_voxel_overlap.h"
#include "../src/mli_corsika_CorsikaPhotonBunch.h"
#include "../src/mli_corsika_EventTape.h"
#include "../src/mli_corsika_Histogram2d.h"
#include "../../mli/src/chk.h"
#include "../../mli/src/mli_math.h"
#include "../../mli/src/mliIo.h"
#include "../../mli/src/mliTar.h"
#include "../../mli/src/mliTarIo.h"
#include "../../mli/src/mliStr_numbers.h"
#include "../../mli/src/mliDynArray.h"
#include "../../mli/src/mli_ray_grid_traversal.h"


int mliAxisAlignedGrid_set_from_config(struct mliAxisAlignedGrid *grid, struct mliIo *text)
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
        struct mliTarHeader arch = mliTarHeader_init();

        struct mliCorsikaHistogram2d hist = mliCorsikaHistogram2d_init();
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;

        float raw[8] = {0.0};
        struct mliCorsikaPhotonBunch bunch;
        struct mliRay ray;
        int i;

        chk(mliTar_read_begin(&arc, stdin));
        chk(mliCorsikaHistogram2d_malloc(&hist, HIST_TARGET_SIZE));

        while (mliTar_read_header(&arc, &arch)) {
                /*fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);*/

                if (strcmp(arch.name, "init.txt") == 0) {
                        struct mliIo config_text = mliIo_init();

                        /*fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);*/

                        if (hist.dict.capacity > 10 * HIST_TARGET_SIZE) {
                                chk(mliCorsikaHistogram2d_malloc(&hist, HIST_TARGET_SIZE));
                        } else {
                                mliCorsikaHistogram2d_reset(&hist);
                        }
                        chk(mliIo_malloc(&config_text));
                        chk(mliTar_read_data_to_io(&arc, &config_text, arch.size));
                        chk(mliAxisAlignedGrid_set_from_config(&grid, &config_text));
                        mliIo_free(&config_text);
                } else if (strcmp(arch.name, "cer.x8.float32") == 0) {
                        const int num_bunches = arch.size / 32;
                        int num = 0;

                        /*fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);*/

                        for (i = 0; i < num_bunches; i++) {
                                chk_msg(
                                        mliTar_read_data(&arc, (void *)raw, sizeof(raw)),
                                        "Failed to read 'cer.x8.float32'."
                                );
                                mliCorsikaPhotonBunch_set_from_raw(&bunch, raw);
                                ray.support.x = bunch.x_cm;
                                ray.support.y = bunch.y_cm;
                                ray.support.z = 0.0;
                                ray.direction.x = mli_corsika_ux_to_cx(bunch.ux);
                                ray.direction.y = mli_corsika_vy_to_cy(bunch.vy);
                                ray.direction.z =
                                        mli_corsika_restore_direction_z_component(
                                                ray.direction.x, ray.direction.y);

                                traversal =
                                        mliAxisAlignedGridTraversal_start(&grid, &ray);

                                while (traversal.valid) {
                                        num += 1;
                                        chk(mliCorsikaHistogram2d_assign(
                                                &hist,
                                                traversal.voxel.x,
                                                traversal.voxel.y,
                                                bunch.weight_photons));
                                        mliAxisAlignedGridTraversal_next(&traversal);
                                }
                        }
                        /*fprintf(stderr, "%s:%d [%d]\n", __FILE__, __LINE__, num);*/

                } else if (strcmp(arch.name, "export.txt") == 0) {
                        struct mliDynCorsikaHistogram2dBin bins = mliDynCorsikaHistogram2dBin_init();

                        /*fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);*/
                        chk_msg(arch.size == 0, "Expected zero size.")

                        chk(
                                mliDynCorsikaHistogram2dBin_malloc(
                                        &bins,
                                        mliCorsikaHistogram2d_len(&hist)
                                )
                        );
                        chk(mliCorsikaHistogram2d_flatten(&hist, &bins));

                        fprintf(stderr, "==================================\n");
                        for (i = 0; i < bins.size; i++) {
                                fprintf(
                                        stderr,
                                        "[%d, %d](%e)\n",
                                        bins.array[i].x,
                                        bins.array[i].y,
                                        bins.array[i].value
                                );
                        }

                        chk_fwrite(
                                &(bins.size),
                                sizeof(uint64_t),
                                1,
                                stdout
                        );
                        chk_fwrite(
                                &(bins.array[0]),
                                sizeof(struct mliCorsikaHistogram2dBin),
                                bins.size,
                                stdout
                        )
                        mliDynCorsikaHistogram2dBin_free(&bins);
                        fflush(stdout);

                } else {
                        chk_bad(
                                "Expected one of "
                                "['init.txt', 'cer.x8.float32', 'export.txt']."
                        );
                }
        }

        mliCorsikaHistogram2d_free(&hist);
        mliTar_read_finalize(&arc);

        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}

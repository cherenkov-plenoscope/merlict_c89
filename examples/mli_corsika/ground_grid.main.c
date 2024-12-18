#include <stdio.h>
#include <stdlib.h>
#include "../../src/corsika/ray_voxel_overlap.h"
#include "../../src/corsika/CorsikaPhotonBunch.h"
#include "../../src/corsika/EventTape.h"
#include "../../src/corsika/Histogram2d.h"
#include "../../src/chk/chk.h"
#include "../../src/string/string_numbers.h"
#include "../../src/math/math.h"
#include "../../src/io/io.h"
#include "../../src/io/io_text.h"
#include "../../src/tar/tar.h"
#include "../../src/tar/tar_io.h"
#include "../../src/vector/vector.h"
#include "../../src/AxisAlignedGrid/AxisAlignedGrid.h"

int mli_AxisAlignedGrid_set_from_config(
        struct mli_AxisAlignedGrid *grid,
        struct mli_IO *text)
{
        struct mli_Vec lower;
        struct mli_Vec upper;
        struct mliIdx3 num_bins;
        struct mli_String line = mli_String_init();
        chk(mli_String_malloc(&line, 100));
        mli_IO_rewind(text);
        /* X */
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_int64(&(num_bins.x), &line, 10));
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_double(&(lower.x), &line)); /* cm */
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_double(&(upper.x), &line)); /* cm */

        /* Y */
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_int64(&(num_bins.y), &line, 10));
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_double(&(lower.y), &line)); /* cm */
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_double(&(upper.y), &line)); /* cm */

        /* Z */
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_int64(&(num_bins.z), &line, 10));
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_double(&(lower.z), &line)); /* cm */
        chk(mli_IO_text_read_line(text, &line, '\n'));
        chk(mli_String_to_double(&(upper.z), &line)); /* cm */

        mli_String_free(&line);

        (*grid) = mli_AxisAlignedGrid_set(mli_AABB_set(lower, upper), num_bins);

        return 1;
chk_error:
        return 0;
}

#define HIST_TARGET_SIZE 10000

int main(int argc, char *argv[])
{
        struct mli_Tar arc = mli_Tar_init();
        struct mli_Tar out = mli_Tar_init();
        struct mli_TarHeader arch = mli_TarHeader_init();

        struct mli_corsika_Histogram2d hist = mli_corsika_Histogram2d_init();
        struct mli_AxisAlignedGrid grid;
        struct mli_AxisAlignedGridTraversal traversal;

        float raw[8] = {0.0};
        struct mli_corsika_PhotonBunch bunch;
        struct mli_Ray ray;
        int i;

        chk(mli_Tar_read_begin(&arc, stdin));
        chk(mli_Tar_write_begin(&out, stdout));
        chk(mli_corsika_Histogram2d_malloc(&hist, HIST_TARGET_SIZE));

        while (mli_Tar_read_header(&arc, &arch)) {
                if (strcmp(arch.name, "init.txt") == 0) {
                        struct mli_IO config_text = mli_IO_init();

                        if (hist.dict.capacity > 10 * HIST_TARGET_SIZE) {
                                chk(mli_corsika_Histogram2d_malloc(
                                        &hist, HIST_TARGET_SIZE));
                        } else {
                                mli_corsika_Histogram2d_reset(&hist);
                        }
                        chk(mli_IO_open_memory(&config_text));
                        chk(mli_Tar_read_data_to_IO(
                                &arc, &config_text, arch.size));
                        chk(mli_AxisAlignedGrid_set_from_config(
                                &grid, &config_text));
                        mli_IO_close(&config_text);
                } else if (strcmp(arch.name, "cer.x8.float32") == 0) {
                        const int num_bunches = arch.size / 32;
                        int num = 0;

                        for (i = 0; i < num_bunches; i++) {
                                chk_msg(mli_Tar_read_data(
                                                &arc, (void *)raw, sizeof(raw)),
                                        "Failed to read 'cer.x8.float32'.");
                                mli_corsika_PhotonBunch_set_from_raw(&bunch, raw);
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

                                traversal = mli_AxisAlignedGridTraversal_start(
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
                                                mli_Ray_fprint(stderr, &ray);
                                                fprintf(stderr, "\n");
                                                mli_AxisAlignedGridTraversal_fprint(
                                                        stderr, &traversal);
                                        }
                                        num += 1;
                                        chk(mli_corsika_Histogram2d_assign(
                                                &hist,
                                                traversal.voxel.x,
                                                traversal.voxel.y,
                                                bunch.weight_photons));
                                        mli_AxisAlignedGridTraversal_next(
                                                &traversal);
                                }
                        }
                } else if (strcmp(arch.name, "export.txt") == 0) {
                        struct mli_TarHeader outh = mli_TarHeader_init();
                        struct mliDynCorsikaHistogram2dBin bins =
                                mliDynCorsikaHistogram2dBin_init();
                        chk_msg(arch.size == 0, "Expected zero size.");
                        chk(mliDynCorsikaHistogram2dBin_malloc(
                                &bins, mli_corsika_Histogram2d_len(&hist)));
                        chk(mli_corsika_Histogram2d_flatten(&hist, &bins));
                        strcpy(outh.name, "histogram.int32_int32_float64");
                        outh.size = bins.size *
                                    sizeof(struct mli_corsika_Histogram2dBin);
                        chk_msg(mli_Tar_write_header(&out, &outh),
                                "Failed to write tar header to stdout");
                        chk_msg(mli_Tar_write_data(
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

        mli_corsika_Histogram2d_free(&hist);
        mli_Tar_read_finalize(&arc);
        mli_Tar_write_finalize(&out);
        fflush(out.stream);

        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}

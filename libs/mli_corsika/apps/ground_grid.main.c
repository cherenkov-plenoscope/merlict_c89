#include <stdio.h>
#include <stdlib.h>
#include "../src/mli_corsika_ray_voxel_overlap.h"
#include "../src/mli_corsika_CorsikaPhotonBunch.h"
#include "../src/mli_corsika_EventTape.h"
#include "../../mli/src/chk.h"
#include "../../mli/src/mli_math.h"
#include "../../mli/src/mliIo.h"
#include "../../mli/src/mliStr_numbers.h"
#include "../../mli/src/mliDynArray.h"

struct mliGroundGridPhotonBunch {
        uint64_t index;
        double weight;
};

MLIDYNARRAY_DEFINITON(
        mli,
        GroundGridPhotonBunch,
        struct mliGroundGridPhotonBunch)

MLIDYNARRAY_IMPLEMENTATION(
        mli,
        GroundGridPhotonBunch,
        struct mliGroundGridPhotonBunch)

struct mliGroundGrid {
        struct mliDynGroundGridPhotonBunch *grid;
        uint64_t num_x;
        uint64_t num_y;
        uint64_t num_overflow;
};

struct mliGroundGrid mliGroundGrid_init(void)
{
        struct mliGroundGrid gg;
        gg.num_overflow = 0u;
        gg.num_x = 0u;
        gg.num_y = 0u;
        gg.grid = NULL;
        return gg;
}

void mliGroundGrid_free(struct mliGroundGrid *gg)
{
        if (gg->grid != NULL) {
                uint64_t i;
                for (i = 0; i < gg->num_x * gg->num_y; i++) {
                        mliDynGroundGridPhotonBunch_free(&gg->grid[i]);
                }
        }
        free(gg->grid);
        (*gg) = mliGroundGrid_init();
}

int mliGroundGrid_malloc(
        struct mliGroundGrid *gg,
        const uint64_t num_x,
        const uint64_t num_y)
{
        uint64_t i;
        mliGroundGrid_free(gg);
        gg->num_x = num_x;
        gg->num_y = num_y;
        chk_malloc(
                gg->grid,
                struct mliDynGroundGridPhotonBunch,
                gg->num_x * gg->num_y);

        for (i = 0; i < gg->num_x * gg->num_y; i++) {
                gg->grid[i] = mliDynGroundGridPhotonBunch_init();
                chk_msg(mliDynGroundGridPhotonBunch_malloc(&gg->grid[i], 0),
                        "");
        }

        return 1;
chk_error:
        mliGroundGrid_free(gg);
        return 0;
}

uint64_t mliGroundGrid_idx(
        const struct mliGroundGrid *gg,
        const uint64_t x,
        const uint64_t y)
{
        return x * gg->num_y + y;
}

int mliGroundGrid_reset(struct mliGroundGrid *gg)
{
        uint64_t i;
        gg->num_overflow = 0u;
        for (i = 0; i < gg->num_x * gg->num_y; i++) {
                if (gg->grid[i].size > 25) {
                        chk(mliDynGroundGridPhotonBunch_malloc(
                                &gg->grid[i], 25));
                }
                gg->grid[i].size = 0;
        }

        return 1;
chk_error:
        return 0;
}

int mliGroundGrid_assign(
        struct mliGroundGrid *gg,
        const uint64_t x,
        const uint64_t y,
        const uint64_t bunch_index,
        const double bunch_weight)
{
        uint64_t i = mliGroundGrid_idx(gg, x, y);

        struct mliGroundGridPhotonBunch bunch;
        bunch.index = bunch_index;
        bunch.weight = bunch_weight;

        chk_msg(i < gg->num_x * gg->num_y, "index out of range.");
        chk_msg(mliDynGroundGridPhotonBunch_push_back((&gg->grid[i]), bunch),
                "woot?");

        return 1;
chk_error:
        return 0;
}

int mliGroundGrid_assign_overlap(
        struct mliGroundGrid *gg,
        const struct mliDynUint32 *x_idxs,
        const struct mliDynUint32 *y_idxs,
        const uint64_t bunch_index,
        const double bunch_weight)
{
        uint64_t oo;

        if (x_idxs->size == 0) {
                gg->num_overflow += 1;
        } else {
                for (oo = 0; oo < x_idxs->size; oo++) {
                        chk(mliGroundGrid_assign(
                                gg,
                                x_idxs->array[oo],
                                y_idxs->array[oo],
                                bunch_index,
                                bunch_weight));
                }
        }
        return 1;
chk_error:
        return 0;
}

int mliGroundGrid_fprint_jsonl(const struct mliGroundGrid *gg, FILE *f)
{
        uint64_t xx, yy, ii, jj = 0;
        uint64_t num_bins_written = 0;

        chk(fprintf(f, "{"));
        fprintf(f, "\"num_overflow\":%lu,", gg->num_overflow);
        fprintf(f, "\"assignment\":{", gg->num_overflow);
        for (xx = 0; xx < gg->num_x; xx++) {
                for (yy = 0; yy < gg->num_y; yy++) {
                        ii = mliGroundGrid_idx(gg, xx, yy);
                        if (gg->grid[ii].size > 0) {
                                if (num_bins_written > 0) {
                                        fprintf(f, ",");
                                }
                                double bin_weight = 0.0;
                                fprintf(f,
                                        "\"%06lu,%06lu\":{\"idx\":[",
                                        xx,
                                        yy);
                                for (jj = 0; jj < gg->grid[ii].size; jj++) {
                                        struct mliGroundGridPhotonBunch bb =
                                                gg->grid[ii].array[jj];
                                        fprintf(f, "%lu", bb.index);
                                        bin_weight += bb.weight;
                                        if (jj + 1 < gg->grid[ii].size) {
                                                fprintf(f, ",");
                                        }
                                }
                                fprintf(f, "],\"weight\":%f}", bin_weight);
                                num_bins_written = +1;
                        }
                }
        }
        fprintf(f, "}");
        fprintf(f, "}\n");
        return 1;
chk_error:
        return 0;
}

int read_config(
        const char *path,
        struct mliDynDouble *x_bin_edges,
        struct mliDynDouble *y_bin_edges,
        struct mliDynDouble *z_bin_edges)
{
        uint64_t nx, ny, nz, i;
        struct mliIo buff = mliIo_init();
        struct mliStr line = mliStr_init();

        chk(mliIo_malloc_from_path(&buff, path));
        mliIo_rewind(&buff);

        /* X */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_uint64(&nx, &line, 10));
        chk(mliDynDouble_malloc_set_size(x_bin_edges, nx));
        for (i = 0; i < nx; i++) {
                double val;
                chk(mli_readline(&buff, &line, '\n'));
                chk(mliStr_to_double(&val, &line));
                x_bin_edges->array[i] = val;
        }

        /* Y */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_uint64(&ny, &line, 10));
        chk(mliDynDouble_malloc_set_size(y_bin_edges, ny));
        for (i = 0; i < ny; i++) {
                double val;
                chk(mli_readline(&buff, &line, '\n'));
                chk(mliStr_to_double(&val, &line));
                y_bin_edges->array[i] = val;
        }

        /* Z */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_uint64(&nz, &line, 10));
        chk(mliDynDouble_malloc_set_size(z_bin_edges, nz));
        for (i = 0; i < nz; i++) {
                double val;
                chk(mli_readline(&buff, &line, '\n'));
                chk(mliStr_to_double(&val, &line));
                z_bin_edges->array[i] = val;
        }

        mliStr_free(&line);
        mliIo_free(&buff);

        return 1;
chk_error:
        return 0;
}

int main(int argc, char *argv[])
{
        FILE *istream = NULL;
        FILE *ostream = NULL;

        struct mliEventTapeReader arc = mliEventTapeReader_init();
        float runh[273] = {0.0};
        float evth[273] = {0.0};
        float raw[8] = {0.0};
        double xystart = -500 * 50e2;
        double xystop = 500 * 50e2;

        unsigned int num_bins_each_axis = 1000;
        uint64_t numover = (uint64_t)(1.5 * (double)num_bins_each_axis);

        struct mliGroundGrid grid = mliGroundGrid_init();

        struct mliCorsikaPhotonBunch bunch;

        struct mliDynDouble x_bin_edges = mliDynDouble_init();
        struct mliDynDouble y_bin_edges = mliDynDouble_init();
        struct mliDynDouble z_bin_edges = mliDynDouble_init();

        struct mliDynUint32 x_idxs = mliDynUint32_init();
        struct mliDynUint32 y_idxs = mliDynUint32_init();
        struct mliDynUint32 z_idxs = mliDynUint32_init();
        struct mliDynDouble overlaps = mliDynDouble_init();

        mli_linspace(xystart, xystop, x_bin_edges.array, x_bin_edges.size);
        mli_linspace(xystart, xystop, y_bin_edges.array, y_bin_edges.size);
        mli_linspace(-50e2, 50e2, z_bin_edges.array, z_bin_edges.size);

        mliDynUint32_malloc(&x_idxs, numover);
        mliDynUint32_malloc(&y_idxs, numover);
        mliDynUint32_malloc(&z_idxs, numover);
        mliDynDouble_malloc(&overlaps, numover);

        chk_msg(argc == 4,
                "Usage: ground_grid event_tape_path out_path config_path");

        chk_msg(read_config(argv[3], &x_bin_edges, &y_bin_edges, &z_bin_edges),
                "Can not read config with bin edges.");

        istream = fopen(argv[1], "rb");
        ostream = fopen(argv[2], "wt");
        chk(mliEventTapeReader_begin(&arc, istream));
        chk(mliEventTapeReader_read_runh(&arc, runh));

        chk(mliGroundGrid_malloc(&grid, x_bin_edges.size, y_bin_edges.size));

        while (mliEventTapeReader_read_evth(&arc, evth)) {
                uint64_t bunch_index = 0;
                uint64_t xx, yy, ii, jj = 0;
                mliGroundGrid_reset(&grid);

                while (mliEventTapeReader_read_cherenkov_bunch(&arc, raw)) {
                        unsigned int oo;
                        mliCorsikaPhotonBunch_set_from_raw(&bunch, raw);
                        mli_corsika_overlap_of_ray_with_voxels(
                                &bunch,
                                &x_bin_edges,
                                &y_bin_edges,
                                &z_bin_edges,
                                &x_idxs,
                                &y_idxs,
                                &z_idxs,
                                &overlaps);

                        chk(mliGroundGrid_assign_overlap(
                                &grid,
                                &x_idxs,
                                &y_idxs,
                                bunch_index,
                                bunch.weight_photons));

                        bunch_index += 1;
                }
                chk(mliGroundGrid_fprint_jsonl(&grid, ostream));
        }
        mliGroundGrid_free(&grid);
        chk(mliEventTapeReader_finalize(&arc));
        fclose(istream);
        fclose(ostream);

        mliDynDouble_free(&x_bin_edges);
        mliDynDouble_free(&y_bin_edges);
        mliDynDouble_free(&z_bin_edges);

        mliDynUint32_free(&x_idxs);
        mliDynUint32_free(&y_idxs);
        mliDynUint32_free(&z_idxs);
        mliDynDouble_free(&overlaps);

        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}

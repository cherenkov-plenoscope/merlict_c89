#include <stdio.h>
#include <stdlib.h>
#include "../src/mli_corsika_ray_voxel_overlap.h"
#include "../src/mli_corsika_CorsikaPhotonBunch.h"
#include "../src/mli_corsika_EventTape.h"
#include "../src/mli_corsika_Histogram2d.h"
#include "../../mli/src/chk.h"
#include "../../mli/src/mli_math.h"
#include "../../mli/src/mliIo.h"
#include "../../mli/src/mliStr_numbers.h"
#include "../../mli/src/mliDynArray.h"


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
        struct mliIo buff = mliIo_init();

        struct mliEventTapeReader arc = mliEventTapeReader_init();
        float runh[273] = {0.0};
        float evth[273] = {0.0};
        float raw[8] = {0.0};
        double xystart = -500 * 50e2;
        double xystop = 500 * 50e2;

        unsigned int num_bins_each_axis = 1000;
        uint64_t numover = (uint64_t)(1.5 * (double)num_bins_each_axis);

        struct mliCorsikaHistogram2d hist = mliCorsikaHistogram2d_init();

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

        chk(mliCorsikaHistogram2d_malloc(&hist, 10*1000));
        chk(mliIo_malloc_capacity(&buff, 10 * 1000));

        while (mliEventTapeReader_read_evth(&arc, evth)) {
                uint64_t bunch_index = 0;
                uint64_t xx, yy, ii, jj = 0;

                if (hist.dict.capacity > 10 * 1000 * 1000) {
                        chk(mliCorsikaHistogram2d_malloc(&hist, 10*1000));
                } else {
                        mliCorsikaHistogram2d_reset(&hist);
                }

                if (buff.capacity > 1000 * 1000) {
                        chk(mliIo_malloc_capacity(&buff, 1000 * 1000));
                } else {
                        buff.pos = 0;
                        buff.size = 0;
                }

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

                        for (oo = 0; oo < overlaps.size; oo ++)
                        {
                                chk(
                                        mliCorsikaHistogram2d_assign(
                                                &hist,
                                                x_idxs.array[oo],
                                                y_idxs.array[oo],
                                                bunch.weight_photons
                                        )
                                );
                        }

                        bunch_index += 1;
                }
                chk(mliCorsikaHistogram2d_dumps(&hist, &buff));
                buff.pos = 0;
                chk_fwrite(
                        buff.cstr,
                        sizeof(unsigned char),
                        buff.size,
                        ostream
                );
        }
        mliCorsikaHistogram2d_free(&hist);

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

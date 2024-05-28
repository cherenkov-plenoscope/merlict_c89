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
#include "../../mli/src/mli_ray_grid_traversal.h"
#include <time.h>

double clock2second(const clock_t t) { return ((double)t) / CLOCKS_PER_SEC; }

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

        struct mliCorsikaHistogram2d hist = mliCorsikaHistogram2d_init();

        struct mliCorsikaPhotonBunch bunch;
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;

        struct mliDynDouble x_bin_edges = mliDynDouble_init();
        struct mliDynDouble y_bin_edges = mliDynDouble_init();
        struct mliDynDouble z_bin_edges = mliDynDouble_init();

        chk_msg(argc == 4,
                "Usage: ground_grid event_tape_path out_path config_path");

        chk_msg(read_config(argv[3], &x_bin_edges, &y_bin_edges, &z_bin_edges),
                "Can not read config with bin edges.");

        chk_msg(z_bin_edges.size == 2, "Expected z_bin_edges.size == 2.");
        chk_msg(z_bin_edges.array[0] < 0, "Expected z_bin_edges[0] < 0");
        chk_msg(z_bin_edges.array[1] > 0, "Expected z_bin_edges[1] > 0");

        grid = mliAxisAlignedGrid_set(
                mliAABB_set(
                        mliVec_init(
                                x_bin_edges.array[0],
                                y_bin_edges.array[0],
                                z_bin_edges.array[0]),
                        mliVec_init(
                                x_bin_edges.array[x_bin_edges.size - 1],
                                y_bin_edges.array[y_bin_edges.size - 1],
                                z_bin_edges.array[z_bin_edges.size - 1])),
                mliIdx3_set(
                        x_bin_edges.size - 1,
                        y_bin_edges.size - 1,
                        z_bin_edges.size - 1));

        mliDynDouble_free(&x_bin_edges);
        mliDynDouble_free(&y_bin_edges);
        mliDynDouble_free(&z_bin_edges);

        istream = fopen(argv[1], "rb");
        ostream = fopen(argv[2], "wt");
        chk(mliEventTapeReader_begin(&arc, istream));
        chk(mliEventTapeReader_read_runh(&arc, runh));

        chk(mliCorsikaHistogram2d_malloc(&hist, 10 * 1000));
        chk(mliIo_malloc_capacity(&buff, 10 * 1000));

        while (mliEventTapeReader_read_evth(&arc, evth)) {
                clock_t t_ray_voxel = 0;
                clock_t t_avl_histogram = 0;
                uint64_t xx, yy, ii, jj = 0;

                if (hist.dict.capacity > 10 * 1000 * 1000) {
                        chk(mliCorsikaHistogram2d_malloc(&hist, 10 * 1000));
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
                        clock_t t1, t2, t3, t4, t5;
                        int num_overlaps = 0;
                        struct mliRay ray;
                        mliCorsikaPhotonBunch_set_from_raw(&bunch, raw);
                        ray.support.x = bunch.x_cm;
                        ray.support.y = bunch.y_cm;
                        ray.support.z = 0.0;
                        ray.direction.x = mli_corsika_ux_to_cx(bunch.ux);
                        ray.direction.y = mli_corsika_vy_to_cy(bunch.vy);
                        ray.direction.z =
                                mli_corsika_restore_direction_z_component(
                                        ray.direction.x, ray.direction.y);

                        t1 = clock();
                        traversal =
                                mliAxisAlignedGridTraversal_start(&grid, &ray);
                        t2 = clock();
                        t_ray_voxel += (t2 - t1);

                        while (traversal.valid) {
                                num_overlaps += 1;
                                t3 = clock();
                                chk(mliCorsikaHistogram2d_assign(
                                        &hist,
                                        traversal.voxel.x,
                                        traversal.voxel.y,
                                        bunch.weight_photons));
                                t4 = clock();
                                t_avl_histogram += (t4 - t3);
                                mliAxisAlignedGridTraversal_next(&traversal);
                                t5 = clock();
                                t_ray_voxel += (t5 - t4);
                        }
                }
                chk(mliCorsikaHistogram2d_dumps(&hist, &buff));
                buff.pos = 0;
                chk_fwrite(
                        buff.cstr, sizeof(unsigned char), buff.size, ostream);

                fprintf(stdout,
                        "t_ray_voxel: %es\n",
                        clock2second(t_ray_voxel));
                fprintf(stdout,
                        "t_avl_histogram: %es\n",
                        clock2second(t_avl_histogram));
        }
        mliCorsikaHistogram2d_free(&hist);

        chk(mliEventTapeReader_finalize(&arc));
        fclose(istream);
        fclose(ostream);

        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}

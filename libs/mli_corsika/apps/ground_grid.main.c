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

int read_config(
        const char *path,
        struct mliVec *lower,
        struct mliVec *upper,
        struct mliIdx3 *num_bins)
{
        struct mliIo buff = mliIo_init();
        struct mliStr line = mliStr_init();

        chk(mliIo_malloc_from_path(&buff, path));
        mliIo_rewind(&buff);

        /* X */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_int64(&(num_bins->x), &line, 10));
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_double(&(lower->x), &line)); /* cm */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_double(&(upper->x), &line)); /* cm */

        /* Y */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_int64(&(num_bins->y), &line, 10));
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_double(&(lower->y), &line)); /* cm */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_double(&(upper->y), &line)); /* cm */

        /* Z */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_int64(&(num_bins->z), &line, 10));
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_double(&(lower->z), &line)); /* cm */
        chk(mli_readline(&buff, &line, '\n'));
        chk(mliStr_to_double(&(upper->z), &line)); /* cm */

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

        struct mliVec lower;
        struct mliVec upper;
        struct mliIdx3 num_bins;
        struct mliCorsikaPhotonBunch bunch;
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;

        chk_msg(argc == 4,
                "Usage: ground_grid event_tape_path out_path config_path");

        chk_msg(read_config(argv[3], &lower, &upper, &num_bins),
                "Can not read config with bin edges.");

        grid = mliAxisAlignedGrid_set(mliAABB_set(lower, upper), num_bins);

        istream = fopen(argv[1], "rb");
        ostream = fopen(argv[2], "wt");
        chk(mliEventTapeReader_begin(&arc, istream));
        chk(mliEventTapeReader_read_runh(&arc, runh));

        chk(mliCorsikaHistogram2d_malloc(&hist, 10 * 1000));
        chk(mliIo_malloc_capacity(&buff, 10 * 1000));

        while (mliEventTapeReader_read_evth(&arc, evth)) {
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

                        traversal =
                                mliAxisAlignedGridTraversal_start(&grid, &ray);

                        while (traversal.valid) {
                                chk(mliCorsikaHistogram2d_assign(
                                        &hist,
                                        traversal.voxel.x,
                                        traversal.voxel.y,
                                        bunch.weight_photons));
                                mliAxisAlignedGridTraversal_next(&traversal);
                        }
                }
                chk(mliCorsikaHistogram2d_dumps(&hist, &buff));
                buff.pos = 0;
                chk_fwrite(
                        buff.cstr, sizeof(unsigned char), buff.size, ostream);
        }
        mliCorsikaHistogram2d_free(&hist);

        chk(mliEventTapeReader_finalize(&arc));
        fclose(istream);
        fclose(ostream);

        return EXIT_SUCCESS;
chk_error:
        return EXIT_FAILURE;
}

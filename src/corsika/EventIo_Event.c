/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "EventIo_Event.h"
#include "EventIo_Run.h"
#include "../chk/chk.h"
#include "../mli/mliDynFloat.h"
#include <math.h>

int mliEventIo_read_telescope_offsets(
        FILE *f,
        struct mliDynEventIoTelescopeOffset *telescope_offsets,
        const uint64_t length)
{
        const int length_first_two = 4 + 4;
        int num_following_arrays;
        int32_t _narray;
        uint64_t narray;
        uint64_t n;
        float toff;

        struct mliDynFloat xoff = mliDynFloat_init();
        struct mliDynFloat yoff = mliDynFloat_init();
        struct mliDynFloat weight = mliDynFloat_init();

        chk_fread(&_narray, sizeof(int32_t), 1, f);
        chk_msg(_narray >= 0, "Expected num. of arrays to be positive.");
        narray = (uint64_t)_narray;

        chk_fread(&toff, sizeof(float), 1, f);

        chk(mliDynFloat_malloc_set_size(&xoff, narray));
        chk(mliDynFloat_malloc_set_size(&yoff, narray));
        chk(mliDynFloat_malloc_set_size(&weight, narray));

        num_following_arrays = (int)((length - length_first_two) / narray / 4);

        chk_fread(xoff.array, sizeof(float), narray, f);
        chk_fread(yoff.array, sizeof(float), narray, f);
        memset(weight.array, 1.0, narray);

        switch (num_following_arrays) {
        case 2:
                /* do nothing, we already read the 2 arrays xoff and yoff. */
                break;
        case 3:
                chk_fread(weight.array, sizeof(float), narray, f);
                break;
        default:
                chk_bad("Expected num_following_arrays to be either 2, or 3.");
        }

        chk_msg(mliDynEventIoTelescopeOffset_malloc(
                        telescope_offsets, narray),
                "Failed to malloc telescope_offsets.");

        for (n = 0; n < narray; n++) {
                telescope_offsets->array[n].xoff = xoff.array[n];
                telescope_offsets->array[n].yoff = yoff.array[n];
                telescope_offsets->array[n].toff = toff;
                telescope_offsets->array[n].weight = weight.array[n];
        }

        mliDynFloat_free(&xoff);
        mliDynFloat_free(&yoff);
        mliDynFloat_free(&weight);

        return 1;
chk_error:
        return 0;
}

struct mliEventIoBunchHeader {
        int16_t array_id;
        int16_t telescope_id;
        float photons;
        int32_t num_bunches;
};

int mliEventIo_read_photon_bunches(
        FILE *f,
        struct mliEventIoBunchHeader *b_head,
        struct mliDynCorsikaPhotonBunch *bunches,
        const int32_t version)
{
        float tmp[8];
        int is_compact = 0;
        uint64_t row, field;

        chk_fread(&b_head->array_id, sizeof(b_head->array_id), 1, f);
        chk_fread(&b_head->telescope_id, sizeof(b_head->telescope_id), 1, f);
        chk_fread(&b_head->photons, sizeof(b_head->photons), 1, f);
        chk_fread(&b_head->num_bunches, sizeof(b_head->num_bunches), 1, f);

        is_compact = (int)(version / 1000 == 1);

        chk_msg(mliDynCorsikaPhotonBunch_malloc_set_size(
                        bunches, b_head->num_bunches),
                "Failed to malloc bunches.");

        if (is_compact) {
                int16_t buff[8];
                for (row = 0; row < bunches->size; row++) {
                        chk_fread(buff, sizeof(int16_t), 8, f);

                        for (field = 0; field < 8; field++) {
                                tmp[field] = (float)buff[field];
                        }

                        bunches->array[row].x_cm = tmp[0] * 0.1;
                        bunches->array[row].y_cm = tmp[1] * 0.1;
                        bunches->array[row].ux = tmp[2] / 30000;
                        bunches->array[row].vy = tmp[3] / 30000;
                        bunches->array[row].time_ns = tmp[4] * 0.1;
                        bunches->array[row].z_emission_cm =
                                pow(10, tmp[5] * 0.001);
                        bunches->array[row].weight_photons = tmp[6] * 0.01;
                        bunches->array[row].wavelength_nm = tmp[7];
                }
        } else {
                for (row = 0; row < bunches->size; row++) {
                        chk_fread(tmp, sizeof(float), 8, f);

                        bunches->array[row].x_cm = tmp[0];
                        bunches->array[row].y_cm = tmp[1];
                        bunches->array[row].ux = tmp[2];
                        bunches->array[row].vy = tmp[3];
                        bunches->array[row].time_ns = tmp[4];
                        bunches->array[row].z_emission_cm = tmp[5];
                        bunches->array[row].weight_photons = tmp[6];
                        bunches->array[row].wavelength_nm = tmp[7];
                }
        }

        return 1;
chk_error:
        return 0;
}

struct mliEventIoEvent mliEventIoEvent_init(void)
{
        struct mliEventIoEvent event;
        memset(event.corsika_event_header, 0.0, 273 * sizeof(float));
        memset(event.corsika_event_end, 0.0, 273 * sizeof(float));
        event.telescopes = mliDynEventIoTelescope_init();
        return event;
}
void mliEventIoEvent_free(struct mliEventIoEvent *event)
{
        uint64_t i;
        for (i = 0; i < event->telescopes.size; i++) {
                mliEventIoTelescope_free(&event->telescopes.array[i]);
        }
        mliDynEventIoTelescope_free(&event->telescopes);
        (*event) = mliEventIoEvent_init();
}

int mliEventIoEvent_malloc_from_run(
        struct mliEventIoEvent *event,
        struct mliEventIoRun *run)
{
        uint64_t remaining_length = 0;
        uint64_t header_length = 0;
        uint64_t num_sub_blocks = 0;
        uint64_t i;
        struct mliDynEventIoTelescopeOffset tmp_offsets =
                mliDynEventIoTelescopeOffset_init();

        mliEventIoEvent_free(event);

        /* corsika_event_header */
        /* -------------------- */
        chk_msg(run->next_block.type == 1202, "Expected type 1202.");
        chk_msg(mliEventIoRun_read_273float32_block(
                        run->f, event->corsika_event_header),
                "Failed to read corsika_event_header 273 float block.");

        /* telescope_offsets */
        /* ----------------- */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        chk_msg(run->next_block.type == 1203, "Expected type 1203.");
        chk_msg(mliEventIo_read_telescope_offsets(
                        run->f, &tmp_offsets, run->next_block.length),
                "Failed to read telescope_offsets.");

        /* array_header */
        /* ------------ */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        chk_msg(run->next_block.type == 1204, "Expected type 1204.");
        chk_msg(run->next_block.only_sub_objects,
                "Expected telescope-array-block to only contain sub-blocks.");
        remaining_length = run->next_block.length;

        /* telescopes */
        /* ---------- */
        chk_msg(mliDynEventIoTelescope_malloc(
                        &event->telescopes, tmp_offsets.size),
                "Failed to malloc telescpes");
        for (i = 0; i < event->telescopes.size; i++) {
                event->telescopes.array[i] = mliEventIoTelescope_init();
                event->telescopes.array[i].offset = tmp_offsets.array[i];
        }
        mliDynEventIoTelescopeOffset_free(&tmp_offsets);

        while (remaining_length) {

                /* photon_bunches for each telescope */
                /* --------------------------------- */
                struct mliEventIoBunchHeader b_head;
                struct mliEventIoTelescope *telescope =
                        &event->telescopes.array[num_sub_blocks];

                header_length = mliEventIoHeader_read(
                        &run->next_block, run->f, MLI_EVENTIO_SUB_LEVEL);
                chk_msg(header_length,
                        "Failed to read EventIo-SUB-block-header.");
                remaining_length -= header_length;

                chk_msg(run->next_block.type == 1205,
                        "Expected subheader type 1205");

                telescope->array_id = b_head.array_id;
                telescope->telescope_id = b_head.telescope_id;
                chk_msg(mliEventIo_read_photon_bunches(
                                run->f,
                                &b_head,
                                &telescope->photon_bunches,
                                run->next_block.version),
                        "Failed to read photon_bunches.");
                remaining_length -= run->next_block.length;
                num_sub_blocks += 1;
        }

        chk_msg(remaining_length == 0, "Expected remaining_length == 0.");
        chk_msg(num_sub_blocks == event->telescopes.size,
                "Expected every telescope to have photon_bunches.");

        /* corsika_event_end */
        /* ----------------- */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        chk_msg(run->next_block.type == 1209, "Expected type 1209.");
        chk_msg(mliEventIoRun_read_273float32_block(
                        run->f, event->corsika_event_end),
                "Failed to read corsika_event_end 273 float block.");

        /* next */
        /* ---- */
        chk(mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));

        return 1;
chk_error:
        return 0;
}

/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "../../merlict_c89_corsika/EventIo/Event.h"

int _read_telescope_offsets(
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

        mli_check_fread(&_narray, sizeof(int32_t), 1, f);
        mli_check_message(_narray >= 0, "Expected num. of arrays to be positive.");
        narray = (uint64_t)_narray;

        mli_check_fread(&toff, sizeof(float), 1, f);

        mli_check(mliDynFloat_malloc_set_size(&xoff, narray));
        mli_check(mliDynFloat_malloc_set_size(&yoff, narray));
        mli_check(mliDynFloat_malloc_set_size(&weight, narray));

        num_following_arrays = (int)((length - length_first_two) / narray / 4);

        mli_check_fread(xoff.array, sizeof(float), narray, f);
        mli_check_fread(yoff.array, sizeof(float), narray, f);
        memset(weight.array, 1.0, narray);

        switch (num_following_arrays) {
        case 2:
                /* do nothing, we already read the 2 arrays xoff and yoff. */
                break;
        case 3:
                mli_check_fread(weight.array, sizeof(float), narray, f);
                break;
        default:
                mli_sentinel(
                        "Expected num_following_arrays to be either 2, or 3.");
        }

        mli_check_message(
                mliDynEventIoTelescopeOffset_malloc_set_size(
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
error:
        return 0;
}

struct _BunchHeader {
        int16_t array_id;
        int16_t telescope_id;
        float photons;
        int32_t num_bunches;
};

int _read_photon_bunches(
        FILE *f,
        struct _BunchHeader *b_head,
        struct mliDynCorsikaPhotonBunch *bunches,
        const int32_t version)
{
        float tmp[8];
        int is_compact = 0;
        uint64_t row, field;

        mli_check_fread(&b_head->array_id, sizeof(b_head->array_id), 1, f);
        mli_check_fread(&b_head->telescope_id, sizeof(b_head->telescope_id), 1, f);
        mli_check_fread(&b_head->photons, sizeof(b_head->photons), 1, f);
        mli_check_fread(&b_head->num_bunches, sizeof(b_head->num_bunches), 1, f);

        is_compact = (int)(version / 1000 == 1);

        mli_check_message(
                mliDynCorsikaPhotonBunch_malloc_set_size(
                        bunches, b_head->num_bunches),
                "Failed to malloc bunches.");

        if (is_compact) {
                int16_t buff[8];
                for (row = 0; row < bunches->size; row++) {
                        mli_check_fread(buff, sizeof(int16_t), 8, f);

                        for (field = 0; field < 8; field++) {
                                tmp[field] = (float)buff[field];
                        }

                        bunches->array[row].x_cm = tmp[0] * 0.1;
                        bunches->array[row].y_cm = tmp[1] * 0.1;
                        bunches->array[row].cx_rad = tmp[2] / 30000;
                        bunches->array[row].cy_rad = tmp[3] / 30000;
                        bunches->array[row].time_ns = tmp[4] * 0.1;
                        bunches->array[row].z_emission_cm =
                                pow(10, tmp[5] * 0.001);
                        bunches->array[row].weight_photons = tmp[6] * 0.01;
                        bunches->array[row].wavelength_nm = tmp[7];
                }
        } else {
                for (row = 0; row < bunches->size; row++) {
                        mli_check_fread(tmp, sizeof(float), 8, f);

                        bunches->array[row].x_cm = tmp[0];
                        bunches->array[row].y_cm = tmp[1];
                        bunches->array[row].cx_rad = tmp[2];
                        bunches->array[row].cy_rad = tmp[3];
                        bunches->array[row].time_ns = tmp[4];
                        bunches->array[row].z_emission_cm = tmp[5];
                        bunches->array[row].weight_photons = tmp[6];
                        bunches->array[row].wavelength_nm = tmp[7];
                }
        }

        return 1;
error:
        return 0;
}

struct mliEventIoEvent mliEventIoEvent_init(void)
{
        struct mliEventIoEvent event;
        memset(event.corsika_event_header, 0.0, 273);
        memset(event.corsika_event_end, 0.0, 273);
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
        mli_check_message(run->_next_block.type == 1202, "Expected type 1202.");
        mli_check_message(
                _read_273_block(run->_f, event->corsika_event_header),
                "Failed to read corsika_event_header 273 float block.");

        /* telescope_offsets */
        /* ----------------- */
        mli_check(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        mli_check_message(run->_next_block.type == 1203, "Expected type 1203.");
        mli_check_message(
                _read_telescope_offsets(
                        run->_f, &tmp_offsets, run->_next_block.length),
                "Failed to read telescope_offsets.");

        /* array_header */
        /* ------------ */
        mli_check(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        mli_check_message(run->_next_block.type == 1204, "Expected type 1204.");
        mli_check_message(
                run->_next_block.only_sub_objects,
                "Expected telescope-array-block to only contain sub-blocks.");
        remaining_length = run->_next_block.length;

        /* telescopes */
        /* ---------- */
        mli_check_message(
                mliDynEventIoTelescope_malloc_set_size(
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
                struct _BunchHeader b_head;
                struct mliEventIoTelescope *telescope =
                        &event->telescopes.array[num_sub_blocks];

                header_length = mliEventIoHeader_read(
                        &run->_next_block, run->_f, MLI_EVENTIO_SUB_LEVEL);
                mli_check_message(
                        header_length,
                        "Failed to read EventIo-SUB-block-header.");
                remaining_length -= header_length;

                mli_check_message(
                        run->_next_block.type == 1205,
                        "Expected subheader type 1205");

                telescope->array_id = b_head.array_id;
                telescope->telescope_id = b_head.telescope_id;
                mli_check_message(
                        _read_photon_bunches(
                                run->_f,
                                &b_head,
                                &telescope->photon_bunches,
                                run->_next_block.version),
                        "Failed to read photon_bunches.");
                remaining_length -= run->_next_block.length;
                num_sub_blocks += 1;
        }

        mli_check_message(remaining_length == 0, "Expected remaining_length == 0.");
        mli_check_message(
                num_sub_blocks == event->telescopes.size,
                "Expected every telescope to have photon_bunches.");

        /* corsika_event_end */
        /* ----------------- */
        mli_check(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));
        mli_check_message(run->_next_block.type == 1209, "Expected type 1209.");
        mli_check_message(
                _read_273_block(run->_f, event->corsika_event_end),
                "Failed to read corsika_event_end 273 float block.");

        /* next */
        /* ---- */
        mli_check(_mliEventIoRun_next_block(run, MLI_EVENTIO_TOP_LEVEL));

        return 1;
error:
        return 0;
}
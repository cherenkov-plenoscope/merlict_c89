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

        mli_fread(&_narray, sizeof(int32_t), 1, f);
        mli_check(_narray >= 0, "Expected num. of arrays to be positive.");
        narray = (uint64_t)_narray;

        mli_fread(&toff, sizeof(float), 1, f);

        mli_c(mliDynFloat_malloc_set_size(&xoff, narray));
        mli_c(mliDynFloat_malloc_set_size(&yoff, narray));
        mli_c(mliDynFloat_malloc_set_size(&weight, narray));

        num_following_arrays = (int)((length - length_first_two) / narray / 4);

        mli_fread(xoff.array, sizeof(float), narray, f);
        mli_fread(yoff.array, sizeof(float), narray, f);
        memset(weight.array, 1.0, narray);

        switch (num_following_arrays) {
        case 2:
                /* do nothing, we already read the 2 arrays xoff and yoff. */
                break;
        case 3:
                mli_fread(weight.array, sizeof(float), narray, f);
                break;
        default:
                mli_sentinel(
                        "Expected num_following_arrays to be either 2, or 3.");
        }

        mli_check(
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
        int16_t array;
        int16_t tel;
        float photons;
        int32_t num_bunches;
};

int _read_photon_bunches(
        FILE *f,
        struct mliDynCorsikaPhotonBunch *bunches,
        const struct mliEventIoHeader eventio_head)
{
        struct _BunchHeader b_head;
        float tmp[8];
        int is_compact = 0;
        uint64_t row, field;

        mli_fread(&b_head.array, sizeof(b_head.array), 1, f);
        mli_fread(&b_head.tel, sizeof(b_head.tel), 1, f);
        mli_fread(&b_head.photons, sizeof(b_head.photons), 1, f);
        mli_fread(&b_head.num_bunches, sizeof(b_head.num_bunches), 1, f);

        fprintf(stderr, "b_head.array %d\n", b_head.array);
        fprintf(stderr, "b_head.tel %d\n", b_head.tel);
        fprintf(stderr, "b_head.photons %f\n", b_head.photons);
        fprintf(stderr, "b_head.num_bunches %u\n", b_head.num_bunches);

        is_compact = (int)(eventio_head.version / 1000 == 1);

        mli_check(
                mliDynCorsikaPhotonBunch_malloc_set_size(
                        bunches, b_head.num_bunches),
                "Failed to malloc bunches.");

        if (is_compact) {
                int16_t buff[8];
                for (row = 0; row < bunches->size; row++) {
                        mli_fread(buff, sizeof(int16_t), 8, f);

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
                        mli_fread(tmp, sizeof(float), 8, f);

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
        event.telescope_offsets = mliDynEventIoTelescopeOffset_init();
        event.photon_bunches = mliDynCorsikaPhotonBunch_init();
        return event;
}
void mliEventIoEvent_free(struct mliEventIoEvent *event)
{
        mliDynEventIoTelescopeOffset_free(&event->telescope_offsets);
        mliDynCorsikaPhotonBunch_free(&event->photon_bunches);
        (*event) = mliEventIoEvent_init();
}

int mliEventIoEvent_malloc_from_run(
        struct mliEventIoEvent *event,
        struct mliEventIoRun *run)
{
        struct mliEventIoHeader a, b, c, d;
        mliEventIoEvent_free(event);

        /* corsika_event_header */
        /* -------------------- */
        mli_check(
                mliEventIoHeader_read(
                        &a, run->f, MLI_EVENTIO_TOP_LEVEL),
                "Failed to read EventIo-header.");
        mli_check(a.type == 1202, "Expected type 1202.");
        mliEventIoHeader_fprint(a, stderr);
        mli_check(
                _read_273_block(run->f, event->corsika_event_header),
                "Failed to read corsika_event_header 273 float block.");

        /* telescope_offsets */
        /* ----------------- */
        mli_check(
                mliEventIoHeader_read(
                        &b, run->f, MLI_EVENTIO_TOP_LEVEL),
                "Failed to read EventIo-header.");
        mli_check(b.type == 1203, "Expected type 1203.");
        mliEventIoHeader_fprint(b, stderr);
        mli_check(
                _read_telescope_offsets(
                        run->f, &event->telescope_offsets, b.length),
                "Failed to read telescope_offsets.");

        /* array_header */
        /* ------------ */
        mli_check(
                mliEventIoHeader_read(
                        &c, run->f, MLI_EVENTIO_TOP_LEVEL),
                "Failed to read EventIo-header.");
        mli_check(c.type == 1204, "Expected type 1204.");

        /* photon_bunches */
        /* -------------- */
        mli_check(
                mliEventIoHeader_read(
                        &d, run->f, MLI_EVENTIO_SUB_LEVEL),
                "Failed to read subhead for photon_bunches.");
        mli_check(d.type == 1205, "Expected subheader type 1205");
        mliEventIoHeader_fprint(d, stderr);
        mli_check(
                _read_photon_bunches(run->f, &event->photon_bunches, d),
                "Failed to read photon_bunches.");

        return 1;
error:
        return 0;
}

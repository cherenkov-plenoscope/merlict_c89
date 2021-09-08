/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "../merlict_c89_corsika/mliEventIo.h"

MLIDYNARRAY_IMPLEMENTATION(mli, EventIoTelPos, struct mliEventIoTelPos)
MLIDYNARRAY_IMPLEMENTATION(mli, EventIoTelOffset, struct mliEventIoTelOffset)

struct mliEventIoEvent mliEventIoEvent_init(void)
{
        struct mliEventIoEvent evt;
        memset(evt.corsika_event_header, 0.0, 273);
        evt.telescope_offsets = mliDynEventIoTelOffset_init();
        evt.photon_bunches = mliDynCorsikaPhotonBunch_init();
        return evt;
}
void mliEventIoEvent_free(struct mliEventIoEvent *evt)
{
        mliDynEventIoTelOffset_free(&evt->telescope_offsets);
        mliDynCorsikaPhotonBunch_free(&evt->photon_bunches);
        (*evt) = mliEventIoEvent_init();
}

struct mliEventIoRun mliEventIoRun_init(void)
{
        struct mliEventIoRun runstream;
        runstream.f = NULL;
        memset(runstream.corsika_run_header, 0.0, 273);
        runstream.corsika_input_card = mliDynStr_init();
        runstream.telescope_positions = mliDynEventIoTelPos_init();
        return runstream;
}

int _read_273_block(FILE *f, float *block, const int32_t type)
{
        struct mliEventIoHeader head;
        int32_t block_size;
        mli_check(
                mliEventIoHeader_read_from_file(&head, f, 1),
                "Failed to read event-io-header.");
        mli_check(head.type == type, "Expected different header-type=.");
        mliEventIoHeader_fprint(head, stderr);

        mli_fread(&block_size, sizeof(int32_t), 1, f);
        mli_check(block_size == 273, "Expected block-size to be 273.");
        mli_fread(block, sizeof(float), (uint64_t)block_size, f);
        return 1;
error:
        return 0;
}

int _read_runh_273_block(FILE *f, float *block)
{
        return _read_273_block(f, block, 1200);
}

int _read_rune_273_block(FILE *f, float *block)
{
        return _read_273_block(f, block, 1210);
}

int _read_evth_273_block(FILE *f, float *block)
{
        return _read_273_block(f, block, 1202);
}

int _read_evte_273_block(FILE *f, float *block)
{
        return _read_273_block(f, block, 1209);
}

int _read_input_card(FILE *f, struct mliDynStr *input_card)
{
        struct mliEventIoHeader head;
        char _unknown[8];
        uint64_t input_card_length;
        mli_check(
                mliEventIoHeader_read_from_file(
                        &head, f, MLI_EVENTIO_TOP_LEVEL),
                "Failed to read event-io-header from file.");
        mli_check(
                head.type == 1212, "Expected corsika-input-card, type: 1212.");
        mliEventIoHeader_fprint(head, stderr);

        mli_check(
                mliDynStr_malloc(input_card, head.length + 1),
                "Failed to malloc c_str for input-card.");

        mli_fread(_unknown, sizeof(_unknown), 1, f);
        mli_check(
                head.length >= sizeof(_unknown),
                "Expected at least 8bytes payload.");
        input_card_length = head.length - sizeof(_unknown);
        mli_fread(input_card->c_str, sizeof(char), input_card_length, f);
        return 1;
error:
        return 0;
}

int _read_telescope_positions(
        FILE *f,
        struct mliDynEventIoTelPos *telescope_positions)
{
        struct mliEventIoHeader head;
        int32_t ntel;
        int num_following_arrays;

        mli_check(
                mliEventIoHeader_read_from_file(
                        &head, f, MLI_EVENTIO_TOP_LEVEL),
                "Failed to read event-io-header from file.");
        mli_check(
                head.type == 1201, "Expected telescope-positions, type: 1201.");
        mliEventIoHeader_fprint(head, stderr);

        mli_fread(&ntel, sizeof(int32_t), 1, f);

        num_following_arrays = (int)((head.length - 4) / ntel / 4);

        mli_check(num_following_arrays == 4, "Expected exactly four arrays.")

                mli_c(mliDynEventIoTelPos_malloc_set_size(
                        telescope_positions, ntel));
        mli_fread(
                telescope_positions->array,
                sizeof(struct mliEventIoTelPos),
                (uint64_t)ntel,
                f);
        return 1;
error:
        return 0;
}

int _read_telescope_offsets(
        FILE *f,
        struct mliDynEventIoTelOffset *telescope_offsets)
{
        struct mliEventIoHeader head;

        const int length_first_two = 4 + 4;
        int num_following_arrays;
        int32_t _narray;
        uint64_t narray;
        uint64_t n;
        float toff;

        struct mliDynFloat xoff = mliDynFloat_init();
        struct mliDynFloat yoff = mliDynFloat_init();
        struct mliDynFloat weight = mliDynFloat_init();

        mli_check(
                mliEventIoHeader_read_from_file(
                        &head, f, MLI_EVENTIO_TOP_LEVEL),
                "Failed to read event-io-header from file.");
        mli_check(head.type == 1203, "Expected telescope-offsets, type: 1203.");
        mliEventIoHeader_fprint(head, stderr);

        mli_fread(&_narray, sizeof(int32_t), 1, f);
        mli_check(_narray >= 0, "Expected num. of arrays to be positive.");
        narray = (uint64_t)_narray;

        mli_fread(&toff, sizeof(float), 1, f);

        mli_c(mliDynFloat_malloc_set_size(&xoff, narray));
        mli_c(mliDynFloat_malloc_set_size(&yoff, narray));
        mli_c(mliDynFloat_malloc_set_size(&weight, narray));

        num_following_arrays =
                (int)((head.length - length_first_two) / narray / 4);

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
                mliDynEventIoTelOffset_malloc_set_size(
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

int _read_telescope_array_header_1204(FILE *f)
{
        struct mliEventIoHeader head;
        mli_check(
                mliEventIoHeader_read_from_file(
                        &head, f, MLI_EVENTIO_TOP_LEVEL),
                "Failed to read telescope_array_header, type: 1204.");
        mli_check(head.type == 1204, "Expected subheader of type 1204");
        mliEventIoHeader_fprint(head, stderr);
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

int _read_photon_bunches(FILE *f, struct mliDynCorsikaPhotonBunch *bunches)
{
        struct mliEventIoHeader subhead;
        struct _BunchHeader b_head;
        float tmp[8];
        int is_compact = 0;
        uint64_t row, field;

        mli_check(
                mliEventIoHeader_read_from_file(
                        &subhead, f, MLI_EVENTIO_SUB_LEVEL),
                "Failed to read subhead for photon_bunches.");
        mli_check(subhead.type == 1205, "Expected subheader of type 1205");
        mliEventIoHeader_fprint(subhead, stderr);

        mli_fread(&b_head.array, sizeof(b_head.array), 1, f);
        mli_fread(&b_head.tel, sizeof(b_head.tel), 1, f);
        mli_fread(&b_head.photons, sizeof(b_head.photons), 1, f);
        mli_fread(&b_head.num_bunches, sizeof(b_head.num_bunches), 1, f);

        fprintf(stderr, "b_head.array %d\n", b_head.array);
        fprintf(stderr, "b_head.tel %d\n", b_head.tel);
        fprintf(stderr, "b_head.photons %f\n", b_head.photons);
        fprintf(stderr, "b_head.num_bunches %u\n", b_head.num_bunches);

        is_compact = (int)(subhead.version / 1000 == 1);

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

int mliEventIoRun_malloc(struct mliEventIoRun *runstream, const char *path)
{
        runstream->f = fopen(path, "rb");
        mli_check(runstream->f, "Can not open event-io-file.");

        mli_check(
                _read_runh_273_block(
                        runstream->f, runstream->corsika_run_header),
                "Failed to read corsika_run_header 273 float block.");
        mli_check(
                _read_input_card(runstream->f, &runstream->corsika_input_card),
                "Failed to read corsika-input-card.");
        mli_check(
                _read_telescope_positions(
                        runstream->f, &runstream->telescope_positions),
                "Failed to read telescope-positions.");

        return 1;
error:
        return 0;
}

void mliEventIoRun_free(struct mliEventIoRun *runstream)
{
        mliDynStr_free(&runstream->corsika_input_card);
        mliDynEventIoTelPos_free(&runstream->telescope_positions);
        fclose(runstream->f);
}

int mliEventIoRun_malloc_next_event(
        struct mliEventIoRun *runstream,
        struct mliEventIoEvent *event)
{
        mliEventIoEvent_free(event);
        mli_check(
                _read_evth_273_block(runstream->f, event->corsika_event_header),
                "Failed to read corsika_event_header 273 float block.");
        mli_check(
                _read_telescope_offsets(
                        runstream->f, &event->telescope_offsets),
                "");
        mli_check(_read_telescope_array_header_1204(runstream->f), "");
        mli_check(
                _read_photon_bunches(runstream->f, &event->photon_bunches), "");
        return 1;
error:
        return 0;
}

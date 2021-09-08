/* Copyright 2020 Sebastian A. Mueller*/
#include "../../merlict_c89_corsika/TarIo/mliTarIo.h"
#include "../../merlict_c89_corsika/utils.h"
#include "../../merlict_c89/mliTar.h"

struct mliTarIoRun mliTarIoRun_init(void)
{
        struct mliTarIoRun run;
        run.tar = mliTar_init();
        memset(run.corsika_run_header, 0.0, 273);
        return run;
}

int mliTarIoRun_open(struct mliTarIoRun *run, const char *path)
{
        struct mliTarHeader info = mliTarHeader_init();
        mli_check(mliTar_open(&run->tar, path, "r"), "Failed to open tar.");
        mli_check(
                mliTar_read_header(&run->tar, &info),
                "Failed to read tarinfo for corsika_run_header from tar.");
        mli_check(
                strcmp(info.name, "runh.float32") == 0,
                "Expected first file to be 'runh.float32'.");
        mli_check(
                info.size == 273 * sizeof(float),
                "Expected corsika_run_header to have size 273*sizeof(float).");
        mli_check(
                mliTar_read_data(
                        &run->tar, (void *)&(run->corsika_run_header), info.size),
                "Failed to read corsika_run_header from tar.");
        mli_check(
                run->corsika_run_header[0] == mli_4chars_to_float("RUNH"),
                "Expected run->corsika_run_header[0] == 'RUNH'");

        return 1;
error:
        return 0;
}

int mliTarIoRun_close(struct mliTarIoRun *run)
{
        return mliTar_close(&run->tar);
}

struct mliTarIoEvent mliTarIoEvent_init(void)
{
        struct mliTarIoEvent event;
        memset(event.corsika_event_header, 0.0, 273);
        event.photon_bunches = mliDynCorsikaPhotonBunch_init();
        return event;
}

void mliTarIoEvent_free(struct mliTarIoEvent *event)
{
        mliDynCorsikaPhotonBunch_free(&event->photon_bunches);
        (*event) = mliTarIoEvent_init();
}

int mliTarIoEvent_malloc(
        struct mliTarIoEvent *event,
        const uint64_t num_photon_bunches)
{
        mliTarIoEvent_free(event);
        mli_c(mliDynCorsikaPhotonBunch_malloc_set_size(
                &event->photon_bunches, num_photon_bunches));
        return 1;
error:
        return 0;
}

int mliTarIoEvent_malloc_from_run(
        struct mliTarIoEvent *event,
        struct mliTarIoRun *run)
{
        struct mliTarHeader info_header = mliTarHeader_init();
        struct mliTarHeader info_bunches = mliTarHeader_init();
        float tmp_evth[273];
        int rc_info_header = 0;

        const char *EVENT_HEADER_SUFFIX = ".evth.float32";
        const uint64_t PATH_NUM_DIGITS = 9;
        const uint64_t EVENT_HEADER_PATH_LENGTH =
                PATH_NUM_DIGITS + strlen(EVENT_HEADER_SUFFIX);

        const char *BUNCH_SUFFIX = ".cherenkov_bunches.Nx8_float32";
        const uint64_t BUNCH_PATH_LENGTH =
                PATH_NUM_DIGITS + strlen(BUNCH_SUFFIX);
        uint64_t evth_event_id = 0;
        uint64_t photon_bunches_event_id = 0;

        uint64_t SIZE_OF_BUNCH;
        uint64_t size_rest;
        uint64_t num_bunches;

        mliTarIoEvent_free(event);

        rc_info_header = mliTar_read_header(&run->tar, &info_header);
        if (!rc_info_header) {
                return rc_info_header;
        }

        mli_check(
                strlen(info_header.name) == EVENT_HEADER_PATH_LENGTH,
                "Expected length of evth path to be "
                "strlen('012345678.evth.float32').");

        mli_check(
                strcmp(&info_header.name[PATH_NUM_DIGITS],
                       EVENT_HEADER_SUFFIX) == 0,
                "Expected evth path to have suffix '.evth.float32'.");

        mli_check(
                mli_nstring_to_uint(
                        &evth_event_id, info_header.name, 10, PATH_NUM_DIGITS),
                "Failed to parse event-id from evth-path.");

        mli_check(
                mliTar_read_data(&run->tar, tmp_evth, 273 * sizeof(float)),
                "Failed to read evth from tar.");

        mli_check(
                tmp_evth[0] == mli_4chars_to_float("EVTH"),
                "Expected event->header[0] == 'EVTH'.");

        mli_check(
                mliTar_read_header(&run->tar, &info_bunches),
                "Failed to read tarinfo for photon-bunches from tar.");

        mli_check(
                strlen(info_bunches.name) == BUNCH_PATH_LENGTH,
                "Expected length of photon-bunches-path to be "
                "strlen('012345678.cherenkov_bunches.Nx8_float32').");

        mli_check(
                strcmp(&info_bunches.name[PATH_NUM_DIGITS], BUNCH_SUFFIX) == 0,
                "Expected photon-bunches-path to have suffix "
                "'.cherenkov_bunches.Nx8_float32'.");

        mli_check(
                mli_nstring_to_uint(
                        &photon_bunches_event_id,
                        info_bunches.name,
                        10,
                        PATH_NUM_DIGITS),
                "Failed to parse event-id from photon-bunches-path.");

        mli_check(
                photon_bunches_event_id == evth_event_id,
                "Expected event-id in evth-path == "
                "event-id in photon-bunches-path.");

        SIZE_OF_BUNCH = sizeof(struct mliCorsikaPhotonBunch);
        size_rest = info_bunches.size % SIZE_OF_BUNCH;
        num_bunches = info_bunches.size / SIZE_OF_BUNCH;

        mli_check(
                size_rest == 0,
                "Expected size of bunches to be an exact multiple of "
                "sizeof(bunch)");

        mli_c(mliTarIoEvent_malloc(event, num_bunches));

        memcpy(event->corsika_event_header, tmp_evth, 273 * sizeof(float));

        mli_check(
                mliTar_read_data(
                        &run->tar,
                        (void *)event->photon_bunches.array,
                        info_bunches.size),
                "Failed to read photon_bunches from tar.");

        return 1;
error:
        return 0;
}

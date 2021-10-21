/* Copyright 2020 Sebastian A. Mueller*/
#include "../../merlict_c89_corsika/TarIo/Event.h"
#include "../../merlict_c89_corsika/utils.h"
#include "../../merlict_c89/mliTar.h"

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
        chk(mliDynCorsikaPhotonBunch_malloc_set_size(
                &event->photon_bunches, num_photon_bunches));
        return 1;
error:
        return 0;
}

int mliTarIoEvent_malloc_from_run(
        struct mliTarIoEvent *event,
        struct mliTarIoRun *run)
{
        float tmp_evth[273];

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

        /* corsika_event_header */
        /* -------------------- */
        chk_msg(run->_has_next_head, "Expected tarinfo for evth.");
        chk_msg(strlen(run->_next_head.name) == EVENT_HEADER_PATH_LENGTH,
                "Expected length of evth path to be "
                "strlen('012345678.evth.float32').");
        chk_msg(strcmp(&run->_next_head.name[PATH_NUM_DIGITS],
                       EVENT_HEADER_SUFFIX) == 0,
                "Expected evth path to have suffix '.evth.float32'.");
        chk_msg(mli_ncstr_to_uint64(
                        &evth_event_id,
                        run->_next_head.name,
                        10,
                        PATH_NUM_DIGITS),
                "Failed to parse event-id from evth-path.");
        chk_msg(mliTar_read_data(&run->tar, tmp_evth, 273 * sizeof(float)),
                "Failed to read evth from tar.");
        chk_msg(tmp_evth[0] == mli_4chars_to_float("EVTH"),
                "Expected event->header[0] == 'EVTH'.");

        /* photon_bunches */
        /* -------------- */
        _mliTarIoRun_try_read_next_head(run);
        chk_msg(run->_has_next_head, "Expected tarinfo for cherenkov_bunches.");
        chk_msg(strlen(run->_next_head.name) == BUNCH_PATH_LENGTH,
                "Expected length of photon-bunches-path to be "
                "strlen('012345678.cherenkov_bunches.Nx8_float32').");
        chk_msg(strcmp(&run->_next_head.name[PATH_NUM_DIGITS], BUNCH_SUFFIX) ==
                        0,
                "Expected photon-bunches-path to have suffix "
                "'.cherenkov_bunches.Nx8_float32'.");
        chk_msg(mli_ncstr_to_uint64(
                        &photon_bunches_event_id,
                        run->_next_head.name,
                        10,
                        PATH_NUM_DIGITS),
                "Failed to parse event-id from photon-bunches-path.");
        chk_msg(photon_bunches_event_id == evth_event_id,
                "Expected event-id in evth-path == "
                "event-id in photon-bunches-path.");
        SIZE_OF_BUNCH = sizeof(struct mliCorsikaPhotonBunch);
        size_rest = run->_next_head.size % SIZE_OF_BUNCH;
        num_bunches = run->_next_head.size / SIZE_OF_BUNCH;
        chk_msg(size_rest == 0,
                "Expected size of bunches to be an exact multiple of "
                "sizeof(bunch)");
        chk(mliTarIoEvent_malloc(event, num_bunches));
        memcpy(event->corsika_event_header, tmp_evth, 273 * sizeof(float));
        chk_msg(mliTar_read_data(
                        &run->tar,
                        (void *)event->photon_bunches.array,
                        run->_next_head.size),
                "Failed to read photon_bunches from tar.");

        /* next */
        /* ---- */
        _mliTarIoRun_try_read_next_head(run);

        return 1;
error:
        return 0;
}

/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "../../merlict_c89_corsika/EventIo/Event.h"

struct mliEventIoEvent mliEventIoEvent_init(void)
{
        struct mliEventIoEvent evt;
        memset(evt.corsika_event_header, 0.0, 273);
        evt.telescope_offsets = mliDynEventIoTelescopeOffset_init();
        evt.photon_bunches = mliDynCorsikaPhotonBunch_init();
        return evt;
}
void mliEventIoEvent_free(struct mliEventIoEvent *evt)
{
        mliDynEventIoTelescopeOffset_free(&evt->telescope_offsets);
        mliDynCorsikaPhotonBunch_free(&evt->photon_bunches);
        (*evt) = mliEventIoEvent_init();
}

int mliEventIoEvent_malloc_from_run(
        struct mliEventIoEvent *event,
        struct mliEventIoRun *run)
{
        mliEventIoEvent_free(event);
        mli_check(
                _read_evth_273_block(run->f, event->corsika_event_header),
                "Failed to read corsika_event_header 273 float block.");
        mli_check(
                _read_telescope_offsets(
                        run->f, &event->telescope_offsets),
                "");
        mli_check(_read_telescope_array_header_1204(run->f), "");
        mli_check(
                _read_photon_bunches(run->f, &event->photon_bunches), "");
        return 1;
error:
        return 0;
}

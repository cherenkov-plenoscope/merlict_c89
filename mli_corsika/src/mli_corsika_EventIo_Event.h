/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_EVENTIO_EVENT_H_
#define MLI_CORSIKA_EVENTIO_EVENT_H_

#include <stdint.h>
#include "mli_corsika_EventIo_TelescopeOffset.h"
#include "mli_corsika_EventIo_Run.h"
#include "mli_corsika_EventIo_Telescope.h"
#include "mli_corsika_CorsikaPhotonBunch.h"

struct mliEventIoEvent {
        float corsika_event_header[273];
        float corsika_event_end[273];
        struct mliDynEventIoTelescope telescopes;
};
struct mliEventIoEvent mliEventIoEvent_init(void);
void mliEventIoEvent_free(struct mliEventIoEvent *event);
int mliEventIoEvent_malloc(
        struct mliEventIoEvent *event,
        uint64_t num_photon_bunches);
int mliEventIoEvent_malloc_from_run(
        struct mliEventIoEvent *event,
        struct mliEventIoRun *run);

#endif

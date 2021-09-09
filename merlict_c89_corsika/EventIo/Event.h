/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_EVENT_H_
#define MERLICT_C89_CORSIKA_EVENTIO_EVENT_H_

#include <stdint.h>
#include "../../merlict_c89_corsika/EventIo/TelescopeOffset.h"
#include "../../merlict_c89_corsika/EventIo/Run.h"
#include "../../merlict_c89_corsika/EventIo/Telescope.h"
#include "../../merlict_c89_corsika/mliCorsikaPhotonBunch.h"

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
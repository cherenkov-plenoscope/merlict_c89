/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_EVENT_H_
#define MERLICT_C89_CORSIKA_EVENTIO_EVENT_H_

#include <stdint.h>
#include "../../merlict_c89_corsika/EventIo/TelescopeOffset.h"
#include "../../merlict_c89_corsika/EventIo/Run.h"
#include "../../merlict_c89_corsika/mliCorsikaPhotonBunch.h"

struct mliEventIoEvent {
        float corsika_event_header[273];
        struct mliDynEventIoTelescopeOffset telescope_offsets;
        struct mliDynCorsikaPhotonBunch photon_bunches;
};
struct mliEventIoEvent mliEventIoEvent_init(void);
void mliEventIoEvent_free(struct mliEventIoEvent *ioevt);
int mliEventIoEvent_malloc(
        struct mliEventIoEvent *ioevt,
        uint64_t num_photon_bunches);
int mliEventIoEvent_malloc_from_run(
        struct mliEventIoEvent *event,
        struct mliEventIoRun *run);

#endif

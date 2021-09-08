/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_RUN_H_
#define MERLICT_C89_CORSIKA_EVENTIO_RUN_H_

#include <stdint.h>
#include "../../merlict_c89_corsika/EventIo/Header.h"
#include "../../merlict_c89_corsika/EventIo/TelescopeOffset.h"
#include "../../merlict_c89_corsika/EventIo/TelescopePosition.h"
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

struct mliEventIoRun {
        FILE *f;
        float corsika_run_header[273];
        struct mliDynStr corsika_input_card;
        struct mliDynEventIoTelescopePosition telescope_positions;
};
struct mliEventIoRun mliEventIoRun_init(void);
void mliEventIoRun_close(struct mliEventIoRun *run);
int mliEventIoRun_open(struct mliEventIoRun *run, const char *path);
int mliEventIoEvent_malloc_from_run(
        struct mliEventIoEvent *event,
        struct mliEventIoRun *run);

#endif

/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_RUN_H_
#define MERLICT_C89_CORSIKA_EVENTIO_RUN_H_

#include <stdint.h>
#include "../../merlict_c89_corsika/mliEventIoHeader.h"
#include "../../merlict_c89_corsika/mliCorsikaPhotonBunch.h"

struct mliEventIoTelPos {
        float x;
        float y;
        float z;
        float r;
};
MLIDYNARRAY_DEFINITON(mli, EventIoTelPos, struct mliEventIoTelPos)

struct mliEventIoTelOffset {
        float toff;
        float xoff;
        float yoff;
        float weight;
};
MLIDYNARRAY_DEFINITON(mli, EventIoTelOffset, struct mliEventIoTelOffset)

struct mliEventIoEvent {
        float corsika_event_header[273];
        struct mliDynEventIoTelOffset telescope_offsets;
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
        struct mliDynEventIoTelPos telescope_positions;
};
struct mliEventIoRun mliEventIoRun_init(void);
void mliEventIoRun_close(struct mliEventIoRun *run);
int mliEventIoRun_open(struct mliEventIoRun *run, const char *path);
int mliEventIoEvent_malloc_from_run(
        struct mliEventIoEvent *event,
        struct mliEventIoRun *run);

#endif

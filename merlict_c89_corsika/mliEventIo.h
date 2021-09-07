/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_H_
#define MERLICT_C89_CORSIKA_EVENTIO_H_

#include <stdint.h>
#include "mliEventIoHeader.h"
#include "mliCorsikaPhotonBunch.h"

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
struct mliEventIoEvent mliEventIoEvent_init();
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
struct mliEventIoRun mliEventIoRun_init();
int mliEventIoRun_malloc(
        struct mliEventIoRun *runstream,
        const char *path);
int mliEventIoRun_free(struct mliEventIoRun *runstream);
int mliEventIoRun_malloc_next_event(
        struct mliEventIoRun *runstream,
        struct mliEventIoEvent *eventstream);



#define MLI_CORSIKA_RUNH_RUN_NUMBER 1
#define MLI_CORSIKA_RUNH_SLOPE_OF_ENERGY_SPECTRUM 15
#define MLI_CORSIKA_RUNH_ENERGY_RANGE_START 16
#define MLI_CORSIKA_RUNH_ENERGY_RANGE_STOP 17
#define MLI_CORSIKA_RUNH_NUM_OBSERVATION_LEVELS 4

#define MLI_CORSIKA_EVTH_EVENT_NUMBER 1
#define MLI_CORSIKA_EVTH_PARTICLE_ID 2
#define MLI_CORSIKA_EVTH_ENERGY_GEV 3
#define MLI_CORSIKA_EVTH_ZENITH_RAD 10
#define MLI_CORSIKA_EVTH_AZIMUTH_RAD 11
#define MLI_CORSIKA_EVTH_FIRST_INTERACTION_HEIGHT_CM 6

#endif

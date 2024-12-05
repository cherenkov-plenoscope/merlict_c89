/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_EVENTIO_RUN_H_
#define MLI_CORSIKA_EVENTIO_RUN_H_

#include <stdint.h>
#include "../mtl/io.h"
#include "EventIo_Header.h"
#include "EventIo_TelescopePosition.h"

struct mliEventIoRun {
        FILE *f;
        struct mliEventIoHeader next_block;
        float corsika_run_header[273];
        struct mtl_IO corsika_input_card;
        struct mliDynEventIoTelescopePosition telescope_positions;
};
struct mliEventIoRun mliEventIoRun_init(void);
void mliEventIoRun_finalize(struct mliEventIoRun *run);
int mliEventIoRun_begin(struct mliEventIoRun *run, FILE *stream);
int mliEventIoRun_has_still_events_left(struct mliEventIoRun *run);
int mliEventIoRun_next_block(struct mliEventIoRun *run, const int level);
int mliEventIoRun_read_273float32_block(FILE *f, float *block);

#endif

/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_RUN_H_
#define MERLICT_C89_CORSIKA_EVENTIO_RUN_H_

#include <stdint.h>
#include "mli_corsika_EventIo_Header.h"
#include "mli_corsika_EventIo_TelescopePosition.h"

struct mliEventIoRun {
        FILE *f;
        struct mliEventIoHeader next_block;
        float corsika_run_header[273];
        struct mliDynStr corsika_input_card;
        struct mliDynEventIoTelescopePosition telescope_positions;
};
struct mliEventIoRun mliEventIoRun_init(void);
void mliEventIoRun_close(struct mliEventIoRun *run);
int mliEventIoRun_open(struct mliEventIoRun *run, const char *path);
int mliEventIoRun_has_still_events_left(struct mliEventIoRun *run);
int mliEventIoRun_next_block(struct mliEventIoRun *run, const int level);

#endif

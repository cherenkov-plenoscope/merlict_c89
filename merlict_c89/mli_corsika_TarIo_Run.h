/* Copyright 2020 Sebastian A. Mueller */
#ifndef MLI_CORSIKA_TARIO_RUN_H_
#define MLI_CORSIKA_TARIO_RUN_H_

#include "mliTar.h"

struct mliTarIoRun {
        int has_next_head;
        struct mliTarHeader next_head;
        struct mliTar tar;
        float corsika_run_header[273];
};

struct mliTarIoRun mliTarIoRun_init(void);
int mliTarIoRun_close(struct mliTarIoRun *run);
int mliTarIoRun_has_still_events_left(struct mliTarIoRun *run);
int mliTarIoRun_open(struct mliTarIoRun *run, const char *path);
void mliTarIoRun_try_read_next_head(struct mliTarIoRun *run);

#endif

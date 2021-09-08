// Copyright 2020 Sebastian A. Mueller
#ifndef MERLICT_C89_CORSIKA_MLITARIO_H_
#define MERLICT_C89_CORSIKA_MLITARIO_H_

#include "../merlict_c89/mliTar.h"
#include "../merlict_c89_corsika/mliCorsikaPhotonBunch.h"

struct mliTarIoRun {
        struct mliTar tar;
        float header[273];
};

struct mliTarIoRun mliTarIoRun_init(void);
int mliTarIoRun_close(struct mliTarIoRun *run);
int mliTarIoRun_open(struct mliTarIoRun *run, const char *path);

struct mliTarIoEvent {
        float header[273];
        struct mliDynCorsikaPhotonBunch photon_bunches;
};

struct mliTarIoEvent mliTarIoEvent_init(void);
void mliTarIoEvent_free(struct mliTarIoEvent *event);
int mliTarIoEvent_malloc(
        struct mliTarIoEvent *event,
        const uint64_t num_photon_bunches);
int mliTarIoEvent_malloc_from_run(
        struct mliTarIoEvent *event,
        struct mliTarIoRun *run);

#endif

/* Copyright 2020 Sebastian A. Mueller */
#ifndef MERLICT_C89_CORSIKA_TARIO_EVENT_H_
#define MERLICT_C89_CORSIKA_TARIO_EVENT_H_

#include "mli_corsika_TarIo_Run.h"
#include "mli_corsika_CorsikaPhotonBunch.h"

struct mliTarIoEvent {
        float corsika_event_header[273];
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

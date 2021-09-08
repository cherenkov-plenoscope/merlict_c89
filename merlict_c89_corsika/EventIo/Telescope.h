/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_TELESCOPE_H_
#define MERLICT_C89_CORSIKA_EVENTIO_TELESCOPE_H_

#include <stdint.h>
#include "../../merlict_c89/mliDynArray_template.h"
#include "../../merlict_c89_corsika/mliCorsikaPhotonBunch.h"
#include "../../merlict_c89_corsika/EventIo/TelescopeOffset.h"

struct mliEventIoTelescope {
        int16_t array_id;
        int16_t telescope_id;
        struct mliEventIoTelescopeOffset offset;
        struct mliDynCorsikaPhotonBunch photon_bunches;
};
struct mliEventIoTelescope mliEventIoTelescope_init(void);
void mliEventIoTelescope_free(struct mliEventIoTelescope *telescope);

MLIDYNARRAY_DEFINITON(mli, EventIoTelescope, struct mliEventIoTelescope)

#endif

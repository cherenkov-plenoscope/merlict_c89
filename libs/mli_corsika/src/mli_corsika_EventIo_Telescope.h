/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_EVENTIO_TELESCOPE_H_
#define MLI_CORSIKA_EVENTIO_TELESCOPE_H_

#include <stdint.h>
#include "../../mli/src/mliDynArray.h"
#include "mli_corsika_CorsikaPhotonBunch.h"
#include "mli_corsika_EventIo_TelescopeOffset.h"

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

/* Copyright 2020 Sebastian A. Mueller*/
#include "EventIo_Telescope.h"

struct mliEventIoTelescope mliEventIoTelescope_init(void)
{
        struct mliEventIoTelescope telescope;
        telescope.array_id = -1;
        telescope.telescope_id = -1;
        telescope.offset = mliEventIoTelescopeOffset_init();
        telescope.photon_bunches = mliDynCorsikaPhotonBunch_init();
        return telescope;
}

void mliEventIoTelescope_free(struct mliEventIoTelescope *telescope)
{
        mliDynCorsikaPhotonBunch_free(&telescope->photon_bunches);
        (*telescope) = mliEventIoTelescope_init();
}

MLI_ARRAY_IMPLEMENTATION(mliDynEventIoTelescope, struct mliEventIoTelescope)

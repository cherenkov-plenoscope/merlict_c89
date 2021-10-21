/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "mli_corsika_EventIo_TelescopeOffset.h"

struct mliEventIoTelescopeOffset mliEventIoTelescopeOffset_init(void)
{
        struct mliEventIoTelescopeOffset offset;
        offset.toff = 0.0;
        offset.xoff = 0.0;
        offset.yoff = 0.0;
        offset.weight = 0.0;
        return offset;
}

MLIDYNARRAY_IMPLEMENTATION(
        mli,
        EventIoTelescopeOffset,
        struct mliEventIoTelescopeOffset)

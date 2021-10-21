/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "../../merlict_c89_corsika/EventIo/TelescopeOffset.h"

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

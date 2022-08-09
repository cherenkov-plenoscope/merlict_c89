/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_EVENTIO_TELESCOPEOFFSET_H_
#define MLI_CORSIKA_EVENTIO_TELESCOPEOFFSET_H_

#include <stdint.h>
#include "mliDynArray.h"

struct mliEventIoTelescopeOffset {
        float toff;
        float xoff;
        float yoff;
        float weight;
};
struct mliEventIoTelescopeOffset mliEventIoTelescopeOffset_init(void);

MLIDYNARRAY_DEFINITON(
        mli,
        EventIoTelescopeOffset,
        struct mliEventIoTelescopeOffset)

#endif

/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_
#define MLI_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_

#include <stdint.h>
#include "../../mli/src/mliDynArray.h"

struct mliEventIoTelescopePosition {
        float x;
        float y;
        float z;
        float r;
};
MLIDYNARRAY_DEFINITON(
        mli,
        EventIoTelescopePosition,
        struct mliEventIoTelescopePosition)

#endif

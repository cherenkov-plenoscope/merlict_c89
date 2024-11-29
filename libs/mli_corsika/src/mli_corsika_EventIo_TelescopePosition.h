/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_
#define MLI_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_

#include <stdint.h>
#include "../../mtl/src/vector.h"

struct mliEventIoTelescopePosition {
        float x;
        float y;
        float z;
        float r;
};
MTL_VEC_DEFINITON(
        mli,
        EventIoTelescopePosition,
        struct mliEventIoTelescopePosition)

#endif

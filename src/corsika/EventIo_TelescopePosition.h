/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MLI_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_
#define MLI_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_

#include <stdint.h>
#include "../vector/vector.h"

struct mliEventIoTelescopePosition {
        float x;
        float y;
        float z;
        float r;
};
MLI_VECTOR_DEFINITON(
        mliDynEventIoTelescopePosition,
        struct mliEventIoTelescopePosition)

#endif

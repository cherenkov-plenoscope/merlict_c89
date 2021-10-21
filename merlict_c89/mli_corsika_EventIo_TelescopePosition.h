/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_
#define MERLICT_C89_CORSIKA_EVENTIO_TELESCOPEPOSITION_H_

#include <stdint.h>
#include "mliDynArray_template.h"

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

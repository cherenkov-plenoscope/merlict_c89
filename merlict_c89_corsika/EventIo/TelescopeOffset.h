/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIO_TELESCOPEOFFSET_H_
#define MERLICT_C89_CORSIKA_EVENTIO_TELESCOPEOFFSET_H_

#include <stdint.h>
#include "../../merlict_c89/mliDynArray_template.h"

struct mliEventIoTelescopeOffset {
        float toff;
        float xoff;
        float yoff;
        float weight;
};
MLIDYNARRAY_DEFINITON(
        mli,
        EventIoTelescopeOffset,
        struct mliEventIoTelescopeOffset)

#endif

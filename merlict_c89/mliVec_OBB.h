/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIVEC_OBB_H_
#define MERLICT_C89_MLIVEC_OBB_H_

#include <stdint.h>

#include "mliVec.h"

int mliVec_overlap_obb(
        const struct mliVec a,
        const struct mliVec obb_lower,
        const struct mliVec obb_upper);
#endif

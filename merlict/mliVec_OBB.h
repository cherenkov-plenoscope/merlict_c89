/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIVEC_OBB_H_
#define MLIVEC_OBB_H_

#include "mliVec.h"

int mliVec_overlap_obb(
        const struct mliVec a,
        const struct mliVec obb_lower,
        const struct mliVec obb_upper);
#endif

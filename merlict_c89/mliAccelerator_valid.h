/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIACCELERATOR_VALID_H_
#define MERLICT_C89_MLIACCELERATOR_VALID_H_

#include "mliAccelerator.h"

int mliAccelerator_valid(const struct mliAccelerator *accel);
int mliAccelerator_valid_wrt_Geometry(
        const struct mliAccelerator *accel,
        const struct mliGeometry *geometry);
#endif

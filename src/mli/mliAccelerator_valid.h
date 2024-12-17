/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIACCELERATOR_VALID_H_
#define MLIACCELERATOR_VALID_H_

#include "mliAccelerator.h"

int mliAccelerator_valid(const struct mliAccelerator *accel);
int mliAccelerator_valid_wrt_Geometry(
        const struct mliAccelerator *accel,
        const struct mli_Geometry *geometry);
#endif

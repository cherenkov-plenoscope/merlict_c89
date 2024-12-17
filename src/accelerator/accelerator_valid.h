/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ACCELERATOR_VALID_H_
#define MLI_ACCELERATOR_VALID_H_

#include "accelerator.h"

int mliAccelerator_valid(const struct mliAccelerator *accel);
int mliAccelerator_valid_wrt_Geometry(
        const struct mliAccelerator *accel,
        const struct mli_Geometry *geometry);
#endif

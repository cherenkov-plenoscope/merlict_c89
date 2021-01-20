/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator.h"

struct mliAccelerator mliAccelerator_init(void)
{
        struct mliAccelerator accel;
        accel.num_robjects = 0u;
        return accel;
}

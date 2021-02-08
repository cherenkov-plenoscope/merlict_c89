/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliCombine.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

struct mliCombine mliCombine_init(void)
{
        struct mliCombine combine;
        combine.scenery = mliScenery_init();
        combine.accelerator = mliAccelerator_init();
        return combine;
}

void mliCombine_free(struct mliCombine *combine)
{
        mliScenery_free(&combine->scenery);
        mliAccelerator_free(&combine->accelerator);
}

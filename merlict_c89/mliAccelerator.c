/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator.h"

struct mliAccelerator mliAccelerator_init(void)
{
        struct mliAccelerator accel;
        accel.num_robjects = 0u;
        accel.robject_obbs = NULL;
        return accel;
}

void mliAccelerator_free(struct mliAccelerator *accel)
{
        free(accel->robject_obbs);
        (*accel) = mliAccelerator_init();
}

int mliAccelerator_malloc(
        struct mliAccelerator *accel,
        const uint32_t num_robjects)
{
        mliAccelerator_free(accel);
        accel->num_robjects = num_robjects;
        mli_malloc(accel->robject_obbs, struct mliOBB, accel->num_robjects);

        return 1;
error:
        return 0;
}

int mliAccelerator_set_obbs(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery)
{
        uint32_t rob;
        mli_check(accel->num_robjects == scenery->num_robjects,
                "Expected num_robjects to be equal, but its not.");

        for (rob = 0; rob < accel->num_robjects; rob++) {

        }
        return 1;
error:
        return 0;
}

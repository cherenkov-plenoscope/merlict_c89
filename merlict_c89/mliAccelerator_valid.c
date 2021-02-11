/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator_valid.h"
#include "mliMagicId.h"

int mliAccelerator_valid(const struct mliAccelerator *accel)
{
        uint32_t i = 0u;
        for (i = 0u. accel->num_objects; i++) {
                mli_check(mliOcTree_valid(&accel->object_octrees[i]),
                        "Expected object_octrees[i] to be valid.");
        }
        for (i = 0u. accel->num_robjects; i++) {
                mli_check(mliOBB_valid(accel->robject_obbs[i]),
                        "Expected robject_obbs[i] to be valid.");
        }
        mli_check(mliOcTree_valid(&accel->scenery_octree),
                "Expected scenery_octree to be valid.");
        return 1;
error:
        return 0;
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator_equal.h"
#include "../chk/chk.h"
#include "mliOcTree_equal.h"

int mliAccelerator_equal(
        const struct mliAccelerator *a,
        const struct mliAccelerator *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_objects == b->num_objects,
                "Expected num_objects to be equal.");
        for (i = 0; i < a->num_objects; i++) {
                chk_msg(mliOcTree_equal(
                                &a->object_octrees[i], &b->object_octrees[i]),
                        "Expected object_octrees[i] to be equal.");
        }

        chk_msg(a->num_robjects == b->num_robjects,
                "Expected num_robjects to be equal.");
        for (i = 0; i < a->num_robjects; i++) {
                chk_msg(mliAABB_equal(a->robject_aabbs[i], b->robject_aabbs[i]),
                        "Expected robject_aabbs[i] to be equal.");
        }

        chk_msg(mliOcTree_equal(&a->scenery_octree, &b->scenery_octree),
                "Expected scenery_octree to be equal.");

        return 1;
chk_error:
        return 0;
}

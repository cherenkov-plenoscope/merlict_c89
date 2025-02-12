/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "accelerator_equal.h"
#include "../chk/chk.h"
#include "../octree/octree_equal.h"

mli_bool mli_Accelerator_equal(
        const struct mli_Accelerator *a,
        const struct mli_Accelerator *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_objects == b->num_objects,
                "Expected num_objects to be equal.");
        for (i = 0; i < a->num_objects; i++) {
                chk_msg(mli_OcTree_equal(
                                &a->object_octrees[i], &b->object_octrees[i]),
                        "Expected object_octrees[i] to be equal.");
        }

        chk_msg(a->num_robjects == b->num_robjects,
                "Expected num_robjects to be equal.");
        for (i = 0; i < a->num_robjects; i++) {
                chk_msg(mli_AABB_equal(
                                a->robject_aabbs[i], b->robject_aabbs[i]),
                        "Expected robject_aabbs[i] to be equal.");
        }

        chk_msg(mli_OcTree_equal(&a->scenery_octree, &b->scenery_octree),
                "Expected scenery_octree to be equal.");

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

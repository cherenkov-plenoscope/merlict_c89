/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "accelerator_valid.h"
#include "../chk/chk.h"
#include "../magicid/magicid.h"
#include "../octree/octree_valid.h"

int mli_Accelerator_valid(const struct mli_Accelerator *self)
{
        uint32_t i = 0u;
        for (i = 0u; i < self->num_objects; i++) {
                chk_msg(mli_OcTree_valid(&self->object_octrees[i]),
                        "Expected object_octrees[i] to be valid.");
        }
        for (i = 0u; i < self->num_robjects; i++) {
                chk_msg(mli_AABB_valid(self->robject_aabbs[i]),
                        "Expected robject_aabbs[i] to be valid.");
        }
        chk_msg(mli_OcTree_valid(&self->scenery_octree),
                "Expected scenery_octree to be valid.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Accelerator_valid_wrt_Geometry(
        const struct mli_Accelerator *self,
        const struct mli_Geometry *geometry)
{
        uint32_t i = 0u;
        chk_msg(self->num_objects == geometry->num_objects,
                "Expected num_objects to be equal.");
        for (i = 0u; i < self->num_objects; i++) {
                chk_msg(mli_OcTree_valid_wrt_links(
                                &self->object_octrees[i],
                                geometry->objects[i].num_faces),
                        "Expected object_octrees[i] to be valid w.r.t. "
                        "the object's num_faces.");
        }

        chk_msg(self->num_robjects == geometry->num_robjects,
                "Expected num_robjects to be equal.");
        chk_msg(mli_OcTree_valid_wrt_links(
                        &self->scenery_octree, geometry->num_robjects),
                "Expected scenery_octree to be valid w.r.t. to "
                "geometry's num_robjects.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

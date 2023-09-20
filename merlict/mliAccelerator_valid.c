/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator_valid.h"
#include "chk_debug.h"
#include "mliMagicId.h"
#include "mliOcTree_valid.h"

int mliAccelerator_valid(const struct mliAccelerator *accel)
{
        uint32_t i = 0u;
        for (i = 0u; i < accel->num_objects; i++) {
                chk_msg(mliOcTree_valid(&accel->object_octrees[i]),
                        "Expected object_octrees[i] to be valid.");
        }
        for (i = 0u; i < accel->num_robjects; i++) {
                chk_msg(mliAABB_valid(accel->robject_aabbs[i]),
                        "Expected robject_aabbs[i] to be valid.");
        }
        chk_msg(mliOcTree_valid(&accel->scenery_octree),
                "Expected scenery_octree to be valid.");
        return 1;
error:
        return 0;
}

int mliAccelerator_valid_wrt_Geometry(
        const struct mliAccelerator *accel,
        const struct mliGeometry *geometry)
{
        uint32_t i = 0u;
        chk_msg(accel->num_objects == geometry->num_objects,
                "Expected num_objects to be equal.");
        for (i = 0u; i < accel->num_objects; i++) {
                chk_msg(mliOcTree_valid_wrt_links(
                                &accel->object_octrees[i],
                                geometry->objects[i].num_faces),
                        "Expected object_octrees[i] to be valid w.r.t. "
                        "the object's num_faces.");
        }

        chk_msg(accel->num_robjects == geometry->num_robjects,
                "Expected num_robjects to be equal.");
        chk_msg(mliOcTree_valid_wrt_links(
                        &accel->scenery_octree, geometry->num_robjects),
                "Expected scenery_octree to be valid w.r.t. to "
                "geometry's num_robjects.");
        return 1;
error:
        return 0;
}

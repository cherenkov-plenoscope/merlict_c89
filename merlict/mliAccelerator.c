/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator.h"
#include "mliObject_AABB.h"

struct mliAccelerator mliAccelerator_init(void)
{
        struct mliAccelerator accel;

        accel.num_objects = 0u;
        accel.object_octrees = NULL;

        accel.num_robjects = 0u;
        accel.robject_aabbs = NULL;

        accel.scenery_octree = mliOcTree_init();

        return accel;
}

void mliAccelerator_free(struct mliAccelerator *accel)
{
        uint32_t obj;

        mliOcTree_free(&accel->scenery_octree);

        for (obj = 0; obj < accel->num_objects; obj++) {
                mliOcTree_free(&accel->object_octrees[obj]);
        }
        free(accel->object_octrees);

        free(accel->robject_aabbs);
        (*accel) = mliAccelerator_init();
}

int mliAccelerator_malloc(
        struct mliAccelerator *accel,
        const uint32_t num_objects,
        const uint32_t num_robjects)
{
        uint32_t obj, rob;
        mliAccelerator_free(accel);

        accel->num_objects = num_objects;
        chk_malloc(accel->object_octrees, struct mliOcTree, accel->num_objects);
        for (obj = 0; obj < accel->num_objects; obj++) {
                accel->object_octrees[obj] = mliOcTree_init();
        }

        accel->num_robjects = num_robjects;
        chk_malloc(accel->robject_aabbs, struct mliAABB, accel->num_robjects);
        for (rob = 0; rob < accel->num_robjects; rob++) {
                accel->robject_aabbs[rob] = mliAABB_set(
                        mliVec_init(0.0, 0.0, 0.0), mliVec_init(0.0, 0.0, 0.0));
        }

        return 1;
error:
        return 0;
}

int mliAccelerator_set_robject_aabbs(
        struct mliAccelerator *accel,
        const struct mliGeometry *geometry)
{
        uint32_t rob;
        chk_msg(accel->num_robjects == geometry->num_robjects,
                "Expected num_robjects to be equal, but its not.");

        for (rob = 0; rob < accel->num_robjects; rob++) {
                const uint32_t robject = geometry->robjects[rob];
                accel->robject_aabbs[rob] = mliObject_aabb(
                        &geometry->objects[robject],
                        mliHomTra_from_compact(geometry->robject2root[rob]));
        }
        return 1;
error:
        return 0;
}

int mliAccelerator_set_object_octrees(
        struct mliAccelerator *accel,
        const struct mliGeometry *geometry)
{
        uint32_t obj;
        chk_msg(accel->num_objects == geometry->num_objects,
                "Expected num_objects to be equal, but its not.");

        for (obj = 0; obj < accel->num_objects; obj++) {
                chk_msg(mliOcTree_malloc_from_object_wavefront(
                                &accel->object_octrees[obj],
                                &geometry->objects[obj]),
                        "Failed to setup mliOctree for object-wavefront.");
        }

        return 1;
error:
        return 0;
}

int mliAccelerator_malloc_from_Geometry(
        struct mliAccelerator *accel,
        const struct mliGeometry *geometry)
{
        struct mliAABB outermost_aabb;

        chk_msg(mliAccelerator_malloc(
                        accel, geometry->num_objects, geometry->num_robjects),
                "Failed to malloc mliAccelerator from mliGeometry's "
                "num_robjects");

        chk_msg(mliAccelerator_set_robject_aabbs(accel, geometry),
                "Failed to set AABBs of robjects.");

        chk_msg(mliAccelerator_set_object_octrees(accel, geometry),
                "Failed to setup object octrees.");

        outermost_aabb = mliAccelerator_outermost_aabb(accel);

        chk_msg(mliOcTree_malloc_from_Geometry(
                        &accel->scenery_octree, geometry, outermost_aabb),
                "Failed to set up octree across all robjects in geometry.");

        return 1;
error:
        return 0;
}

void mliAccelerator_info_fprint(FILE *f, const struct mliAccelerator *accel)
{
        uint32_t rob, i;
        fprintf(f, "accelerator\n");
        fprintf(f, "-----------\n");
        fprintf(f, "\n");
        fprintf(f, "    object-references bounding-boxes (AABB)s:\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%5s ", "ref");
        fprintf(f, "%9s ", "-x/m");
        fprintf(f, "%9s ", "-y/m");
        fprintf(f, "%9s ", "-z/m");
        fprintf(f, "%9s ", "+x/m");
        fprintf(f, "%9s ", "+y/m");
        fprintf(f, "%9s ", "+z/m");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        for (rob = 0; rob < accel->num_robjects; rob++) {
                fprintf(f, "    ");
                fprintf(f, "%5d ", rob);
                fprintf(f, "%9.1f ", accel->robject_aabbs[rob].lower.x);
                fprintf(f, "%9.1f ", accel->robject_aabbs[rob].lower.y);
                fprintf(f, "%9.1f ", accel->robject_aabbs[rob].lower.z);
                fprintf(f, "%9.1f ", accel->robject_aabbs[rob].upper.x);
                fprintf(f, "%9.1f ", accel->robject_aabbs[rob].upper.y);
                fprintf(f, "%9.1f ", accel->robject_aabbs[rob].upper.z);
                fprintf(f, "\n");
        }
}

struct mliAABB mliAccelerator_outermost_aabb(const struct mliAccelerator *accel)
{
        uint32_t rob;
        struct mliAABB aabb;
        if (accel->num_robjects == 0) {
                aabb.lower = mliVec_init(MLI_NAN, MLI_NAN, MLI_NAN);
                aabb.upper = mliVec_init(MLI_NAN, MLI_NAN, MLI_NAN);
                return aabb;
        }
        aabb.lower = accel->robject_aabbs[0].lower;
        aabb.upper = accel->robject_aabbs[0].upper;
        for (rob = 0; rob < accel->num_robjects; rob++) {
                aabb = mliAABB_outermost(aabb, accel->robject_aabbs[rob]);
        }
        return aabb;
}

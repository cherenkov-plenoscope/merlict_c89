/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator.h"
#include "mliObject_OBB.h"

struct mliAccelerator mliAccelerator_init(void)
{
        struct mliAccelerator accel;

        accel.num_objects = 0u;
        accel.object_octrees = NULL;

        accel.num_robjects = 0u;
        accel.robject_obbs = NULL;

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

        free(accel->robject_obbs);
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
        chk_malloc(accel->robject_obbs, struct mliOBB, accel->num_robjects);
        for (rob = 0; rob < accel->num_robjects; rob++) {
                accel->robject_obbs[rob] = mliOBB_set(
                        mliVec_set(0.0, 0.0, 0.0), mliVec_set(0.0, 0.0, 0.0));
        }

        return 1;
error:
        return 0;
}

int mliAccelerator_set_robject_obbs(
        struct mliAccelerator *accel,
        const struct mliGeometry *geometry)
{
        uint32_t rob;
        chk_msg(accel->num_robjects == geometry->num_robjects,
                "Expected num_robjects to be equal, but its not.");

        for (rob = 0; rob < accel->num_robjects; rob++) {
                uint32_t robject;
                robject = geometry->robjects[rob];
                accel->robject_obbs[rob] = mliObject_obb(
                        &geometry->objects[robject],
                        geometry->robject2root[rob]);
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
        struct mliOBB outermost_obb;

        chk_msg(mliAccelerator_malloc(
                        accel, geometry->num_objects, geometry->num_robjects),
                "Failed to malloc mliAccelerator from mliGeometry's "
                "num_robjects");

        chk_msg(mliAccelerator_set_robject_obbs(accel, geometry),
                "Failed to set OBBs of robjects.");

        chk_msg(mliAccelerator_set_object_octrees(accel, geometry),
                "Failed to setup object octrees.");

        outermost_obb = mliAccelerator_outermost_obb(accel);

        chk_msg(mliOcTree_malloc_from_Geometry(
                        &accel->scenery_octree, geometry, outermost_obb),
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
        fprintf(f, "    object-reference-bounding-boxes (OBB)s:\n");
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
                fprintf(f, "%9.1f ", accel->robject_obbs[rob].lower.x);
                fprintf(f, "%9.1f ", accel->robject_obbs[rob].lower.y);
                fprintf(f, "%9.1f ", accel->robject_obbs[rob].lower.z);
                fprintf(f, "%9.1f ", accel->robject_obbs[rob].upper.x);
                fprintf(f, "%9.1f ", accel->robject_obbs[rob].upper.y);
                fprintf(f, "%9.1f ", accel->robject_obbs[rob].upper.z);
                fprintf(f, "\n");
        }
}

struct mliOBB mliAccelerator_outermost_obb(const struct mliAccelerator *accel)
{
        uint32_t rob;
        struct mliOBB obb;
        if (accel->num_robjects == 0) {
                obb.lower = mliVec_set(MLI_NAN, MLI_NAN, MLI_NAN);
                obb.upper = mliVec_set(MLI_NAN, MLI_NAN, MLI_NAN);
                return obb;
        }
        obb.lower = accel->robject_obbs[0].lower;
        obb.upper = accel->robject_obbs[0].upper;
        for (rob = 0; rob < accel->num_robjects; rob++) {
                obb = mliOBB_outermost(obb, accel->robject_obbs[rob]);
        }
        return obb;
}

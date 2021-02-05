/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator.h"
#include "mliObject_OBB.h"
#include "mliCombine.h"

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
        mli_malloc(accel->object_octrees, struct mliOcTree, accel->num_objects);
        for (obj = 0; obj < accel->num_objects; obj++) {
                accel->object_octrees[obj] = mliOcTree_init();
        }

        accel->num_robjects = num_robjects;
        mli_malloc(accel->robject_obbs, struct mliOBB, accel->num_robjects);
        for (rob = 0; rob < accel->num_robjects; rob++) {
                accel->robject_obbs[rob] = mliOBB_set(
                        mliVec_set(0.0, 0.0, 0.0), mliVec_set(0.0, 0.0, 0.0));
        }

        return 1;
error:
        return 0;
}

int _mliAccelerator_set_robject_obbs(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery)
{
        uint32_t rob;
        mli_check(
                accel->num_robjects == scenery->num_robjects,
                "Expected num_robjects to be equal, but its not.");

        for (rob = 0; rob < accel->num_robjects; rob++) {
                uint32_t robject;
                robject = scenery->robjects[rob];
                accel->robject_obbs[rob] = mliObject_obb(
                        &scenery->resources.objects[robject],
                        scenery->robject2root[rob]);
        }
        return 1;
error:
        return 0;
}

int _mliAccelerator_set_object_octrees(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery)
{
        uint32_t obj;
        mli_check(
                accel->num_objects == scenery->resources.num_objects,
                "Expected num_objects to be equal, but its not.");

        for (obj = 0; obj < accel->num_objects; obj++) {
                mli_check(
                        mliOcTree_malloc_from_object_wavefront(
                                &accel->object_octrees[obj],
                                &scenery->resources.objects[obj]),
                        "Failed to setup mliOctree for object-wavefront.");
        }

        return 1;
error:
        return 0;
}

int mliAccelerator_malloc_from_scenery(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery)
{
        struct mliCombine combine;
        combine.scenery = scenery;
        combine.accelerator = accel;

        mli_check(
                mliAccelerator_malloc(
                        accel,
                        scenery->resources.num_objects,
                        scenery->num_robjects),
                "Failed to malloc mliAccelerator from mliScenery's "
                "num_robjects");

        mli_check(
                _mliAccelerator_set_robject_obbs(accel, scenery),
                "Failed to set OBBs of robjects.");

        mli_check(
                _mliAccelerator_set_object_octrees(accel, scenery),
                "Failed to setup object octrees.");

        mli_check(
                mliOcTree_malloc_from_combine(&accel->scenery_octree, &combine),
                "Failed to set up octree across all robjects in scenery.");

        return 1;
error:
        return 0;
}

void mliAccelerator_info_fprint(FILE *f, const struct mliAccelerator *accel)
{
        uint32_t rob;
        fprintf(f, "__mliAccelerator__\n");
        fprintf(f, " rob|  lx|  ly|  lz|  ux|  uy|  uz|\n");
        fprintf(f, "----|----|----|----|----|----|----|\n");
        for (rob = 0; rob < accel->num_robjects; rob++) {
                fprintf(f,
                        "% 4d|"
                        "% .1f|"
                        "% .1f|"
                        "% .1f|"

                        "% .1f|"
                        "% .1f|"
                        "% .1f|"
                        "\n",
                        rob,
                        accel->robject_obbs[rob].lower.x,
                        accel->robject_obbs[rob].lower.y,
                        accel->robject_obbs[rob].lower.z,
                        accel->robject_obbs[rob].upper.x,
                        accel->robject_obbs[rob].upper.y,
                        accel->robject_obbs[rob].upper.z);
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

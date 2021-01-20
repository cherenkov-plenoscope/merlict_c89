/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator.h"
#include "mliObject_OBB.h"

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

int _mliAccelerator_set_obbs(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery)
{
        uint32_t rob;
        mli_check(accel->num_robjects == scenery->num_robjects,
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

int mliAccelerator_malloc_from_scenery(
        struct mliAccelerator *accel,
        const struct mliScenery *scenery)
{
        mli_check(
                mliAccelerator_malloc(accel, scenery->num_robjects),
                "Failed to malloc mliAccelerator from mliScenery's "
                "num_robjects");

        mli_check(
                _mliAccelerator_set_obbs(accel, scenery),
                "Failed to set OBBs of robjects.");

        return 1;
error:
        return 0;
}

void mliAccelerator_fprint(FILE *f, const struct mliAccelerator *accel)
{
        uint32_t rob;
        fprintf(f, "__mliAccelerator__\n");
        fprintf(f, " rob|  lx|  ly|  lz|  ux|  uy|  uz|\n");
        fprintf(f, "----|----|----|----|----|----|----|\n");
        for (rob = 0; rob < accel->num_robjects; rob++) {
                fprintf(
                        f,
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

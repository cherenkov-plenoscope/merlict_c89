/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_equal.h"

int mliScenery_is_equal(const struct mliScenery *a, const struct mliScenery *b)
{
        uint64_t rob;

        mli_check(
                mliSceneryResources_equal(&a->resources, &b->resources),
                "Expected SceneryResources to be equal, but its not.");
        mli_check(
                a->num_robjects == b->num_robjects,
                "Expected num_robjects to be equal.");

        for (rob = 0; rob < a->num_robjects; rob++) {
                mli_check(
                        a->robjects[rob] == b->robjects[rob],
                        "Expected object-references to be equal.");
                mli_check(
                        a->robject_ids[rob] == b->robject_ids[rob],
                        "Expected the users object-ids to be equal.");

                mli_check(
                        a->robject_ids[rob] == b->robject_ids[rob],
                        "Expected the users object-ids to be equal.");

                mli_check(
                        mliBoundaryLayer_is_equal(
                                a->robject_boundary_layers[rob],
                                b->robject_boundary_layers[rob]),
                        "Expected boundary layers to be equal");

                mli_check(
                        mliHomTraComp_is_equal(
                                a->robject2root[rob], b->robject2root[rob]),
                        "Expected homogenous transformation of "
                        "object-references to be equal");
        }

        return 1;
error:
        return 0;
}

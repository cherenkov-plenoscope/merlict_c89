/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_valid.h"

int mliScenery_valid(const struct mliScenery *scenery)
{
        uint64_t rob;

        mli_check(
                mliSceneryResources_valid(&scenery->resources),
                "Expected resources to be valid.");

        for (rob = 0; rob < scenery->num_robjects; rob++) {

                mli_check(
                        scenery->robjects[rob] <=
                        scenery->resources.num_objects,
                        "Expected robjects to refer to valid objects.");
                /*
                *       robject_ids are set by the user and can be whatever
                *       the user wants.
                */

                mli_check(
                        scenery->robject_boundary_layers[rob].inner.surface <=
                        scenery->resources.num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner surface.");
                mli_check(
                        scenery->robject_boundary_layers[rob].outer.surface <=
                        scenery->resources.num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid outer surface.");

                mli_check(
                        scenery->robject_boundary_layers[rob].inner.medium <=
                        scenery->resources.num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner medium.");
                mli_check(
                        scenery->robject_boundary_layers[rob].outer.medium <=
                        scenery->resources.num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid outer medium.");

                /*
                *       robject homogenous transformations can be whatever
                *       they want to be.
                */
        }

        return 1;
error:
        return 0;
}

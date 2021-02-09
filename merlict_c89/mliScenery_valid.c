/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_valid.h"

int _mliScenery_valid_objects(const struct mliScenery *scenery)
{
        uint64_t i;
        for (i = 0; i < scenery->num_objects; i++) {
                if (!mliObject_assert_valid_faces(&scenery->objects[i]))
                        return 0;
        }
        return 1;
}

int _mliScenery_valid_object_references(
        const struct mliScenery *scenery,
        const struct mliMaterials *materials)
{
        uint64_t rob;
        for (rob = 0; rob < scenery->num_robjects; rob++) {

                mli_check(
                        scenery->robjects[rob] <= scenery->num_objects,
                        "Expected robjects to refer to valid objects.");
                /*
                 *       robject_ids are set by the user and can be whatever
                 *       the user wants.
                 */

                mli_check(
                        scenery->robject_boundary_layers[rob].inner.surface <=
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner surface.");
                mli_check(
                        scenery->robject_boundary_layers[rob].outer.surface <=
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid outer surface.");

                mli_check(
                        scenery->robject_boundary_layers[rob].inner.medium <=
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner medium.");
                mli_check(
                        scenery->robject_boundary_layers[rob].outer.medium <=
                                materials->num_surfaces,
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

int mliScenery_valid(
        const struct mliScenery *scenery,
        const struct mliMaterials *materials)
{
        mli_check(
                _mliScenery_valid_objects(scenery),
                "Expected objects to be valid.");
        mli_check(
                _mliScenery_valid_object_references(scenery, materials),
                "Expected object-references to be valid.");
        return 1;
error:
        return 0;
}

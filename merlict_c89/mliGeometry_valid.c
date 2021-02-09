/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_valid.h"

int _mliGeometry_valid_objects(const struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_objects; i++) {
                mli_check(mliObject_is_valid(&geometry->objects[i]),
                        "Expected object to be valid.");
        }
        return 1;
error:
        mli_log_err_vargs(("objects[%u] is invalid.", i));
        return 0;
}

int _mliGeometry_valid_object_references(
        const struct mliGeometry *geometry,
        const struct mliMaterials *materials)
{
        uint64_t rob;
        for (rob = 0; rob < geometry->num_robjects; rob++) {

                mli_check(
                        geometry->robjects[rob] <= geometry->num_objects,
                        "Expected robjects to refer to valid objects.");
                /*
                 *       robject_ids are set by the user and can be whatever
                 *       the user wants.
                 */

                mli_check(
                        geometry->robject_boundary_layers[rob].inner.surface <=
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner surface.");
                mli_check(
                        geometry->robject_boundary_layers[rob].outer.surface <=
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid outer surface.");

                mli_check(
                        geometry->robject_boundary_layers[rob].inner.medium <=
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner medium.");
                mli_check(
                        geometry->robject_boundary_layers[rob].outer.medium <=
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

int mliGeometry_valid(
        const struct mliGeometry *geometry,
        const struct mliMaterials *materials)
{
        mli_check(
                _mliGeometry_valid_objects(geometry),
                "Expected objects to be valid.");
        mli_check(
                _mliGeometry_valid_object_references(geometry, materials),
                "Expected object-references to be valid.");
        return 1;
error:
        return 0;
}

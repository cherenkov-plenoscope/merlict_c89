/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_valid.h"

int _mliGeometry_valid_objects(const struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_objects; i++) {
                mli_check(mliObject_is_valid(&geometry->objects[i]),
                        "Expected object to be valid.");
                mli_check(mliName_valid(&geometry->object_names[i]),
                        "Expected object_name to be valid.");
        }
        return 1;
error:
        mli_eprintf("objects[%u] is invalid.", i);
        return 0;
}

int _mliGeometry_valid_robjects_HomTras(const struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_robjects; i++) {
                const struct mliVec t = geometry->robject2root[i].translation;
                const struct mliQuaternion q = geometry->robject2root[i].rotation;
                mli_check(!MLI_IS_NAN(t.x), "translation.x is 'nan'.");
                mli_check(!MLI_IS_NAN(t.y), "translation.y is 'nan'.");
                mli_check(!MLI_IS_NAN(t.z), "translation.z is 'nan'.");

                mli_check(!MLI_IS_NAN(q.w), "quaternion.w is 'nan'.");
                mli_check(!MLI_IS_NAN(q.x), "quaternion.x is 'nan'.");
                mli_check(!MLI_IS_NAN(q.y), "quaternion.y is 'nan'.");
                mli_check(!MLI_IS_NAN(q.z), "quaternion.z is 'nan'.");
        }
        return 1;
error:
        mli_eprintf("robject2root[%u] is invalid.", i);
        return 0;
}

int _mliGeometry_valid_object_references(
        const struct mliGeometry *geometry,
        const struct mliMaterials *materials)
{
        uint32_t i;
        for (i = 0; i < geometry->num_robjects; i++) {

                mli_check(
                        geometry->robjects[i] < geometry->num_objects,
                        "Expected robjects to refer to valid objects.");
                /*
                 *       robject_ids are set by the user and can be whatever
                 *       the user wants.
                 */

                mli_check(
                        geometry->robject_boundary_layers[i].inner.surface <
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner surface.");
                mli_check(
                        geometry->robject_boundary_layers[i].outer.surface <
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid outer surface.");

                mli_check(
                        geometry->robject_boundary_layers[i].inner.medium <
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid inner medium.");
                mli_check(
                        geometry->robject_boundary_layers[i].outer.medium <
                                materials->num_surfaces,
                        "Expected object-reference to refer "
                        "to a valid outer medium.");
        }
        return 1;
        mli_eprintf("robject[%u] is invalid.", i);
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
                _mliGeometry_valid_robjects_HomTras(geometry),
                "Expected robject transformations to be free of 'nan'.");
        mli_check(
                _mliGeometry_valid_object_references(geometry, materials),
                "Expected object-references to be valid.");
        return 1;
error:
        return 0;
}

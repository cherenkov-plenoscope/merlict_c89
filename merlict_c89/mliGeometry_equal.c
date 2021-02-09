/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_equal.h"

int _mliGeometry_objects_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b)
{
        uint32_t i = 0u;
        mli_check(
                a->num_objects == b->num_objects,
                "Expected num_objects to be equal.");

        for (i = 0; i < a->num_objects; i++) {
                mli_check(
                        mliObject_is_equal(&a->objects[i], &b->objects[i]),
                        "Expected object to be equal.");
                mli_check(
                        mliName_is_equal(
                                &a->object_names[i],
                                &b->object_names[i]),
                        "Expected object_name to be equal.");
        }
        return 1;
error:
        mli_log_err_vargs(("object[%u] is not equal.", i));
        return 0;
}

int _mliGeometry_object_references_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b)
{
        uint64_t rob;
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


int mliGeometry_is_equal(const struct mliGeometry *a, const struct mliGeometry *b)
{
        mli_check(
                _mliGeometry_objects_equal(a, b),
                "Expected objects to be equal.");
        mli_check(
                _mliGeometry_object_references_equal(a, b),
                "Expected object-references to be equal.");
        return 1;
error:
        return 0;
}

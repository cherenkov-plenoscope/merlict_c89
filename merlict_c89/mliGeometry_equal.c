/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_equal.h"

int _mliGeometry_objects_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b)
{
        uint32_t i = 0u;
        mli_check_message(
                a->num_objects == b->num_objects,
                "Expected num_objects to be equal.");

        for (i = 0; i < a->num_objects; i++) {
                mli_check_message(
                        mliObject_equal(&a->objects[i], &b->objects[i]),
                        "Expected object to be equal.");
                mli_check_message(
                        mliName_equal(&a->object_names[i], &b->object_names[i]),
                        "Expected object_name to be equal.");
        }
        return 1;
error:
        mli_eprintf("object[%u] is not equal.", i);
        return 0;
}

int _mliGeometry_object_references_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b)
{
        uint64_t rob = 0u;
        mli_check_message(
                a->num_robjects == b->num_robjects,
                "Expected num_robjects to be equal.");

        for (rob = 0; rob < a->num_robjects; rob++) {
                mli_check_message(
                        a->robjects[rob] == b->robjects[rob],
                        "Expected object-references to be equal.");
                mli_check_message(
                        a->robject_ids[rob] == b->robject_ids[rob],
                        "Expected the users object-ids to be equal.");

                mli_check_message(
                        a->robject_ids[rob] == b->robject_ids[rob],
                        "Expected the users object-ids to be equal.");

                mli_check_message(
                        mliHomTraComp_equal(
                                a->robject2root[rob], b->robject2root[rob]),
                        "Expected homogenous transformation of "
                        "object-references to be equal");
        }
        return 1;
error:
        mli_eprintf("object_reference[%u] is not equal.", rob);
        return 0;
}

int mliGeometry_equal(const struct mliGeometry *a, const struct mliGeometry *b)
{
        mli_check_message(
                _mliGeometry_objects_equal(a, b),
                "Expected objects to be equal.");
        mli_check_message(
                _mliGeometry_object_references_equal(a, b),
                "Expected object-references to be equal.");
        return 1;
error:
        return 0;
}

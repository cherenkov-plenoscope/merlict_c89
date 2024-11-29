/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_equal.h"
#include "../../chk/src/chk.h"

int mliGeometry_objects_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_objects == b->num_objects,
                "Expected num_objects to be equal.");

        for (i = 0; i < a->num_objects; i++) {
                chk_msg(mliObject_equal(&a->objects[i], &b->objects[i]),
                        "Expected object to be equal.");
                chk_msg(mliName_equal(&a->object_names[i], &b->object_names[i]),
                        "Expected object_name to be equal.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In geometry.object[%u]\n", i);
        return 0;
}

int mliGeometry_object_references_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b)
{
        uint64_t rob = 0u;
        chk_msg(a->num_robjects == b->num_robjects,
                "Expected num_robjects to be equal.");

        for (rob = 0; rob < a->num_robjects; rob++) {
                chk_msg(a->robjects[rob] == b->robjects[rob],
                        "Expected object-references to be equal.");
                chk_msg(a->robject_ids[rob] == b->robject_ids[rob],
                        "Expected the users object-ids to be equal.");

                chk_msg(a->robject_ids[rob] == b->robject_ids[rob],
                        "Expected the users object-ids to be equal.");

                chk_msg(mliHomTraComp_equal(
                                a->robject2root[rob], b->robject2root[rob]),
                        "Expected homogenous transformation of "
                        "object-references to be equal");
        }
        return 1;
chk_error:
        fprintf(stderr, "In geometry.object_reference[%lu]\n", rob);
        return 0;
}

int mliGeometry_equal(const struct mliGeometry *a, const struct mliGeometry *b)
{
        chk_msg(mliGeometry_objects_equal(a, b),
                "Expected objects to be equal.");
        chk_msg(mliGeometry_object_references_equal(a, b),
                "Expected object-references to be equal.");
        return 1;
chk_error:
        return 0;
}

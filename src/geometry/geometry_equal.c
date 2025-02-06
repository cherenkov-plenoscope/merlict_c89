/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometry_equal.h"
#include "../chk/chk.h"

int mli_Geometry_objects_equal(
        const struct mli_Geometry *a,
        const struct mli_Geometry *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_objects == b->num_objects,
                "Expected num_objects to be equal.");

        for (i = 0; i < a->num_objects; i++) {
                chk_msg(mli_Object_equal(&a->objects[i], &b->objects[i]),
                        "Expected object to be equal.");
                chk_msg(mli_String_equal(
                                &a->object_names[i], &b->object_names[i]),
                        "Expected object_name to be equal.");
        }
        return CHK_SUCCESS;
chk_error:
        fprintf(stderr, "In geometry.object[%u]\n", i);
        return CHK_FAIL;
}

int mli_Geometry_object_references_equal(
        const struct mli_Geometry *a,
        const struct mli_Geometry *b)
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

                chk_msg(mli_HomTraComp_equal(
                                a->robject2root[rob], b->robject2root[rob]),
                        "Expected homogenous transformation of "
                        "object-references to be equal");
        }
        return CHK_SUCCESS;
chk_error:
        fprintf(stderr, "In geometry.object_reference[%lu]\n", rob);
        return CHK_FAIL;
}

int mli_Geometry_equal(
        const struct mli_Geometry *a,
        const struct mli_Geometry *b)
{
        chk_msg(mli_Geometry_objects_equal(a, b),
                "Expected objects to be equal.");
        chk_msg(mli_Geometry_object_references_equal(a, b),
                "Expected object-references to be equal.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_valid.h"
#include "chk_debug.h"

int mliGeometry_valid_objects(const struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_objects; i++) {
                chk_msg(mliObject_is_valid(&geometry->objects[i]),
                        "Expected object to be valid.");
                chk_msg(mliName_valid(&geometry->object_names[i]),
                        "Expected object_name to be valid.");
        }
        return 1;
error:
        fprintf(stderr, "In geometry.objects[%u]\n", i);
        return 0;
}

int mliGeometry_valid_robjects_HomTras(const struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_robjects; i++) {
                const struct mliVec t = geometry->robject2root[i].translation;
                const struct mliQuaternion q =
                        geometry->robject2root[i].rotation;
                chk_msg(!MLI_IS_NAN(t.x), "translation.x is 'nan'.");
                chk_msg(!MLI_IS_NAN(t.y), "translation.y is 'nan'.");
                chk_msg(!MLI_IS_NAN(t.z), "translation.z is 'nan'.");

                chk_msg(!MLI_IS_NAN(q.w), "quaternion.w is 'nan'.");
                chk_msg(!MLI_IS_NAN(q.x), "quaternion.x is 'nan'.");
                chk_msg(!MLI_IS_NAN(q.y), "quaternion.y is 'nan'.");
                chk_msg(!MLI_IS_NAN(q.z), "quaternion.z is 'nan'.");
        }
        return 1;
error:
        fprintf(stderr, "In geometry.robject2root[%u]\n", i);
        return 0;
}

int mliGeometry_valid_object_references(const struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_robjects; i++) {
                chk_msg(geometry->robjects[i] < geometry->num_objects,
                        "Expected robjects to refer to valid objects.");
                /*
                 *       robject_ids are set by the user and can be whatever
                 *       the user wants.
                 */
        }
        return 1;
        fprintf(stderr, "In geometry.robject[%u]\n", i);
error:
        return 0;
}

int mliGeometry_valid(const struct mliGeometry *geometry)
{
        chk_msg(mliGeometry_valid_objects(geometry),
                "Expected objects to be valid.");
        chk_msg(mliGeometry_valid_robjects_HomTras(geometry),
                "Expected robject transformations to be free of 'nan'.");
        chk_msg(mliGeometry_valid_object_references(geometry),
                "Expected object-references to be valid.");
        return 1;
error:
        return 0;
}

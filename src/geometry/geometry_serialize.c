/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometry_serialize.h"
#include "../object/object_serialize.h"
#include "../materials/materials_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"
#include "../string/string_serialize.h"

chk_rc mli_Geometry_from_io(struct mli_Geometry *geometry, struct mli_IO *f)
{
        uint32_t i;
        uint32_t num_objects = 0u;
        uint32_t num_robjects = 0u;
        struct mli_MagicId magic;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Geometry"));
        mli_MagicId_warn_version(&magic);

        /* payload */
        chk_IO_read(&num_objects, sizeof(uint32_t), 1u, f);
        chk_IO_read(&num_robjects, sizeof(uint32_t), 1u, f);

        chk_msg(mli_Geometry_malloc(geometry, num_objects, num_robjects),
                "Failed to malloc robjects in mli_Geometry.");

        for (i = 0; i < geometry->num_objects; i++) {
                chk_msg(mli_Object_from_io(&geometry->objects[i], f),
                        "Failed to read object into geometry.");
                chk_msg(mli_String_from_io(&geometry->object_names[i], f),
                        "Failed to read object name into geometry.");
        }

        chk_IO_read(
                geometry->robjects,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_IO_read(
                geometry->robject_ids,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_IO_read(
                geometry->robject2root,
                sizeof(struct mli_HomTraComp),
                geometry->num_robjects,
                f);

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Geometry_to_io(const struct mli_Geometry *geometry, struct mli_IO *f)
{
        uint32_t i;
        struct mli_MagicId magic;

        /* magic identifier */
        chk(mli_MagicId_set(&magic, "mli_Geometry"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        /* payload */
        chk_IO_write(&geometry->num_objects, sizeof(uint32_t), 1, f);
        chk_IO_write(&geometry->num_robjects, sizeof(uint32_t), 1, f);

        for (i = 0; i < geometry->num_objects; i++) {
                chk_msg(mli_Object_to_io(&geometry->objects[i], f),
                        "Failed to write objects.");
                chk_msg(mli_String_to_io(&geometry->object_names[i], f),
                        "Failed to write object name.");
        }

        chk_IO_write(
                geometry->robjects,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_IO_write(
                geometry->robject_ids,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_IO_write(
                geometry->robject2root,
                sizeof(struct mli_HomTraComp),
                geometry->num_robjects,
                f);

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

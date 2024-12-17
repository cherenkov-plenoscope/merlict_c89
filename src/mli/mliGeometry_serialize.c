/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_serialize.h"
#include "mliObject_serialize.h"
#include "mliMaterials_serialize.h"
#include "mliMagicId.h"
#include "../chk/chk.h"
#include "../string/string_serialize.h"

int mliGeometry_from_io(struct mliGeometry *geometry, struct mli_IO *f)
{
        uint32_t i;
        uint32_t num_objects = 0u;
        uint32_t num_robjects = 0u;
        struct mliMagicId magic;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliGeometry"));
        mliMagicId_warn_version(&magic);

        /* payload */
        chk_IO_read(&num_objects, sizeof(uint32_t), 1u, f);
        chk_IO_read(&num_robjects, sizeof(uint32_t), 1u, f);

        chk_msg(mliGeometry_malloc(geometry, num_objects, num_robjects),
                "Failed to malloc robjects in mliGeometry.");

        for (i = 0; i < geometry->num_objects; i++) {
                chk_msg(mliObject_from_io(&geometry->objects[i], f),
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

        return 1;
chk_error:
        return 0;
}

int mliGeometry_to_io(const struct mliGeometry *geometry, struct mli_IO *f)
{
        uint32_t i;
        struct mliMagicId magic;

        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliGeometry"));
        chk_IO_write(&magic, sizeof(struct mliMagicId), 1u, f);

        /* payload */
        chk_IO_write(&geometry->num_objects, sizeof(uint32_t), 1, f);
        chk_IO_write(&geometry->num_robjects, sizeof(uint32_t), 1, f);

        for (i = 0; i < geometry->num_objects; i++) {
                chk_msg(mliObject_to_io(&geometry->objects[i], f),
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

        return 1;
chk_error:
        return 0;
}

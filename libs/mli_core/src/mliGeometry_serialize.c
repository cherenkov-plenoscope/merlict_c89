/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_serialize.h"
#include "mliMaterials_serialize.h"
#include "mliMagicId.h"
#include "../../chk_debug/src/chk_debug.h"

int mliGeometry_malloc_fread(struct mliGeometry *geometry, FILE *f)
{
        uint32_t i;
        uint32_t num_objects = 0u;
        uint32_t num_robjects = 0u;
        struct mliMagicId magic;

        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliGeometry"));
        mliMagicId_warn_version(&magic);

        /* payload */
        chk_fread(&num_objects, sizeof(uint32_t), 1u, f);
        chk_fread(&num_robjects, sizeof(uint32_t), 1u, f);

        chk_msg(mliGeometry_malloc(geometry, num_objects, num_robjects),
                "Failed to malloc robjects in mliGeometry.");

        for (i = 0; i < geometry->num_objects; i++) {
                chk_msg(mliObject_malloc_fread(&geometry->objects[i], f),
                        "Failed to read object into geometry.");
        }
        chk_fread(
                geometry->object_names,
                sizeof(struct mliName),
                geometry->num_objects,
                f);

        chk_fread(
                geometry->robjects,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_fread(
                geometry->robject_ids,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_fread(
                geometry->robject2root,
                sizeof(struct mliHomTraComp),
                geometry->num_robjects,
                f);

        return 1;
error:
        return 0;
}

int mliGeometry_fwrite(const struct mliGeometry *geometry, FILE *f)
{
        uint32_t i;
        struct mliMagicId magic;

        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliGeometry"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* payload */
        chk_fwrite(&geometry->num_objects, sizeof(uint32_t), 1, f);
        chk_fwrite(&geometry->num_robjects, sizeof(uint32_t), 1, f);

        for (i = 0; i < geometry->num_objects; i++) {
                chk_msg(mliObject_fwrite(&geometry->objects[i], f),
                        "Failed to write objects.");
        }
        chk_fwrite(
                geometry->object_names,
                sizeof(struct mliName),
                geometry->num_objects,
                f);

        chk_fwrite(
                geometry->robjects,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_fwrite(
                geometry->robject_ids,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        chk_fwrite(
                geometry->robject2root,
                sizeof(struct mliHomTraComp),
                geometry->num_robjects,
                f);

        return 1;
error:
        return 0;
}

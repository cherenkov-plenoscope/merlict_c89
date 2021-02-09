/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_serialize.h"
#include "mliMaterials_serialize.h"
#include "mliMagicId.h"

int mliGeometry_malloc_fread(struct mliGeometry *geometry, FILE *f)
{
        uint32_t i;
        uint32_t num_objects = 0u;
        uint32_t num_robjects = 0u;
        struct mliMagicId magic;

        /* magic identifier */
        mli_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliGeometry"));
        mliMagicId_warn_version(&magic);

        /* payload */
        mli_fread(&num_objects, sizeof(uint32_t), 1u, f);
        mli_fread(&num_robjects, sizeof(uint32_t), 1u, f);

        mli_check(
                mliGeometry_malloc(geometry, num_objects, num_robjects),
                "Failed to malloc robjects in mliGeometry.");

        for (i = 0; i < geometry->num_objects; i++) {
                mli_check(
                        mliObject_malloc_fread(&geometry->objects[i], f),
                        "Failed to read object into geometry.");
        }

        mli_fread(
                geometry->robjects, sizeof(uint32_t), geometry->num_robjects, f);
        mli_fread(
                geometry->robject_ids,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        mli_fread(
                geometry->robject_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                geometry->num_robjects,
                f);
        mli_fread(
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
        mli_c(mliMagicId_set(&magic, "mliGeometry"));
        mli_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* payload */
        mli_write_type(uint32_t, geometry->num_objects, f);
        mli_write_type(uint32_t, geometry->num_robjects, f);

        for (i = 0; i < geometry->num_objects; i++) {
                mli_check(mliObject_fwrite(&geometry->objects[i], f),
                        "Failed to write objects.");
        }
        mli_fwrite(
                geometry->robjects, sizeof(uint32_t), geometry->num_robjects, f);
        mli_fwrite(
                geometry->robject_ids,
                sizeof(uint32_t),
                geometry->num_robjects,
                f);
        mli_fwrite(
                geometry->robject_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                geometry->num_robjects,
                f);
        mli_fwrite(
                geometry->robject2root,
                sizeof(struct mliHomTraComp),
                geometry->num_robjects,
                f);

        return 1;
error:
        return 0;
}

int mliGeometry_malloc_from_path(struct mliGeometry *geometry, const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        mli_check(f != NULL, "Can not open geometry-file for reading.");
        mli_check(
                mliGeometry_malloc_fread(geometry, f),
                "Can not read geometry-file.");
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        mliGeometry_free(geometry);
        return 0;
}

int mliGeometry_write_to_path(const struct mliGeometry *geometry, const char *path)
{
        FILE *f;
        f = fopen(path, "w");
        mli_check(f != NULL, "Can not open geometry-file for writing.");
        mli_check(
                mliGeometry_fwrite(geometry, f),
                "Failed to write to file.");
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}

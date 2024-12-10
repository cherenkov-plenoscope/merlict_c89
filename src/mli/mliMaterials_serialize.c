/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_serialize.h"
#include "mliMedium_serialize.h"
#include "mliMagicId.h"
#include "../chk/chk.h"
#include "../string/string_serialize.h"

int mliMaterials_fwrite(const struct mliMaterials *res, FILE *f)
{
        uint64_t i;
        struct mliMagicId magic = mliMagicId_init();

        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliMaterials"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_fwrite(&res->num_media, sizeof(uint64_t), 1u, f);
        chk_fwrite(&res->num_surfaces, sizeof(uint64_t), 1u, f);
        chk_fwrite(&res->num_boundary_layers, sizeof(uint64_t), 1u, f);

        for (i = 0; i < res->num_media; i++) {
                chk(mli_String_fwrite(&res->medium_names[i], f));
                chk(mliMedium_fwrite(&res->media[i], f));
        }
        for (i = 0; i < res->num_surfaces; i++) {
                chk(mli_String_fwrite(&res->surface_names[i], f));
                chk(mliSurface_fwrite(&res->surfaces[i], f));
        }
        for (i = 0; i < res->num_boundary_layers; i++) {
                chk(mli_String_fwrite(&res->boundary_layer_names[i], f));
                chk_fwrite(
                        &res->boundary_layers[i],
                        sizeof(struct mliBoundaryLayer),
                        1,
                        f);
        }
        chk_fwrite(&res->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

int mliMaterials_malloc_fread(struct mliMaterials *res, FILE *f)
{
        uint64_t i;
        struct mliMagicId magic;
        struct mliMaterialsCapacity cap;

        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliMaterials"));
        mliMagicId_warn_version(&magic);

        chk_fread(&cap.num_media, sizeof(uint64_t), 1u, f);
        chk_fread(&cap.num_surfaces, sizeof(uint64_t), 1u, f);
        chk_fread(&cap.num_boundary_layers, sizeof(uint64_t), 1u, f);

        chk(mliMaterials_malloc(res, cap));

        /* payload */
        for (i = 0; i < res->num_media; i++) {
                chk_dbg chk_msg(
                        mli_String_malloc_fread(&res->medium_names[i], f),
                        "Failed to fread medium name.");
                chk_msg(mliMedium_malloc_fread(&res->media[i], f),
                        "Failed to fread Medium.");
        }
        for (i = 0; i < res->num_surfaces; i++) {
                chk_msg(mli_String_malloc_fread(&res->surface_names[i], f),
                        "Failed to fread surface name.");
                chk_msg(mliSurface_malloc_fread(&res->surfaces[i], f),
                        "Failed to fread Surface.");
        }
        for (i = 0; i < res->num_boundary_layers; i++) {
                chk_msg(mli_String_malloc_fread(
                                &res->boundary_layer_names[i], f),
                        "Failed to fread boundary layer name.");
                chk_fread(
                        &res->boundary_layers[i],
                        sizeof(struct mliBoundaryLayer),
                        1,
                        f);
        }
        chk_fread(&res->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

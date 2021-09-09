/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_serialize.h"
#include "mliMagicId.h"

int mliMaterials_fwrite(const struct mliMaterials *res, FILE *f)
{
        uint64_t i;
        struct mliMagicId magic = mliMagicId_init();

        /* magic identifier */
        mli_c(mliMagicId_set(&magic, "mliMaterials"));
        mli_check_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        mli_check_fwrite(&res->num_functions, sizeof(uint32_t), 1u, f);
        mli_check_fwrite(&res->num_colors, sizeof(uint32_t), 1u, f);
        mli_check_fwrite(&res->num_media, sizeof(uint32_t), 1u, f);
        mli_check_fwrite(&res->num_surfaces, sizeof(uint32_t), 1u, f);
        mli_check_fwrite(&res->num_boundary_layers, sizeof(uint32_t), 1u, f);

        for (i = 0; i < res->num_functions; i++) {
                mliFunc_fwrite(&res->functions[i], f);
        }
        mli_check_fwrite(
                res->function_names,
                sizeof(struct mliName),
                res->num_functions,
                f);

        mli_check_fwrite(res->colors, sizeof(struct mliColor), res->num_colors, f);
        mli_check_fwrite(
                res->color_names, sizeof(struct mliName), res->num_colors, f);

        mli_check_fwrite(res->media, sizeof(struct mliMedium), res->num_media, f);
        mli_check_fwrite(
                res->medium_names, sizeof(struct mliName), res->num_media, f);

        mli_check_fwrite(
                res->surfaces, sizeof(struct mliSurface), res->num_surfaces, f);
        mli_check_fwrite(
                res->surface_names,
                sizeof(struct mliName),
                res->num_surfaces,
                f);

        mli_check_fwrite(
                res->boundary_layers,
                sizeof(struct mliBoundaryLayer),
                res->num_boundary_layers,
                f);
        mli_check_fwrite(
                res->boundary_layer_names,
                sizeof(struct mliName),
                res->num_boundary_layers,
                f);
        return 1;
error:
        return 0;
}

int mliMaterials_malloc_fread(struct mliMaterials *res, FILE *f)
{
        uint64_t i;
        struct mliMagicId magic;
        struct mliMaterialsCapacity cap;

        /* magic identifier */
        mli_check_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliMaterials"));
        mliMagicId_warn_version(&magic);

        mli_check_fread(&cap.num_functions, sizeof(uint32_t), 1u, f);
        mli_check_fread(&cap.num_colors, sizeof(uint32_t), 1u, f);
        mli_check_fread(&cap.num_media, sizeof(uint32_t), 1u, f);
        mli_check_fread(&cap.num_surfaces, sizeof(uint32_t), 1u, f);
        mli_check_fread(&cap.num_boundary_layers, sizeof(uint32_t), 1u, f);

        mli_c(mliMaterials_malloc(res, cap));

        /* payload */
        for (i = 0; i < res->num_functions; i++) {
                mli_c(mliFunc_malloc_fread(&res->functions[i], f));
        }
        mli_check_fread(
                res->function_names,
                sizeof(struct mliName),
                res->num_functions,
                f);

        mli_check_fread(res->colors, sizeof(struct mliColor), res->num_colors, f);
        mli_check_fread(res->color_names, sizeof(struct mliName), res->num_colors, f);

        mli_check_fread(res->media, sizeof(struct mliMedium), res->num_media, f);
        mli_check_fread(res->medium_names, sizeof(struct mliName), res->num_media, f);

        mli_check_fread(
                res->surfaces, sizeof(struct mliSurface), res->num_surfaces, f);
        mli_check_fread(
                res->surface_names,
                sizeof(struct mliName),
                res->num_surfaces,
                f);

        mli_check_fread(
                res->boundary_layers,
                sizeof(struct mliBoundaryLayer),
                res->num_boundary_layers,
                f);
        mli_check_fread(
                res->boundary_layer_names,
                sizeof(struct mliName),
                res->num_boundary_layers,
                f);
        return 1;
error:
        return 0;
}

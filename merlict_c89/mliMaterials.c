/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials.h"
#include "mli_debug.h"

struct mliMaterialsCapacity mliMaterialsCapacity_init(void)
{
        struct mliMaterialsCapacity cap;
        cap.num_functions = 0;
        cap.num_colors = 0;
        cap.num_surfaces = 0;
        cap.num_media = 0;
        return cap;
}

struct mliMaterials mliMaterials_init(void)
{
        struct mliMaterials res;
        res.default_medium = 0u;

        res.num_functions = 0u;
        res.functions = NULL;
        res.function_names = NULL;

        res.num_colors = 0u;
        res.colors = NULL;
        res.color_names = NULL;

        res.num_media = 0u;
        res.media = NULL;
        res.medium_names = NULL;

        res.num_surfaces = 0u;
        res.surfaces = NULL;
        res.surface_names = NULL;
        return res;
}

void mliMaterials_free(struct mliMaterials *res)
{
        uint64_t i;
        for (i = 0; i < res->num_functions; i++) {
                mliFunc_free(&(res->functions[i]));
        }
        free(res->functions);
        free(res->function_names);

        free(res->colors);
        free(res->color_names);

        free(res->media);
        free(res->medium_names);

        free(res->surfaces);
        free(res->surface_names);

        (*res) = mliMaterials_init();
}

int mliMaterials_malloc(
        struct mliMaterials *res,
        const struct mliMaterialsCapacity rescap)
{
        uint64_t i;
        mliMaterials_free(res);
        res->num_functions = rescap.num_functions;
        res->num_colors = rescap.num_colors;
        res->num_surfaces = rescap.num_surfaces;
        res->num_media = rescap.num_media;

        mli_malloc(res->functions, struct mliFunc, res->num_functions);
        mli_malloc(res->function_names, struct mliName, res->num_functions);
        for (i = 0; i < res->num_functions; i++) {
                res->functions[i] = mliFunc_init();
                res->function_names[i] = mliName_init();
        }

        mli_malloc(res->colors, struct mliColor, res->num_colors);
        mli_malloc(res->color_names, struct mliName, res->num_colors);
        for (i = 0; i < res->num_colors; i++) {
                res->color_names[i] = mliName_init();
        }

        mli_malloc(res->media, struct mliMedium, res->num_media);
        mli_malloc(res->medium_names, struct mliName, res->num_media);
        for (i = 0; i < res->num_media; i++) {
                res->medium_names[i] = mliName_init();
        }

        mli_malloc(res->surfaces, struct mliSurface, res->num_surfaces);
        mli_malloc(res->surface_names, struct mliName, res->num_surfaces);
        for (i = 0; i < res->num_surfaces; i++) {
                res->surface_names[i] = mliName_init();
        }

        return 1;
error:
        mliMaterials_free(res);
        return 0;
}

void mliMaterials_info_fprint(FILE *f, const struct mliMaterials *res)
{
        fprintf(f, "__mliMaterials__\n");
        fprintf(f, "- default_medium: %d\n", res->default_medium);
}

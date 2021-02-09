/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_equal.h"
#include "mli_debug.h"

int _mliMaterials_functions_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        mli_c(a->num_functions == b->num_functions);
        for (i = 0; i < a->num_functions; i++) {
                mli_c(mliFunc_is_equal(a->functions[i], b->functions[i]));
                mli_c(mliName_is_equal(
                        &a->function_names[i],
                        &b->function_names[i]));
        }
        return 1;
error:
        mli_log_err_vargs(("functions[%u] is not equal.", i));
        return 0;
}

int _mliMaterials_colors_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        mli_c(a->num_colors == b->num_colors);
        for (i = 0; i < a->num_colors; i++) {
                mli_c(mliColor_is_equal(a->colors[i], b->colors[i]));
                mli_c(mliName_is_equal(&a->color_names[i], &b->color_names[i]));
        }
        return 1;
error:
        mli_log_err_vargs(("colors[%u] is not equal.", i));
        return 0;
}

int _mliMaterials_media_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        mli_c(a->num_media == b->num_media);
        for (i = 0; i < a->num_media; i++) {
                mli_c(mliMedium_is_equal(a->media[i], b->media[i]));
                mli_c(mliName_is_equal(
                        &a->medium_names[i],
                        &b->medium_names[i]));
        }
        return 1;
error:
        mli_log_err_vargs(("media[%u] is not equal.", i));
        return 0;
}

int _mliMaterials_surfaces_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        mli_c(a->num_surfaces == b->num_surfaces);
        for (i = 0; i < a->num_surfaces; i++) {
                mli_c(mliSurface_is_equal(a->surfaces[i], b->surfaces[i]));
                mli_c(mliName_is_equal(
                        &a->surface_names[i],
                        &b->surface_names[i]));
        }
        return 1;
error:
        mli_log_err_vargs(("surfaces[%u] is not equal.", i));
        return 0;
}

int mliMaterials_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        mli_c(a->default_medium == b->default_medium);
        mli_c(_mliMaterials_functions_equal(a, b));
        mli_c(_mliMaterials_colors_equal(a, b));
        mli_c(_mliMaterials_media_equal(a, b));
        mli_c(_mliMaterials_surfaces_equal(a, b));
        return 1;
error:
        return 0;
}

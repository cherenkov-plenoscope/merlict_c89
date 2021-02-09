/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_equal.h"

int _mliMaterials_functions_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint64_t i;
        if (a->num_functions != b->num_functions)
                return 0;
        for (i = 0; i < a->num_functions; i++) {
                if (!mliFunc_is_equal(a->functions[i], b->functions[i]))
                        return 0;
        }
        return 1;
}

int _mliMaterials_colors_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint64_t i;
        if (a->num_colors != b->num_colors)
                return 0;
        for (i = 0; i < a->num_colors; i++) {
                if (!mliColor_is_equal(a->colors[i], b->colors[i]))
                        return 0;
        }
        return 1;
}

int _mliMaterials_media_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint64_t i;
        if (a->num_media != b->num_media)
                return 0;
        for (i = 0; i < a->num_media; i++) {
                if (!mliMedium_is_equal(a->media[i], b->media[i]))
                        return 0;
        }
        return 1;
}

int _mliMaterials_surfaces_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint64_t i;
        if (a->num_surfaces != b->num_surfaces)
                return 0;
        for (i = 0; i < a->num_surfaces; i++) {
                if (!mliSurface_is_equal(a->surfaces[i], b->surfaces[i]))
                        return 0;
        }
        return 1;
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

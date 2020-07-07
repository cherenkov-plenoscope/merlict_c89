/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSurface.h"

int mliSurface_is_equal(const struct mliSurface a, const struct mliSurface b)
{
        if (a.material != b.material)
                return 0;
        if (a.specular_reflection != b.specular_reflection)
                return 0;
        if (a.diffuse_reflection != b.diffuse_reflection)
                return 0;
        if (a.color != b.color)
                return 0;
        return 1;
}

int mli_material_to_string(const uint32_t type, char *s)
{
        switch (type) {
        case MLI_MATERIAL_PHONG:
                sprintf(s, "Phong");
                break;
        case MLI_MATERIAL_TRANSPARENT:
                sprintf(s, "transparent");
                break;
        default:
                mli_sentinel("material is unknown.");
        }
        return 1;
error:
        return 0;
}

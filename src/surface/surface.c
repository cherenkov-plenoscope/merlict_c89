/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "surface.h"
#include "../magicid/magicid.h"
#include "../func/func_serialize.h"
#include "../chk/chk.h"

struct mli_Surface mli_Surface_init(void)
{
        struct mli_Surface surface;
        surface.material = MLI_SURFACE_PHONG;
        surface.specular_reflection = mli_Func_init();
        surface.diffuse_reflection = mli_Func_init();
        return surface;
}

void mli_Surface_free(struct mli_Surface *surface)
{
        mli_Func_free(&surface->specular_reflection);
        mli_Func_free(&surface->diffuse_reflection);
        (*surface) = mli_Surface_init();
}

int mli_Surface_malloc(
        struct mli_Surface *surface,
        const uint32_t num_points_specular_reflection,
        const uint32_t num_points_diffuse_reflection)
{
        mli_Surface_free(surface);
        chk(mli_Func_malloc(
                &surface->specular_reflection, num_points_specular_reflection));
        chk(mli_Func_malloc(
                &surface->diffuse_reflection, num_points_diffuse_reflection));
        return 1;
chk_error:
        return 0;
}

int mli_Surface_equal(const struct mli_Surface *a, const struct mli_Surface *b)
{
        if (a->material != b->material)
                return 0;
        if (!mli_Func_equal(a->specular_reflection, b->specular_reflection))
                return 0;
        if (!mli_Func_equal(a->diffuse_reflection, b->diffuse_reflection))
                return 0;
        return 1;
}

int mli_material_type_to_string(const uint32_t type, struct mli_String *s)
{
        switch (type) {
        case MLI_SURFACE_PHONG:
                chk(mli_String_from_cstr(s, "Phong"));
                break;
        case MLI_SURFACE_TRANSPARENT:
                chk(mli_String_from_cstr(s, "transparent"));
                break;
        default:
                chk_bad("material-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_material_type_from_string(const struct mli_String *s, uint32_t *id)
{
        if (mli_String_equal_cstr(s, "Phong")) {
                (*id) = MLI_SURFACE_PHONG;
                return 1;
        } else if (mli_String_equal_cstr(s, "transparent")) {
                (*id) = MLI_SURFACE_TRANSPARENT;
                return 1;
        } else {
                chk_bad("material-type-string is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

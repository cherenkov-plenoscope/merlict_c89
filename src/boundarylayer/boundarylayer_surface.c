/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface.h"
#include "../chk/chk.h"

struct mli_BoundaryLayer_Surface mli_BoundaryLayer_Surface_init(void)
{
        struct mli_BoundaryLayer_Surface out;
        out.type = MLI_BOUNDARYLAYER_SURFACE_TYPE_NONE;
        return out;
}

int mli_BoundaryLayer_Surface_equal(
        const struct mli_BoundaryLayer_Surface *a,
        const struct mli_BoundaryLayer_Surface *b)
{
        chk_msg(a->type == b->type, "Different types of surface models.");

        switch (a->type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                chk_msg(mli_BoundaryLayer_Surface_Phong_equal(
                                &a->data.phong, &b->data.phong),
                        "'phong' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Surface_Transparent_equal(
                                &a->data.transparent, &b->data.transparent),
                        "'transparent' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN:
                chk_msg(mli_BoundaryLayer_Surface_Lambertian_equal(
                                &a->data.lambertian, &b->data.lambertian),
                        "'lambertian' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR:
                chk_msg(mli_BoundaryLayer_Surface_Mirror_equal(
                                &a->data.mirror, &b->data.mirror),
                        "'mirror' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE:
                chk_msg(mli_BoundaryLayer_Surface_Cook_Torrance_equal(
                                &a->data.cook_torrance, &b->data.cook_torrance),
                        "'cook-torrance' surfaces are not equal.");
                break;
        default:
                chk_bad("surface-type-id is unknown.");
        }

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_type_to_string(
        const uint64_t type,
        struct mli_String *s)
{
        switch (type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                chk(mli_String_from_cstr(s, "phong"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT:
                chk(mli_String_from_cstr(s, "transparent"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN:
                chk(mli_String_from_cstr(s, "lambertian"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR:
                chk(mli_String_from_cstr(s, "mirror"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE:
                chk(mli_String_from_cstr(s, "cook-torrance"));
                break;
        default:
                chk_bad("surface-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_type_from_string(
        const struct mli_String *s,
        uint64_t *id)
{
        if (mli_String_equal_cstr(s, "phong")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG;
                return 1;
        } else if (mli_String_equal_cstr(s, "transparent")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT;
                return 1;
        } else if (mli_String_equal_cstr(s, "lambertian")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN;
                return 1;
        } else if (mli_String_equal_cstr(s, "mirror")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR;
                return 1;
        } else if (mli_String_equal_cstr(s, "cook-torrance")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE;
                return 1;
        } else {
                chk_bad("surface-type-string is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "surface_type.h"
#include "../string/string.h"

chk_rc mli_Surface_type_to_string(const uint64_t type, struct mli_String *s)
{
        switch (type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk(mli_String_from_cstr(s, "transparent"));
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk(mli_String_from_cstr(s, "cook-torrance"));
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", type));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Surface_type_from_string(const struct mli_String *s, uint64_t *id)
{
        if (mli_String_equal_cstr(s, "transparent")) {
                (*id) = MLI_SURFACE_TYPE_TRANSPARENT;
                return CHK_SUCCESS;
        } else if (mli_String_equal_cstr(s, "cook-torrance")) {
                (*id) = MLI_SURFACE_TYPE_COOKTORRANCE;
                return CHK_SUCCESS;
        } else {
                chk_badf(("surface-type-string '%s' is unknown.", s->array));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

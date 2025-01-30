/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_medium.h"

struct mli_BoundaryLayer_Medium mli_BoundaryLayer_Medium_init(void)
{
        struct mli_BoundaryLayer_Medium out;
        out.type = MLI_BOUNDARYLAYER_MEDIUM_TYPE_NONE;
        return out;
}

int mli_BoundaryLayer_Medium_type_to_string(
        const uint64_t type,
        struct mli_String *s)
{
        switch (type) {
        case MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT:
                chk(mli_String_from_cstr(s, "transparent"));
                break;
        default:
                chk_bad("medium-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_type_from_string(
        const struct mli_String *s,
        uint64_t *id)
{
        if (mli_String_equal_cstr(s, "transparent")) {
                (*id) = MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT;
                return 1;
        } else {
                chk_bad("medium-type-string is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

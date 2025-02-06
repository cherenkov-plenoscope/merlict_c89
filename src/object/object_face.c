/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "object_face.h"

struct mli_object_Face mli_object_Face_set(
        const uint32_t a,
        const uint32_t b,
        const uint32_t c)
{
        struct mli_object_Face face;
        face.a = a;
        face.b = b;
        face.c = c;
        return face;
}

mli_bool mli_object_Face_equal(
        const struct mli_object_Face a,
        const struct mli_object_Face b)
{
        if (a.a != b.a)
                return MLI_FALSE;
        if (a.b != b.b)
                return MLI_FALSE;
        if (a.c != b.c)
                return MLI_FALSE;
        return MLI_TRUE;
}

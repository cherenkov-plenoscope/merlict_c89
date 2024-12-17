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

int mli_object_Face_equal(
        const struct mli_object_Face a,
        const struct mli_object_Face b)
{
        if (a.a != b.a)
                return 0;
        if (a.b != b.b)
                return 0;
        if (a.c != b.c)
                return 0;
        return 1;
}

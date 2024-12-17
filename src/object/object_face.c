/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "object_face.h"

struct mliFace mliFace_set(const uint32_t a, const uint32_t b, const uint32_t c)
{
        struct mliFace face;
        face.a = a;
        face.b = b;
        face.c = c;
        return face;
}

int mliFace_equal(const struct mliFace a, const struct mliFace b)
{
        if (a.a != b.a)
                return 0;
        if (a.b != b.b)
                return 0;
        if (a.c != b.c)
                return 0;
        return 1;
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_FACE_H_
#define MLI_OBJECT_FACE_H_

#include <stdint.h>
#include "../bool/bool.h"

struct mli_object_Face {
        uint32_t a;
        uint32_t b;
        uint32_t c;
};

mli_bool mli_object_Face_equal(
        const struct mli_object_Face a,
        const struct mli_object_Face b);
struct mli_object_Face mli_object_Face_set(
        const uint32_t a,
        const uint32_t b,
        const uint32_t c);
#endif

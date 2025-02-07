/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_TYPE_H_
#define MLI_SURFACE_TYPE_H_

#include <stdint.h>
#include "../chk/chk.h"
struct mli_String;

enum mli_surface_type {
        MLI_SURFACE_TYPE_NONE = 0,
        MLI_SURFACE_TYPE_TRANSPARENT = 1000,
        MLI_SURFACE_TYPE_COOKTORRANCE = 5000
};

chk_rc mli_Surface_type_to_string(const uint64_t type, struct mli_String *s);
chk_rc mli_Surface_type_from_string(const struct mli_String *s, uint64_t *id);

#endif

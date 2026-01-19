/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_ARRAY_H_
#define MLI_SURFACE_ARRAY_H_

#include "../array/array.h"
#include "surface.h"
MLI_ARRAY_DEFINITON(mli_SurfaceArray, struct mli_Surface)

mli_bool mli_SurfaceArray_has_name_cstr(
        const struct mli_SurfaceArray *self,
        const char *name);
int64_t mli_SurfaceArray_get_index_by_name_cstr(
        const struct mli_SurfaceArray *self,
        const char *name);

#endif

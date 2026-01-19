/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "surface_array.h"
#include "../string/string.h"

MLI_ARRAY_IMPLEMENTATION_FREE(
        mli_SurfaceArray,
        struct mli_Surface,
        mli_Surface_free)

mli_bool mli_SurfaceArray_has_name_cstr(
        const struct mli_SurfaceArray *self,
        const char *name)
{
        int64_t i = mli_SurfaceArray_get_index_by_name_cstr(self, name);
        if (i >= 0) {
                return MLI_TRUE;
        } else {
                return MLI_FALSE;
        }
}

int64_t mli_SurfaceArray_get_index_by_name_cstr(
        const struct mli_SurfaceArray *self,
        const char *name)
{
        uint64_t i;
        for (i = 0; i < self->size; i++) {
                if (mli_String_equal_cstr(&self->array[i].name, name)) {
                        return (int64_t)i;
                }
        }
        return -1;
}

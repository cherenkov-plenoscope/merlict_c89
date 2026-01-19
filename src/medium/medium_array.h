/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_MEDIUM_ARRAY_H_
#define MLI_MEDIUM_ARRAY_H_

#include "../array/array.h"
#include "medium.h"
MLI_ARRAY_DEFINITON(mli_MediumArray, struct mli_Medium)

mli_bool mli_MediumArray_has_name_cstr(
        const struct mli_MediumArray *self,
        const char *name);
int64_t mli_MediumArray_get_index_by_name_cstr(
        const struct mli_MediumArray *self,
        const char *name);

#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliName.h"
#include <string.h>

int mliName_find_idx(
        const struct mli_String *names,
        const uint64_t num_names,
        const char *key,
        uint64_t *idx)
{
        uint64_t i;
        (*idx) = 0u;
        for (i = 0; i < num_names; i++) {
                if (mli_String_equal_cstr(&names[i], key)) {
                        (*idx) = i;
                        return 1;
                }
        }
        return 0;
}

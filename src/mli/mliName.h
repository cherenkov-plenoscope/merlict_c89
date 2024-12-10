/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLINAME_H_
#define MLINAME_H_

#include <stdint.h>
#include "../string/string.h"

int mliName_find_idx(
        const struct mli_String *names,
        const uint64_t num_names,
        const char *key,
        uint64_t *idx);
#endif

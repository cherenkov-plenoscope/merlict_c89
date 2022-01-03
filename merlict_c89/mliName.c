/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliName.h"

struct mliName mliName_init(void)
{
        struct mliName name;
        memset(name.cstr, '\0', sizeof(name.cstr));
        return name;
}

int mliName_valid(const struct mliName *name)
{
        if (name->cstr[sizeof(name->cstr) - 1] == '\0') {
                return 1;
        } else {
                return 0;
        }
}

int mliName_equal(const struct mliName *a, const struct mliName *b)
{
        uint32_t i;
        for (i = 0; i < sizeof(a->cstr); i++) {
                if (a->cstr[i] != b->cstr[i]) {
                        return 0;
                }
        }
        return 1;
}

int mliName_find_idx(
        const struct mliName *names,
        const uint64_t num_names,
        const char *key,
        uint64_t *idx)
{
        uint64_t i;
        (*idx) = 0u;
        for (i = 0; i < num_names; i++) {
                if (strcmp(names[i].cstr, key) == 0) {
                        (*idx) = i;
                        return 1;
                }
        }
        return 0;
}

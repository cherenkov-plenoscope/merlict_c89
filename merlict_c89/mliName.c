/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliName.h"
#include "mli_debug.h"

struct mliName mliName_init(void)
{
        struct mliName name;
        memset(name.c_str, '\0', sizeof(name.c_str));
        return name;
}

int mliName_valid(const struct mliName *name)
{
        if (name->c_str[sizeof(name->c_str) - 1] == '\0') {
                return 1;
        } else {
                return 0;
        }
}

int mliName_is_equal(const struct mliName *a, const struct mliName *b)
{
        uint32_t i;
        for (i = 0; i < sizeof(a->c_str); i++) {
                if (a->c_str[i] != b->c_str[i]) {
                        return 0;
                }
        }
        return 1;
}

int mliName_find_idx(
        const struct mliName *names,
        const uint64_t num_names,
        const char *key,
        uint64_t*idx)
{
        uint64_t i;
        (*idx) = 0u;
        for (i = 0; i < num_names; i++) {
                if (strcmp(names[i].c_str, key) == 0) {
                        (*idx) = i;
                        return 1;
                }
        }
        return 0;
}

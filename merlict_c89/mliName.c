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

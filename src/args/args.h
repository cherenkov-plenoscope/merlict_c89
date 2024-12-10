/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_ARGS_H_
#define MLI_ARGS_H_

#include "../array/array.h"
#include "../string/string.h"

MLI_ARRAY_DEFINITON(mli_ArrayString, struct mli_String)

int mli_ArrayString_from_argc_argv(
        struct mli_ArrayString *self,
        int argc,
        char *argv[]);

#endif

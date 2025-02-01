/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_ARGS_H_
#define MLI_ARGS_H_

#include "../string/string_vector.h"

int mli_StringVector_from_argc_argv(
        struct mli_StringVector *self,
        int argc,
        char *argv[]);

#endif

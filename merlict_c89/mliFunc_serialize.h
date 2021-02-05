/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFUNC_SERIALIZE_H_
#define MERLICT_C89_MLIFUNC_SERIALIZE_H_

#include <stdio.h>
#include "mliFunc.h"

int mliFunc_malloc_fread(struct mliFunc *func, FILE *f);
int mliFunc_fwrite(const struct mliFunc *func, FILE *f);
#endif

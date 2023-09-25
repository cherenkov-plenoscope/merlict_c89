/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIFUNC_SERIALIZE_H_
#define MLIFUNC_SERIALIZE_H_

#include <stdio.h>
#include "mliFunc.h"

int mliFunc_malloc_fread(struct mliFunc *func, FILE *f);
int mliFunc_fwrite(const struct mliFunc *func, FILE *f);
#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOBJECT_SERIALIZE_H_
#define MERLICT_C89_MLIOBJECT_SERIALIZE_H_

#include <stdio.h>
#include "mliObject.h"

int mliObject_fwrite(const struct mliObject *obj, FILE *f);
int mliObject_malloc_fread(struct mliObject *obj, FILE *f);
#endif

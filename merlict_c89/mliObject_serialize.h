/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_SERIALIZE_H_
#define MLIOBJECT_SERIALIZE_H_

#include <stdio.h>
#include "mliObject.h"

int mliObject_fwrite(const struct mliObject *obj, FILE *f);
int mliObject_malloc_fread(struct mliObject *obj, FILE *f);
#endif

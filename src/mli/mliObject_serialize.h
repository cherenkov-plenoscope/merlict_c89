/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_SERIALIZE_H_
#define MLIOBJECT_SERIALIZE_H_

#include "../io/io.h"
#include "mliObject.h"

int mliObject_fwrite(const struct mliObject *obj, struct mli_IO *f);
int mliObject_malloc_fread(struct mliObject *obj, struct mli_IO *f);
#endif

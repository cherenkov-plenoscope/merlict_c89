/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_SERIALIZE_H_
#define MLIOBJECT_SERIALIZE_H_

#include "../io/io.h"
#include "mliObject.h"

int mliObject_to_io(const struct mliObject *obj, struct mli_IO *f);
int mliObject_from_io(struct mliObject *obj, struct mli_IO *f);
#endif

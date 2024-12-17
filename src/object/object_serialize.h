/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_SERIALIZE_H_
#define MLI_OBJECT_SERIALIZE_H_

#include "../io/io.h"
#include "object.h"

int mliObject_to_io(const struct mliObject *obj, struct mli_IO *f);
int mliObject_from_io(struct mliObject *obj, struct mli_IO *f);
#endif

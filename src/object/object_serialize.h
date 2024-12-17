/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_SERIALIZE_H_
#define MLI_OBJECT_SERIALIZE_H_

#include "../io/io.h"
#include "object.h"

int mli_Object_to_io(const struct mli_Object *obj, struct mli_IO *f);
int mli_Object_from_io(struct mli_Object *obj, struct mli_IO *f);
#endif

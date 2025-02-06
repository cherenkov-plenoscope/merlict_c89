/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_SERIALIZE_H_
#define MLI_OBJECT_SERIALIZE_H_

#include "../io/io.h"
#include "../chk/chk.h"
#include "object.h"

chk_rc mli_Object_to_io(const struct mli_Object *obj, struct mli_IO *f);
chk_rc mli_Object_from_io(struct mli_Object *obj, struct mli_IO *f);
#endif

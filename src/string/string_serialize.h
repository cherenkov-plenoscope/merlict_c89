/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_STRING_SERIALIZE_H_
#define MLI_STRING_SERIALIZE_H_

#include "../io/io.h"
#include "string.h"

int mli_String_to_io(const struct mli_String *self, struct mli_IO *f);
int mli_String_from_io(struct mli_String *self, struct mli_IO *f);

#endif

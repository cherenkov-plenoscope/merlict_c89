/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_STRING_SERIALIZE_H_
#define MLI_STRING_SERIALIZE_H_

#include <stdio.h>
#include "string.h"

int mli_String_fwrite(const struct mli_String *self, FILE *f);
int mli_String_malloc_fread(struct mli_String *self, FILE *f);

#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMEDIUM_SERIALIZE_H_
#define MERLICT_C89_MLIMEDIUM_SERIALIZE_H_

#include "mliMedium.h"

int mliMedium_fwrite(const struct mliMedium *med, FILE *f);
int mliMedium_malloc_fread(struct mliMedium *med, FILE *f);
#endif

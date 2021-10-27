/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMEDIUM_SERIALIZE_H_
#define MLIMEDIUM_SERIALIZE_H_

#include "mliMedium.h"

int mliMedium_fwrite(const struct mliMedium *med, FILE *f);
int mliMedium_malloc_fread(struct mliMedium *med, FILE *f);
#endif

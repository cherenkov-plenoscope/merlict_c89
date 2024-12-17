/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISURFACE_SERIALIZE_H_
#define MLISURFACE_SERIALIZE_H_

#include "mliSurface.h"
#include "../io/io.h"

int mliSurface_fwrite(const struct mliSurface *srf, struct mli_IO *f);
int mliSurface_malloc_fread(struct mliSurface *srf, struct mli_IO *f);
#endif

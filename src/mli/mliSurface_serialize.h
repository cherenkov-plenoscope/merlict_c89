/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISURFACE_SERIALIZE_H_
#define MLISURFACE_SERIALIZE_H_

#include "mliSurface.h"
#include "../io/io.h"

int mliSurface_to_io(const struct mliSurface *srf, struct mli_IO *f);
int mliSurface_from_io(struct mliSurface *srf, struct mli_IO *f);
#endif

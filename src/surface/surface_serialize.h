/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_SERIALIZE_H_
#define MLI_SURFACE_SERIALIZE_H_

#include "surface.h"
#include "../io/io.h"

int mli_Surface_to_io(const struct mli_Surface *srf, struct mli_IO *f);
int mli_Surface_from_io(struct mli_Surface *srf, struct mli_IO *f);
#endif

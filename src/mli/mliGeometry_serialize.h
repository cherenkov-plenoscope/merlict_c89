/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRY_SERIALIZE_H_
#define MLIGEOMETRY_SERIALIZE_H_

#include "../io/io.h"
#include "mliGeometry.h"

int mliGeometry_to_io(const struct mliGeometry *scenery, struct mli_IO *f);
int mliGeometry_from_io(struct mliGeometry *scenery, struct mli_IO *f);
#endif

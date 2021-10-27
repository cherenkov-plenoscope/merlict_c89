/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRY_SERIALIZE_H_
#define MLIGEOMETRY_SERIALIZE_H_

#include <stdio.h>
#include "mliGeometry.h"

int mliGeometry_fwrite(const struct mliGeometry *scenery, FILE *f);
int mliGeometry_malloc_fread(struct mliGeometry *scenery, FILE *f);
#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIGEOMETRY_SERIALIZE_H_
#define MERLICT_C89_MLIGEOMETRY_SERIALIZE_H_

#include <stdio.h>
#include "mliGeometry.h"

int mliGeometry_fwrite(const struct mliGeometry *scenery, FILE *f);
int mliGeometry_malloc_fread(struct mliGeometry *scenery, FILE *f);
#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_mliGeometry_SERIALIZE_H_
#define MERLICT_C89_mliGeometry_SERIALIZE_H_

#include <stdio.h>
#include "mliGeometry.h"

int mliGeometry_fwrite(const struct mliGeometry *scenery, FILE *f);
int mliGeometry_malloc_fread(struct mliGeometry *scenery, FILE *f);

int mliGeometry_write_to_path(
        const struct mliGeometry *scenery,
        const char *path);
int mliGeometry_malloc_from_path(struct mliGeometry *scenery, const char *path);

#endif

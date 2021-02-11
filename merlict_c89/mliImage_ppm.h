/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIIMAGE_PPM_H_
#define MERLICT_C89_MLIIMAGE_PPM_H_

#include <stdio.h>
#include "mliImage.h"

int mliImage_write_to_ppm(const struct mliImage *img, const char *path);
int mliImage_malloc_from_ppm(struct mliImage *img, const char *path);
#endif

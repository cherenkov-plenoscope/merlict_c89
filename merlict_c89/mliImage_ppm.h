/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIIMAGE_PPM_H_
#define MLIIMAGE_PPM_H_

#include <stdio.h>
#include "mliImage.h"

int mliImage_fwrite(const struct mliImage *img, FILE *f);
int mliImage_malloc_fread(struct mliImage *img, FILE *f);

int mliImage_malloc_from_path(struct mliImage *img, const char *path);
int mliImage_write_to_path(const struct mliImage *img, const char *path);
#endif

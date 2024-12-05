/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_PPM_H_
#define MLI_IMAGE_PPM_H_

#include <stdio.h>
#include "../image/image.h"

int mli_Image_fwrite(const struct mli_Image *img, FILE *f);
int mli_Image_malloc_fread(struct mli_Image *img, FILE *f);

int mli_Image_malloc_from_path(struct mli_Image *img, const char *path);
int mli_Image_write_to_path(const struct mli_Image *img, const char *path);
#endif

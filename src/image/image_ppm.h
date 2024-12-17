/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_PPM_H_
#define MLI_IMAGE_PPM_H_

#include "../image/image.h"
#include "../io/io.h"

int mli_Image_to_io(const struct mli_Image *img, struct mli_IO *f);
int mli_Image_from_io(struct mli_Image *img, struct mli_IO *f);

#endif

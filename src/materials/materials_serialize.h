/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_SERIALIZE_H_
#define MLI_MATERIALS_SERIALIZE_H_

#include "../io/io.h"
#include "materials.h"

int mli_Materials_to_io(const struct mli_Materials *res, struct mli_IO *f);
int mli_Materials_from_io(struct mli_Materials *res, struct mli_IO *f);
#endif

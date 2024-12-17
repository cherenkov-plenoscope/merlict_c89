/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMATERIALS_SERIALIZE_H_
#define MLIMATERIALS_SERIALIZE_H_

#include "../io/io.h"
#include "mliMaterials.h"

int mliMaterials_fwrite(const struct mliMaterials *res, struct mli_IO *f);
int mliMaterials_malloc_fread(struct mliMaterials *res, struct mli_IO *f);
#endif

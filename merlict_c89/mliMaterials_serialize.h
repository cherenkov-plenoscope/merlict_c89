/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMATERIALS_SERIALIZE_H_
#define MLIMATERIALS_SERIALIZE_H_

#include <stdio.h>
#include "mliMaterials.h"

int mliMaterials_fwrite(const struct mliMaterials *res, FILE *f);
int mliMaterials_malloc_fread(struct mliMaterials *res, FILE *f);
#endif

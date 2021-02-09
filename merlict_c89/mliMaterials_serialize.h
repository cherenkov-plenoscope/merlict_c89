/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMATERIALS_SERIALIZE_H_
#define MERLICT_C89_MLIMATERIALS_SERIALIZE_H_

#include <stdio.h>
#include "mliMaterials.h"

int mliMaterials_fwrite(
        const struct mliMaterials *res,
        FILE *f);
int mliMaterials_malloc_fread(
        struct mliMaterials *res,
        FILE *f);
#endif

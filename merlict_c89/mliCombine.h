/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICOMBINE_H_
#define MERLICT_C89_MLICOMBINE_H_

#include "mliGeometry.h"
#include "mliMaterials.h"
#include "mliAccelerator.h"
#include "mliArchive.h"

struct mliCombine {
        struct mliGeometry geometry;
        struct mliAccelerator accelerator;
        struct mliMaterials materials;
};

struct mliCombine mliCombine_init(void);

void mliCombine_free(struct mliCombine *combine);


int mliCombine_malloc_from_tar(
        struct mliCombine *combine,
        const char *path);
int mliCombine_malloc_from_Archive(
        struct mliCombine *combine,
        const struct mliArchive *archive);

#endif

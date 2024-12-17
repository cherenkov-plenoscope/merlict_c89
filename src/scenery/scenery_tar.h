/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SCENERY_TAR_H_
#define MLI_SCENERY_TAR_H_

#include "scenery.h"
#include "../archive/archive.h"

int mli_Scenery_from_io_tar(struct mli_Scenery *self, FILE *f);
int mli_Scenery_malloc_from_path_tar(
        struct mli_Scenery *self,
        const char *path);
int mli_Scenery_malloc_from_Archive(
        struct mli_Scenery *self,
        const struct mli_Archive *archive);
#endif

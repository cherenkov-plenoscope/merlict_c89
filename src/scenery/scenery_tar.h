/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SCENERY_TAR_H_
#define MLI_SCENERY_TAR_H_

#include "scenery.h"
#include "../chk/chk.h"
#include "../archive/archive.h"

chk_rc mli_Scenery_from_io_tar(struct mli_Scenery *self, struct mli_IO *f);
chk_rc mli_Scenery__from_path_cstr(struct mli_Scenery *self, const char *path);
chk_rc mli_Scenery_malloc_from_Archive(
        struct mli_Scenery *self,
        const struct mli_Archive *archive);
#endif

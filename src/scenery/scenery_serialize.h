/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SCENERY_SERIALIZE_H_
#define MLI_SCENERY_SERIALIZE_H_

#include "scenery.h"
#include "../io/io.h"

int mli_Scenery_to_io(const struct mli_Scenery *self, struct mli_IO *f);
int mli_Scenery_from_io(struct mli_Scenery *self, struct mli_IO *f);

int mli_Scenery_malloc_from_path(struct mli_Scenery *self, const char *path);
int mli_Scenery_write_to_path(
        const struct mli_Scenery *self,
        const char *path);
#endif

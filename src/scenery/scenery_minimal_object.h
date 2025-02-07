/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SCENERY_MINIMAL_OBJECT_H_
#define MLI_SCENERY_MINIMAL_OBJECT_H_

#include "scenery.h"
#include "../chk/chk.h"

chk_rc mli_Scenery_malloc_minimal_from_wavefront(
        struct mli_Scenery *self,
        const char *path);
#endif

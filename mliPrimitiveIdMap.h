/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPRIMITIVEIDMAP_H_
#define MERLICT_MLIPRIMITIVEIDMAP_H_

#include <stdlib.h>
#include <stdint.h>
#include "mli_debug.h"

struct mliPrimitiveIdMap {
        uint64_t num_primitives;
        uint32_t *user_ids;
};

struct mliPrimitiveIdMap mliPrimitiveIdMap_init(void) {
        struct mliPrimitiveIdMap pm;
        pm.num_primitives = 0u;
        pm.user_ids = NULL;
        return pm;
}

void mliPrimitiveIdMap_free(struct mliPrimitiveIdMap *pm) {
        free(pm->user_ids);
        (*pm) = mliPrimitiveIdMap_init();
}

int mliPrimitiveIdMap_malloc(
        struct mliPrimitiveIdMap *pm,
        const uint64_t num_primitives)
{
        mliPrimitiveIdMap_free(pm);
        pm->num_primitives = num_primitives;
        mli_malloc(pm->user_ids, uint32_t, pm->num_primitives);
        return 1;
error:
        mliPrimitiveIdMap_free(pm);
        return 0;
}

#endif

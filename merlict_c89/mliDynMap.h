/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMAP_H_
#define MERLICT_C89_MLIMAP_H_

#include <stdint.h>
#include "mli_debug.h"
#include "mliName.h"

struct _mliMapItem {
        char key[MLI_NAME_CAPACITY];
        uint64_t value;
};
MLIDYNARRAY_DEFINITON(mli, Map, struct _mliMapItem)
int mliDynMap_has(const struct mliDynMap *map, const char *key);
int mliDynMap_insert(struct mliDynMap *map, const char *key, uint64_t value);
int mliDynMap_find(const struct mliDynMap *map, const char *key, uint64_t *idx);
int mliDynMap_get(
        const struct mliDynMap *map,
        const char *key,
        uint64_t *value);

#endif

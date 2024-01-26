/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMAP_H_
#define MLIMAP_H_

#include <stdint.h>
#include "mliName.h"
#include "mliDynArray.h"

struct mliDynMapItem {
        char key[MLI_NAME_CAPACITY];
        uint64_t value;
};
MLIDYNARRAY_DEFINITON(mli, Map, struct mliDynMapItem)
int mliDynMap_has(const struct mliDynMap *map, const char *key);
int mliDynMap_insert(struct mliDynMap *map, const char *key, uint64_t value);
int mliDynMap_find(const struct mliDynMap *map, const char *key, uint64_t *idx);
int mliDynMap_get(
        const struct mliDynMap *map,
        const char *key,
        uint64_t *value);

#endif

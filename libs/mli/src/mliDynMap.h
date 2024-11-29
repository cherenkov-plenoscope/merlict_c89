/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMAP_H_
#define MLIMAP_H_

#include <stdint.h>
#include "mliName.h"
#include "mliArray.h"
#include "../../mtl/src/vec.h"

struct mliMapItem {
        char key[MLI_NAME_CAPACITY];
        uint64_t value;
};
MLIARRAY_DEFINITON(mli, MapItem, struct mliMapItem)
MTL_VEC_DEFINITON(mli, MapItem, struct mliMapItem)

struct mliDynMap {
        struct mliDynMapItem items;
};

struct mliDynMap mliDynMap_init(void);
void mliDynMap_free(struct mliDynMap *map);
int mliDynMap_malloc(struct mliDynMap *map);
uint64_t mliDynMap_size(const struct mliDynMap *map);
int mliDynMap_has(const struct mliDynMap *map, const char *key);
int mliDynMap_insert(struct mliDynMap *map, const char *key, uint64_t value);
int mliDynMap_find(const struct mliDynMap *map, const char *key, uint64_t *idx);
int mliDynMap_get(
        const struct mliDynMap *map,
        const char *key,
        uint64_t *value);

#endif

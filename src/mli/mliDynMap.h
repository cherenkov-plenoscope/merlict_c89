/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMAP_H_
#define MLIMAP_H_

#include <stdint.h>
#include "mliName.h"
#include "../string/string.h"
#include "../vector/vector.h"

struct mliMapItem {
        struct mli_String key;
        uint64_t value;
};

MLI_VECTOR_DEFINITON(mli_VectorMapItem, struct mliMapItem)

struct mliDynMap {
        struct mli_VectorMapItem items;
};

struct mliDynMap mliDynMap_init(void);
void mliDynMap_free(struct mliDynMap *map);
int mliDynMap_malloc(struct mliDynMap *map);
uint64_t mliDynMap_size(const struct mliDynMap *map);
int mliDynMap_has(const struct mliDynMap *map, const struct mli_String *key);
int mliDynMap_insert(
        struct mliDynMap *map,
        const struct mli_String *key,
        uint64_t value);
int mliDynMap_find(
        const struct mliDynMap *map,
        const struct mli_String *key,
        uint64_t *idx);
int mliDynMap_get(
        const struct mliDynMap *map,
        const struct mli_String *key,
        uint64_t *value);

#endif

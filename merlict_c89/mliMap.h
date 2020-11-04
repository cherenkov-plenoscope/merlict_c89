/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMAP_H_
#define MERLICT_C89_MLIMAP_H_

#include <stdint.h>
#include "mli_debug.h"

#define MLI_MAP_KEY_CAPACITY 128

struct mliMap3Item {
        char key[MLI_MAP_KEY_CAPACITY];
        uint64_t value;
};
MLIDYNARRAY_DEFINITON(mli, Map3, struct mliMap3Item)
struct mliMap3Item mliMap3Item_init(void);
int mliDynMap3_has(const struct mliDynMap3 *map, const char *key);
int mliDynMap3_insert(struct mliDynMap3 *map, const char *key, uint64_t v);
int mliDynMap3_get(const struct mliDynMap3 *map, const char *key, uint64_t *v);

#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MAP_H_
#define MLI_MAP_H_

#include <stdint.h>
#include "../bool/bool.h"
#include "../chk/chk.h"
#include "../string/string.h"
#include "../vector/vector.h"

struct mliMapItem {
        struct mli_String key;
        uint64_t value;
};

MLI_VECTOR_DEFINITON(mli_MapItemVector, struct mliMapItem)

struct mli_Map {
        struct mli_MapItemVector items;
};

struct mli_Map mli_Map_init(void);
void mli_Map_free(struct mli_Map *map);
chk_rc mli_Map_malloc(struct mli_Map *map);
uint64_t mli_Map_size(const struct mli_Map *map);
mli_bool mli_Map_has(const struct mli_Map *map, const struct mli_String *key);
chk_rc mli_Map_insert(
        struct mli_Map *map,
        const struct mli_String *key,
        uint64_t value);
chk_rc mli_Map_find(
        const struct mli_Map *map,
        const struct mli_String *key,
        uint64_t *idx);
chk_rc mli_Map_get(
        const struct mli_Map *map,
        const struct mli_String *key,
        uint64_t *value);

#endif

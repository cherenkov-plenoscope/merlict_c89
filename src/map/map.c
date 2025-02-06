/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "map.h"
#include "../chk/chk.h"

MLI_VECTOR_IMPLEMENTATION(mli_MapItemVector, struct mliMapItem)

struct mli_Map mli_Map_init(void)
{
        struct mli_Map map;
        map.items = mli_MapItemVector_init();
        return map;
}

void mli_Map_free(struct mli_Map *map)
{
        size_t i;
        for (i = 0; i < map->items.size; i++) {
                struct mliMapItem *item = &map->items.array[i];
                mli_String_free(&item->key);
        }
        mli_MapItemVector_free(&map->items);
}

int mli_Map_malloc(struct mli_Map *map)
{
        chk_msg(mli_MapItemVector_malloc(&map->items, 0u),
                "Failed to malloc map items.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

uint64_t mli_Map_size(const struct mli_Map *map) { return map->items.size; }

int mli_Map_find(
        const struct mli_Map *map,
        const struct mli_String *key,
        uint64_t *idx)
{
        uint64_t i;
        for (i = 0; i < map->items.size; i++) {
                struct mliMapItem *item = &map->items.array[i];
                if (mli_String_compare(&item->key, key) == 0) {
                        (*idx) = i;
                        return 1;
                }
        }
        return 0;
}

int mli_Map_has(const struct mli_Map *map, const struct mli_String *key)
{
        uint64_t idx;
        return mli_Map_find(map, key, &idx);
}

int mli_Map_insert(
        struct mli_Map *map,
        const struct mli_String *key,
        uint64_t value)
{
        struct mliMapItem item;
        if (map->items.size == 0u) {
                chk_msg(mli_Map_malloc(map),
                        "Failed to initially malloc dyn-map.");
        }
        chk_msg(0 == mli_Map_has(map, key), "Key already in use.");
        item.key = mli_String_init();
        chk_msg(mli_String_copy(&item.key, key),
                "Failed to malloc map item key.") item.value = value;
        chk_msg(mli_MapItemVector_push_back(&map->items, item),
                "Failed to mmaloc item.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Map_get(
        const struct mli_Map *map,
        const struct mli_String *key,
        uint64_t *value)
{
        uint64_t idx;
        struct mliMapItem *item = NULL;
        chk_msg(mli_Map_find(map, key, &idx), "Key does not exist.");
        item = &map->items.array[idx];
        (*value) = item->value;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

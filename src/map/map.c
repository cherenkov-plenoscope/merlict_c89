/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "map.h"
#include "../chk/chk.h"

MLI_VECTOR_IMPLEMENTATION(mli_VectorMapItem, struct mliMapItem)

struct mliDynMap mliDynMap_init(void)
{
        struct mliDynMap map;
        map.items = mli_VectorMapItem_init();
        return map;
}

void mliDynMap_free(struct mliDynMap *map)
{
        size_t i;
        for (i = 0; i < map->items.size; i++) {
                struct mliMapItem *item = &map->items.array[i];
                mli_String_free(&item->key);
        }
        mli_VectorMapItem_free(&map->items);
}

int mliDynMap_malloc(struct mliDynMap *map)
{
        chk_msg(mli_VectorMapItem_malloc(&map->items, 0u),
                "Failed to malloc map items.");
        return 1;
chk_error:
        return 0;
}

uint64_t mliDynMap_size(const struct mliDynMap *map) { return map->items.size; }

int mliDynMap_find(
        const struct mliDynMap *map,
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

int mliDynMap_has(const struct mliDynMap *map, const struct mli_String *key)
{
        uint64_t idx;
        return mliDynMap_find(map, key, &idx);
}

int mliDynMap_insert(
        struct mliDynMap *map,
        const struct mli_String *key,
        uint64_t value)
{
        struct mliMapItem item;
        if (map->items.size == 0u) {
                chk_msg(mliDynMap_malloc(map),
                        "Failed to initially malloc dyn-map.");
        }
        chk_msg(0 == mliDynMap_has(map, key), "Key already in use.");
        item.key = mli_String_init();
        chk_msg(mli_String_copy(&item.key, key),
                "Failed to malloc map item key.") item.value = value;
        chk_msg(mli_VectorMapItem_push_back(&map->items, item),
                "Failed to mmaloc item.");
        return 1;
chk_error:
        return 0;
}

int mliDynMap_get(
        const struct mliDynMap *map,
        const struct mli_String *key,
        uint64_t *value)
{
        uint64_t idx;
        struct mliMapItem *item = NULL;
        chk_msg(mliDynMap_find(map, key, &idx), "Key does not exist.");
        item = &map->items.array[idx];
        (*value) = item->value;
        return 1;
chk_error:
        return 0;
}

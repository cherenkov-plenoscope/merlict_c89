/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDynMap.h"
#include "../../chk_debug/include/chk_debug.h"

MLIDYNARRAY_IMPLEMENTATION(mli, Map, struct mliDynMapItem)

int mliDynMap_find(const struct mliDynMap *map, const char *key, uint64_t *idx)
{
        uint64_t i;
        for (i = 0; i < map->size; i++) {
                if (strcmp(map->array[i].key, key) == 0) {
                        (*idx) = i;
                        return 1;
                }
        }
        return 0;
}

int mliDynMap_has(const struct mliDynMap *map, const char *key)
{
        uint64_t idx;
        return mliDynMap_find(map, key, &idx);
}

int mliDynMap_insert(struct mliDynMap *map, const char *key, uint64_t value)
{
        struct mliDynMapItem item;
        if (map->size == 0u) {
                chk_msg(mliDynMap_malloc(map, 0u),
                        "Failed to initially malloc dyn-map.");
        }
        chk_msg(strlen(key) < MLI_NAME_CAPACITY, "Key too long.");
        chk_msg(0 == mliDynMap_has(map, key), "Key already in use.");
        memset(item.key, '\0', MLI_NAME_CAPACITY);
        strcpy(item.key, key);
        item.value = value;
        chk_msg(mliDynMap_push_back(map, item), "Failed to mmaloc item.");
        return 1;
error:
        return 0;
}

int mliDynMap_get(const struct mliDynMap *map, const char *key, uint64_t *value)
{
        uint64_t idx;
        chk_msg(mliDynMap_find(map, key, &idx), "Key does not exist.");
        (*value) = map->array[idx].value;
        return 1;
error:
        return 0;
}

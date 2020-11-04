/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMap.h"

MLIDYNARRAY_IMPLEMENTATION(mli, Map, struct _mliMapItem)
struct _mliMapItem _mliMapItem_init(void)
{
        struct _mliMapItem item;
        memset(item.key, '\0', MLI_MAP_KEY_CAPACITY);
        item.value = 0u;
        return item;
}
int mliDynMap_find(
        const struct mliDynMap *map,
        const char *key,
        uint64_t *idx)
{
        uint64_t i;
        for (i = 0; i < map->dyn.size; i++) {
                if (strcmp(map->arr[i].key, key) == 0) {
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
int mliDynMap_insert(struct mliDynMap *map, const char *key, uint64_t v)
{
        struct _mliMapItem item;
        mli_check(strlen(key) < MLI_MAP_KEY_CAPACITY, "Key too long.");
        mli_check(0 == mliDynMap_has(map, key), "Key already in use.");
        item = _mliMapItem_init();
        strcpy(item.key, key);
        item.value = v;
        mli_check(mliDynMap_push_back(map, item), "Failed to mmaloc item.");
        return 1;
error:
        return 0;
}
int mliDynMap_get(const struct mliDynMap *map, const char *key, uint64_t *v)
{
        uint64_t idx;
        mli_check(mliDynMap_find(map, key, &idx), "Key does not exist.");
        (*v) = map->arr[idx].value;
        return 1;
error:
        return 0;
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMap.h"

MLIDYNARRAY_IMPLEMENTATION(mli, Map3, struct mliMap3Item)
struct mliMap3Item mliMap3Item_init(void)
{
        struct mliMap3Item item;
        memset(item.key, '\0', 128);
        item.value = 0u;
        return item;
}
int mliDynMap3_find(
        const struct mliDynMap3 *map,
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
int mliDynMap3_has(const struct mliDynMap3 *map, const char *key)
{
        uint64_t idx;
        return mliDynMap3_find(map, key, &idx);
}
int mliDynMap3_insert(struct mliDynMap3 *map, const char *key, uint64_t v)
{
        struct mliMap3Item item;
        mli_check(strlen(key) < 128, "Key too long.");
        mli_check(0 == mliDynMap3_has(map, key), "Key already in use.");
        item = mliMap3Item_init();
        strcpy(item.key, key);
        item.value = v;
        mli_check(mliDynMap3_push_back(map, item), "Failed to mmaloc item.");
        return 1;
error:
        return 0;
}
int mliDynMap3_get(const struct mliDynMap3 *map, const char *key, uint64_t *v)
{
        uint64_t idx;
        mli_check(mliDynMap3_find(map, key, &idx), "Key does not exist.");
        (*v) = map->arr[idx].value;
        return 1;
error:
        return 0;
}


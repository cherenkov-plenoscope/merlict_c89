#include "mliArchive.h"

MLIDYNARRAY_IMPLEMENTATION(mli, MapItem, struct mliMapItem)

struct mliMapItem mliMapItem_init(void)
{
        struct mliMapItem item;
        memset(item.key, '\0', 128);
        item.value = NULL;
        return item;
}

int mliDynMapItem_find_key(
        const struct mliDynMapItem *map,
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

int mliDynMapItem_has(const struct mliDynMapItem *map, const char *key)
{
        uint64_t idx;
        return mliDynMapItem_find_key(map, key, &idx);
}

int mliDynMapItem_malloc_insert(
        struct mliDynMapItem *map,
        const char *key,
        void *value)
{
        struct mliMapItem item;
        mli_check(strlen(key) < 128, "Key too long.");
        mli_check(0 == mliDynMapItem_has(map, key), "Key already in use.");
        item = mliMapItem_init();
        strcpy(item.key, key);
        item.value = value;
        mli_check(mliDynMapItem_push_back(map, item), "Failed to mmaloc item.");
        return 1;
error:
        return 0;
}

int mliDynMapItem_get(
        const struct mliDynMapItem *map,
        const char *key,
        void **value)
{
        uint64_t idx;
        mli_check(
                mliDynMapItem_find_key(map, key, &idx),
                "Key does not exist."
        );
        (*value) = map->arr[idx].value;
        return 1;
error:
        return 0;
}









struct mliArc mliArc_init(void)
{
        struct mliArc arc;
        arc.objects = mliDynMapItem_init();
        arc.functions = mliDynMapItem_init();
        return arc;
}

void mliArc_free(struct mliArc *arc)
{
        uint64_t i;
        for (i = 0; i < arc->objects.dyn.size; i++) {
                mliObject_free(arc->objects.arr[i].value);
                free(arc->objects.arr[i].value);
        }
        mliDynMapItem_free(&arc->functions);
        for (i = 0; i < arc->functions.dyn.size; i++) {
                mliFunc_free(arc->functions.arr[i].value);
                free(arc->functions.arr[i].value);
        }
        mliDynMapItem_free(&arc->functions);
        (*arc) = mliArc_init();
}

int mliArc_malloc_from_tar(struct mliArc *arc, const char *path)
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        struct mliString string_buffer = mliString_init();
        mliArc_free(arc);

        mli_c(mliDynMapItem_malloc(&arc->objects, 0u));
        mli_c(mliDynMapItem_malloc(&arc->functions, 0u));

        mli_check(mliTar_open(&tar, path, "r"), "Cant open Tar.");

        while (mliTar_read_header(&tar, &tarh)) {

                mli_check(
                        mliString_malloc(&string_buffer, tarh.size),
                        "Can not allocate string-buffer."
                );
                mli_check(
                        mliTar_read_data(
                                &tar,
                                (void *)string_buffer.c_str,
                                tarh.size
                        ),
                        "Failed to read payload from tar into string-buffer."
                );

                if (mli_string_ends_with(tarh.name, ".obj")) {

                        struct mliObject *obj = NULL;
                        mli_malloc(obj, struct mliObject, 1u);
                        (*obj) = mliObject_init();
                        mli_check(
                                mliObject_malloc_from_string(
                                        obj,
                                        string_buffer.c_str
                                ),
                                "Failed to malloc wavefronf Object from '.obj'"
                                "-string from tar."
                        );
                        mli_check(
                                mliDynMapItem_malloc_insert(
                                        &arc->objects,
                                        tarh.name,
                                        (void *)obj
                                ),
                                "Failed to insert Object into named map."
                        );
                }

                if (mli_string_ends_with(tarh.name, ".csv")) {

                        struct mliFunc *func = NULL;
                        mli_malloc(func, struct mliFunc, 1u);
                        (*func) = mliFunc_init();
                        mli_check(
                                mliFunc_malloc_from_string(
                                        func,
                                        string_buffer.c_str
                                ),
                                "Failed to malloc CSV function from '.csv'"
                                "-string from tar."
                        );
                        mli_check(
                                mliDynMapItem_malloc_insert(
                                        &arc->functions,
                                        tarh.name,
                                        (void *)func
                                ),
                                "Failed to insert Object into map."
                        );
                }
        }

        mliTar_close(&tar);
        mliString_free(&string_buffer);
        return 1;
error:
        mliArc_free(arc);
        if (tar.stream) {
                mliTar_close(&tar);
        }
        mliString_free(&string_buffer);
        return 0;
}

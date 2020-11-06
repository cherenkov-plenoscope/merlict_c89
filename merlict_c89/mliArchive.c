#include "mliArchive.h"

MLIDYNARRAY_IMPLEMENTATION(mli, String, struct mliString)

struct mliArchive mliArchive_init(void)
{
        struct mliArchive arc;
        arc.strings = mliDynString_init();
        arc.filenames = mliDynMap_init();
        return arc;
}

void mliArchive_free(struct mliArchive *arc)
{
        uint64_t i;
        for (i = 0; i < arc->strings.dyn.size; i++) {
                mliString_free(&arc->strings.arr[i]);
        }
        mliDynString_free(&arc->strings);
        mliDynMap_free(&arc->filenames);
        (*arc) = mliArchive_init();
}

int mliArchive_malloc_from_tar(struct mliArchive *arc, const char *path)
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();

        mliArchive_free(arc);
        mliDynString_malloc(&arc->strings, 0u);

        mli_check(mliTar_open(&tar, path, "r"), "Cant open Tar.");

        while (mliTar_read_header(&tar, &tarh)) {
                uint64_t next = arc->filenames.dyn.size;
                struct mliString *payload = NULL;

                mli_check(
                        mliDynMap_insert(&arc->filenames, tarh.name, next),
                        "Can not insert key."
                );

                mli_check(
                        mliDynString_push_back(&arc->strings, mliString_init()),
                        "Can not push back mliString."
                );
                payload = &arc->strings.arr[next];

                mli_check(
                        mliString_malloc(payload, tarh.size),
                        "Can not allocate string-buffer."
                );

                mli_check(
                        mliTar_read_data(
                                &tar,
                                (void *)payload->c_str,
                                tarh.size
                        ),
                        "Failed to read payload from tar into string-buffer."
                );
        }

        mliTar_close(&tar);
        return 1;
error:
        mliArchive_free(arc);
        if (tar.stream) {
                mliTar_close(&tar);
        }
        return 0;
}

int mliArchive_has(const struct mliArchive *arc, const char *filename)
{
        return mliDynMap_has(&arc->filenames, filename);
}

int mliArchive_get(struct mliArchive *arc, const char *filename, struct mliString **str)
{
        uint64_t idx;
        mli_c(mliDynMap_find(&arc->filenames, filename, &idx));
        (*str) = &arc->strings.arr[idx];
        return 1;
error:
        return 0;
}

uint64_t mliArchive_num(const struct mliArchive *arc)
{
        return arc->filenames.dyn.size;
}

void mliArchive_print(struct mliArchive *arc)
{
        uint64_t i;
        for (i = 0; i < arc->strings.dyn.size; i++) {
                struct _mliMapItem *map_item = &arc->filenames.arr[i];
                fprintf(
                        stderr,
                        "%ld: %s, %ld\n",
                        i,
                        map_item->key,
                        arc->strings.arr[i].capacity
                );
        }
}
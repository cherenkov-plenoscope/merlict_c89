#include "mliArchive.h"

MLIDYNARRAY_IMPLEMENTATION(mli, String, struct mliString)

struct mliArc mliArc_init(void)
{
        struct mliArc arc;
        arc.strings = mliDynString_init();
        arc.filenames = mliDynMap_init();
        return arc;
}

void mliArc_free(struct mliArc *arc)
{
        uint64_t i;
        for (i = 0; i < arc->strings.dyn.size; i++) {
                mliString_free(&arc->strings.arr[i]);
        }
        mliDynString_free(&arc->strings);
        mliDynMap_free(&arc->filenames);
        (*arc) = mliArc_init();
}

int mliArc_malloc_from_tar(struct mliArc *arc, const char *path)
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();

        mliArc_free(arc);
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
        mliArc_free(arc);
        if (tar.stream) {
                mliTar_close(&tar);
        }
        return 0;
}

int mliArc_has(const struct mliArc *arc, const char *filename)
{
        return mliDynMap_has(&arc->filenames, filename);
}

int mliArc_get(struct mliArc *arc, const char *filename, struct mliString **str)
{
        uint64_t idx;
        mli_c(mliDynMap_find(&arc->filenames, filename, &idx));
        (*str) = &arc->strings.arr[idx];
        return 1;
error:
        return 0;
}

uint64_t mliArc_num(const struct mliArc *arc)
{
        return arc->filenames.dyn.size;
}

void mliArc_print(struct mliArc *arc)
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

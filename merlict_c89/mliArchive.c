#include "mliArchive.h"


struct mliArc mliArc_init(void)
{
        struct mliArc arc;
        arc.objects = mliDynMap_init();
        arc.functions = mliDynMap_init();
        return arc;
}

void mliArc_free(struct mliArc *arc)
{
        uint64_t i;
        for (i = 0; i < arc->objects.dyn.size; i++) {
                mliObject_free(arc->objects.arr[i].value);
                free(arc->objects.arr[i].value);
        }
        mliDynMap_free(&arc->functions);
        for (i = 0; i < arc->functions.dyn.size; i++) {
                mliFunc_free(arc->functions.arr[i].value);
                free(arc->functions.arr[i].value);
        }
        mliDynMap_free(&arc->functions);
        (*arc) = mliArc_init();
}

int mliArc_malloc_from_tar(struct mliArc *arc, const char *path)
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        struct mliString string_buffer = mliString_init();
        mliArc_free(arc);

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
                                mliDynMap_insert(
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
                                mliDynMap_insert(
                                        &arc->functions,
                                        tarh.name,
                                        (void *)func
                                ),
                                "Failed to insert 1D-Func into map."
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

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliArchive.h"
#include "mli_string_to.h"
#include "mli_debug.h"
#include "mliJson.h"
#include "mliTar.h"

MLIDYNARRAY_IMPLEMENTATION(mli, String, struct mliDynStr)

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
        for (i = 0; i < arc->strings.size; i++) {
                mliDynStr_free(&arc->strings.array[i]);
        }
        mliDynString_free(&arc->strings);
        mliDynMap_free(&arc->filenames);
        (*arc) = mliArchive_init();
}

int mliArchive_malloc_from_tar(struct mliArchive *arc, const char *path)
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        struct mliDynStr tmp_payload = mliDynStr_init();
        char tarh_name[MLITAR_NAME_LENGTH] = {'\0'};

        mliArchive_free(arc);
        mliDynString_malloc(&arc->strings, 0u);
        mli_check(mliTar_open(&tar, path, "r"), "Cant open Tar.");

        while (mliTar_read_header(&tar, &tarh)) {
                uint64_t next = arc->filenames.size;
                struct mliDynStr *payload = NULL;
                memset(tarh_name, '\0', sizeof(tarh_name));

                _mli_strip_this_dir(tarh_name, tarh.name);

                mli_check(
                        mliDynMap_insert(&arc->filenames, tarh_name, next),
                        "Can not insert key.");
                mli_check(
                        mliDynString_push_back(&arc->strings, mliDynStr_init()),
                        "Can not push back mliString.");
                mli_check(
                        mliDynStr_malloc(&tmp_payload, tarh.size + 1),
                        "Can not allocate tmp-string-buffer.");

                payload = &arc->strings.array[next];
                (*payload) = mliDynStr_init();

                mli_check(
                        mliDynStr_malloc(payload, 0),
                        "Can not allocate string-buffer.");
                mli_check(
                        mliTar_read_data(
                                &tar, (void *)tmp_payload.c_str, tarh.size),
                        "Failed to read payload from tar into "
                        "tmp-string-buffer.");
                mli_check(
                        mliDynStr_convert_line_break_CRLF_CR_to_LF(
                                payload, &tmp_payload),
                        "Failed to replace CRLF and CR linebreaks.");
                mliDynStr_free(&tmp_payload);
                mli_check(
                        mli_string_assert_only_NUL_LF_TAB_controls(
                                payload->c_str),
                        "Did not expect control codes other than "
                        "('\\n', '\\t', '\\0') in text-files.");
        }

        mliTar_close(&tar);
        return 1;
error:
        mli_eprintf("tar '%s', filename: '%s'.", path, tarh_name);
        mliDynStr_free(&tmp_payload);
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

int mliArchive_get(
        const struct mliArchive *arc,
        const char *filename,
        struct mliDynStr **str)
{
        uint64_t idx;
        mli_c(mliDynMap_find(&arc->filenames, filename, &idx));
        (*str) = &arc->strings.array[idx];
        return 1;
error:
        return 0;
}

int mliArchive_get_malloc_json(
        const struct mliArchive *arc,
        const char *filename,
        struct mliJson *json)
{
        struct mliDynStr *text = NULL;

        mli_check(
                mliArchive_get(arc, filename, &text),
                "Can not find requested file in archive.");

        mli_check(
                mliJson_malloc_from_string(json, text->c_str),
                "Can not parse requested json.");

        return 1;
error:
        return 0;
}

uint64_t mliArchive_num(const struct mliArchive *arc)
{
        return arc->filenames.size;
}

void mliArchive_info_fprint(FILE *f, const struct mliArchive *arc)
{
        uint64_t i;
        for (i = 0; i < arc->strings.size; i++) {
                struct _mliMapItem *map_item = &arc->filenames.array[i];
                fprintf(f,
                        "%u: %s, %u\n",
                        (uint32_t)i,
                        map_item->key,
                        (uint32_t)arc->strings.array[i].capacity);
        }
}

void mliArchive_mask_filename_prefix_sufix(
        const struct mliArchive *arc,
        uint64_t *mask,
        const char *prefix,
        const char *sufix)
{
        uint64_t i = 0u;
        uint64_t match = 0u;
        for (i = 0; i < arc->strings.size; i++) {
                struct _mliMapItem *map_item = &arc->filenames.array[i];

                match = mli_string_has_prefix_suffix(
                        map_item->key, prefix, sufix);

                if (match) {
                        mask[i] = 1;
                } else {
                        mask[i] = 0;
                }
        }
}

uint64_t mliArchive_num_filename_prefix_sufix(
        const struct mliArchive *arc,
        const char *prefix,
        const char *sufix)
{
        uint64_t i = 0;
        uint64_t match;
        uint64_t num_matches = 0;
        for (i = 0; i < arc->strings.size; i++) {
                struct _mliMapItem *map_item = &arc->filenames.array[i];

                match = mli_string_has_prefix_suffix(
                        map_item->key, prefix, sufix);

                if (match) {
                        num_matches++;
                }
        }
        return num_matches;
}

void _mli_strip_this_dir(char *dst, const char *src)
{
        const char *_src = &src[0];
        memset(dst, '\0', strlen(src));
        while (mli_string_starts_with(_src, "./") && _src[0] != '\0') {
                _src += 2;
        }
        strcpy(dst, _src);
}

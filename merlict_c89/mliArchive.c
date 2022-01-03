/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliArchive.h"
#include "mli_cstr.h"
#include "mli_json.h"
#include "mliTar.h"

MLIDYNARRAY_IMPLEMENTATION(mli, TextFiles, struct mliStr)

struct mliArchive mliArchive_init(void)
{
        struct mliArchive arc;
        arc.textfiles = mliDynTextFiles_init();
        arc.filenames = mliDynMap_init();
        return arc;
}

void mliArchive_free(struct mliArchive *arc)
{
        uint64_t i;
        for (i = 0; i < arc->textfiles.size; i++) {
                mliStr_free(&arc->textfiles.array[i]);
        }
        mliDynTextFiles_free(&arc->textfiles);
        mliDynMap_free(&arc->filenames);
        (*arc) = mliArchive_init();
}

int mliArchive_malloc_from_tar(struct mliArchive *arc, const char *path)
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        struct mliStr tmp_payload = mliStr_init();
        char tarh_name[MLI_TAR_NAME_LENGTH] = {'\0'};

        mliArchive_free(arc);
        mliDynTextFiles_malloc(&arc->textfiles, 0u);
        chk_msg(mliTar_open(&tar, path, "r"), "Cant open Tar.");

        while (mliTar_read_header(&tar, &tarh)) {
                uint64_t next = arc->filenames.size;
                struct mliStr *payload = NULL;
                memset(tarh_name, '\0', sizeof(tarh_name));

                mli_cstr_path_strip_this_dir(tarh_name, tarh.name);

                chk_msg(mliDynMap_insert(&arc->filenames, tarh_name, next),
                        "Can not insert key.");
                chk_msg(mliDynTextFiles_push_back(
                                &arc->textfiles, mliStr_init()),
                        "Can not push back mliString.");
                chk_msg(mliStr_malloc_capacity(&tmp_payload, tarh.size + 1),
                        "Can not allocate tmp-string-buffer.");

                payload = &arc->textfiles.array[next];
                (*payload) = mliStr_init();

                chk_msg(mliStr_malloc(payload),
                        "Can not allocate string-buffer.");
                chk_msg(mliTar_read_data(
                                &tar, (void *)tmp_payload.c_str, tarh.size),
                        "Failed to read payload from tar into "
                        "tmp-string-buffer.");
                chk_msg(mliStr_convert_line_break_CRLF_CR_to_LF(
                                payload, &tmp_payload),
                        "Failed to replace CRLF and CR linebreaks.");
                mliStr_free(&tmp_payload);
                chk_msg(mli_cstr_assert_only_NUL_LF_TAB_controls(
                                payload->c_str),
                        "Did not expect control codes other than "
                        "('\\n', '\\t', '\\0') in textfiles.");
        }

        mliTar_close(&tar);
        return 1;
error:
        fprintf(stderr, "tar '%s', filename: '%s'.\n", path, tarh_name);
        mliStr_free(&tmp_payload);
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
        struct mliStr **str)
{
        uint64_t idx;
        chk(mliDynMap_find(&arc->filenames, filename, &idx));
        (*str) = &arc->textfiles.array[idx];
        return 1;
error:
        return 0;
}

int mliArchive_get_malloc_json(
        const struct mliArchive *arc,
        const char *filename,
        struct mliJson *json)
{
        struct mliStr *text = NULL;

        chk_msg(mliArchive_get(arc, filename, &text),
                "Can not find requested file in archive.");

        chk_msg(mliJson_malloc_from_c_str(json, text->c_str),
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
        for (i = 0; i < arc->textfiles.size; i++) {
                struct mliDynMapItem *map_item = &arc->filenames.array[i];
                fprintf(f,
                        "%u: %s, %u\n",
                        (uint32_t)i,
                        map_item->key,
                        (uint32_t)arc->textfiles.array[i].capacity);
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
        for (i = 0; i < arc->textfiles.size; i++) {
                struct mliDynMapItem *map_item = &arc->filenames.array[i];

                match = mli_cstr_has_prefix_suffix(
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
        for (i = 0; i < arc->textfiles.size; i++) {
                struct mliDynMapItem *map_item = &arc->filenames.array[i];

                match = mli_cstr_has_prefix_suffix(
                        map_item->key, prefix, sufix);

                if (match) {
                        num_matches++;
                }
        }
        return num_matches;
}

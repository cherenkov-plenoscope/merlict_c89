/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliArchive.h"
#include "../chk/chk.h"
#include "../string/string.h"
#include "../path/path.h"
#include "../cstr/cstr.h"
#include "../json/json.h"
#include "../tar/tar.h"

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
                struct mli_String *str = &arc->textfiles.array[i];
                mli_String_free(str);
        }
        mliDynTextFiles_free(&arc->textfiles);
        mliDynMap_free(&arc->filenames);
        (*arc) = mliArchive_init();
}

int mliArchive_malloc(struct mliArchive *arc)
{
        mliArchive_free(arc);
        chk(mliDynTextFiles_malloc(&arc->textfiles, 0u));
        chk(mliDynMap_malloc(&arc->filenames));
        return 1;
chk_error:
        return 0;
}

int mliArchive_push_back(
        struct mliArchive *arc,
        const struct mli_String *filename,
        const struct mli_String *payload)
{
        uint64_t next;
        struct mli_String *text = NULL;
        chk_msg(filename->size < MLI_TAR_NAME_LENGTH,
                "Expected shorter filename.");
        next = mliDynMap_size(&arc->filenames);

        /* filename */
        /* ======== */
        chk_msg(mliDynMap_insert(&arc->filenames, filename, next),
                "Can not insert key.");

        /* payload */
        /* ======= */
        chk_msg(mliDynTextFiles_push_back(&arc->textfiles, mli_String_init()),
                "Can not push back mli_String.");
        text = &arc->textfiles.array[next];
        chk_msg(mli_String_copy(text, payload), "Can not copy payload.");
        return 1;
chk_error:
        return 0;
}

int mliArchive_malloc_fread(struct mliArchive *arc, FILE *f)
{
        struct mli_Tar tar = mli_Tar_init();
        struct mli_TarHeader tarh = mli_TarHeader_init();
        struct mli_String payload = mli_String_init();
        struct mli_String filename = mli_String_init();

        char tarh_name[MLI_TAR_NAME_LENGTH] = {'\0'};

        chk_msg(mliArchive_malloc(arc), "Can not malloc archive.");
        chk_msg(mli_Tar_read_begin(&tar, f), "Can't begin tar.");

        while (mli_Tar_read_header(&tar, &tarh)) {

                chk(mli_String_from_cstr(&filename, tarh.name));
                chk(mli_String_strip(&filename, &filename));
                chk(mli_path_strip_this_dir(&filename, &filename));

                chk_msg(mli_String_malloc(&payload, tarh.size),
                        "Can not allocate payload.");
                chk_msg(mli_Tar_read_data(
                                &tar, (void *)payload.array, tarh.size),
                        "Failed to read payload from tar into payload.");
                payload.size = strlen(payload.array);

                chk_msg(mli_String_convert_line_break_CRLF_CR_to_LF(
                                &payload, &payload),
                        "Failed to replace CRLF and CR linebreaks.");
                chk_msg(mli_cstr_assert_only_NUL_LF_TAB_controls(payload.array),
                        "Did not expect control codes other than "
                        "('\\n', '\\t', '\\0') in textfiles.");
                chk_msg(mliArchive_push_back(arc, &filename, &payload),
                        "Can not push back file into archive.");
        }

        chk_msg(mli_Tar_read_finalize(&tar), "Can't finalize reading tar.");
        mli_String_free(&payload);
        mli_String_free(&filename);
        return 1;
chk_error:
        fprintf(stderr, "tar->filename: '%s'.\n", tarh_name);
        mli_String_free(&payload);
        mli_String_free(&filename);
        mliArchive_free(arc);
        return 0;
}

int mliArchive_malloc_from_path(struct mliArchive *arc, const char *path)
{
        FILE *f = fopen(path, "rb");
        chk_msgf(f != NULL, ("Can't open path '%s'.", path))
                chk_msg(mliArchive_malloc_fread(arc, f),
                        "Can't fread Archive from file.");
        fclose(f);
        return 1;
chk_error:
        return 0;
}

int mliArchive_has(
        const struct mliArchive *arc,
        const struct mli_String *filename)
{
        return mliDynMap_has(&arc->filenames, filename);
}

int mliArchive_get(
        const struct mliArchive *arc,
        const struct mli_String *filename,
        struct mli_String **str)
{
        uint64_t idx;
        struct mli_String *txt = NULL;
        chk(mliDynMap_find(&arc->filenames, filename, &idx));
        txt = &arc->textfiles.array[idx];
        (*str) = txt;
        return 1;
chk_error:
        return 0;
}

int mliArchive_get_malloc_json(
        const struct mliArchive *arc,
        const struct mli_String *filename,
        struct mli_Json *json)
{
        struct mli_String *text = NULL;

        chk_msg(mliArchive_get(arc, filename, &text),
                "Can not find requested file in archive.");
        chk_msg(mli_Json_malloc_from_cstr(json, (char *)text->array),
                "Can not parse requested json.");

        return 1;
chk_error:
        return 0;
}

uint64_t mliArchive_num(const struct mliArchive *arc)
{
        return mliDynMap_size(&arc->filenames);
}

void mliArchive_info_fprint(FILE *f, const struct mliArchive *arc)
{
        uint64_t i;
        for (i = 0; i < arc->textfiles.size; i++) {
                struct mliMapItem *item = &arc->filenames.items.array[i];
                struct mli_String *str = &arc->textfiles.array[i];
                fprintf(f,
                        "%u: %s, %u\n",
                        (uint32_t)i,
                        item->key.array,
                        (uint32_t)str->size);
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
                struct mliMapItem *item = &arc->filenames.items.array[i];

                if (mli_String_starts_with_cstr(&item->key, prefix) &&
                    mli_String_ends_with_cstr(&item->key, sufix)) {
                        match = 1;
                } else {
                        match = 0;
                }

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
                struct mliMapItem *item = &arc->filenames.items.array[i];

                if (mli_String_starts_with_cstr(&item->key, prefix) &&
                    mli_String_ends_with_cstr(&item->key, sufix)) {
                        match = 1;
                } else {
                        match = 0;
                }

                if (match) {
                        num_matches++;
                }
        }
        return num_matches;
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliArchive.h"
#include "chk_debug.h"
#include "mli_cstr.h"
#include "mli_json.h"
#include "mliTar.h"
#include "chk_debug.h"

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

int mliArchive_malloc(struct mliArchive *arc)
{
        mliArchive_free(arc);
        chk(mliDynTextFiles_malloc(&arc->textfiles, 0u));
        chk(mliDynMap_malloc(&arc->filenames, 0u));
        return 1;
error:
        return 0;
}

int mliArchive_push_back(
        struct mliArchive *arc,
        const struct mliStr *filename,
        const struct mliStr *payload)
{
        uint64_t next;
        chk_msg(filename->length < MLI_TAR_NAME_LENGTH,
                "Expected shorter filename.");
        next = arc->filenames.size;

        /* filename */
        /* ======== */
        chk_msg(mliDynMap_insert(&arc->filenames, filename->cstr, next),
                "Can not insert key.");

        /* payload */
        /* ======= */
        chk_msg(mliDynTextFiles_push_back(&arc->textfiles, mliStr_init()),
                "Can not push back mliStr.");
        chk_msg(mliStr_malloc_copy(&arc->textfiles.array[next], payload),
                "Can not copy payload.");
        return 1;
error:
        return 0;
}

int mliArchive_malloc_fread(struct mliArchive *arc, FILE *f)
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        struct mliStr payload = mliStr_init();
        struct mliStr filename = mliStr_init();

        char tarh_name[MLI_TAR_NAME_LENGTH] = {'\0'};

        chk_msg(mliArchive_malloc(arc), "Can not malloc archive.");
        chk_msg(mliTar_read_begin(&tar, f), "Can't begin tar.");

        while (mliTar_read_header(&tar, &tarh)) {

                chk(mliStr_malloc_cstr(&filename, tarh.name));
                chk(mliStr_strip(&filename, &filename));
                chk(mli_path_strip_this_dir(&filename, &filename));

                chk_msg(mliStr_malloc(&payload, tarh.size),
                        "Can not allocate payload.");
                chk_msg(mliTar_read_data(&tar, (void *)payload.cstr, tarh.size),
                        "Failed to read payload from tar into payload.");
                chk_msg(mliStr_convert_line_break_CRLF_CR_to_LF(
                                &payload, &payload),
                        "Failed to replace CRLF and CR linebreaks.");
                chk_msg(mli_cstr_assert_only_NUL_LF_TAB_controls(payload.cstr),
                        "Did not expect control codes other than "
                        "('\\n', '\\t', '\\0') in textfiles.");
                chk_msg(mliArchive_push_back(arc, &filename, &payload),
                        "Can not push back file into archive.");
        }

        chk_msg(mliTar_read_finalize(&tar), "Can't finalize reading tar.");
        mliStr_free(&payload);
        mliStr_free(&filename);
        return 1;
error:
        fprintf(stderr, "tar->filename: '%s'.\n", tarh_name);
        mliStr_free(&payload);
        mliStr_free(&filename);
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
error:
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

        chk_msg(mliJson_malloc_from_cstr(json, (char *)text->cstr),
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
                        (uint32_t)arc->textfiles.array[i].length);
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

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "archive.h"
#include "../chk/chk.h"
#include "../path/path.h"
#include "../cstr/cstr.h"
#include "../string/string.h"
#include "../tar/tar.h"

struct mli_Archive mli_Archive_init(void)
{
        struct mli_Archive out;
        out.textfiles = mli_StringVector_init();
        out.filenames = mli_Map_init();
        return out;
}

void mli_Archive_free(struct mli_Archive *self)
{
        uint64_t i;
        for (i = 0; i < self->textfiles.size; i++) {
                struct mli_String *str = &self->textfiles.array[i];
                mli_String_free(str);
        }
        mli_StringVector_free(&self->textfiles);
        mli_Map_free(&self->filenames);
        (*self) = mli_Archive_init();
}

chk_rc mli_Archive_malloc(struct mli_Archive *self)
{
        mli_Archive_free(self);
        chk(mli_StringVector_malloc(&self->textfiles, 0u));
        chk(mli_Map_malloc(&self->filenames));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Archive_push_back(
        struct mli_Archive *self,
        const struct mli_String *filename,
        const struct mli_String *payload)
{
        uint64_t next;
        struct mli_String *text = NULL;
        chk_msg(filename->size < MLI_TAR_NAME_LENGTH,
                "Expected shorter filename.");
        next = mli_Map_size(&self->filenames);

        /* filename */
        /* ======== */
        chk_msg(mli_Map_insert(&self->filenames, filename, next),
                "Can not insert key.");

        /* payload */
        /* ======= */
        chk_msg(mli_StringVector_push_back(&self->textfiles, mli_String_init()),
                "Can not push back mli_String.");
        text = &self->textfiles.array[next];
        chk_msg(mli_String_copy(text, payload), "Can not copy payload.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Archive_from_io(struct mli_Archive *self, struct mli_IO *f)
{
        struct mli_Tar tar = mli_Tar_init();
        struct mli_TarHeader tarh = mli_TarHeader_init();
        struct mli_String payload = mli_String_init();
        struct mli_String filename = mli_String_init();

        char tarh_name[MLI_TAR_NAME_LENGTH] = {'\0'};

        chk_msg(mli_Archive_malloc(self), "Can not malloc selfhive.");
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
                chk_msg(mli_Archive_push_back(self, &filename, &payload),
                        "Can not push back file into selfhive.");
        }

        chk_msg(mli_Tar_read_finalize(&tar), "Can't finalize reading tar.");
        mli_String_free(&payload);
        mli_String_free(&filename);
        return CHK_SUCCESS;
chk_error:
        chk_eprintf("tar->filename: '%s'.\n", tarh_name);
        mli_String_free(&payload);
        mli_String_free(&filename);
        mli_Archive_free(self);
        return CHK_FAIL;
}

chk_rc mli_Archive__from_path_cstr(struct mli_Archive *self, const char *path)
{
        struct mli_IO f = mli_IO_init();
        chk_msg(mli_IO_open_file_cstr(&f, path, "r"),
                "Cant open archive from path.");
        chk_msg(mli_Archive_from_io(self, &f),
                "Can't fread Archive from file.");
        mli_IO_close(&f);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_Archive_has(
        const struct mli_Archive *self,
        const struct mli_String *filename)
{
        return mli_Map_has(&self->filenames, filename);
}

chk_rc mli_Archive_get(
        const struct mli_Archive *self,
        const struct mli_String *filename,
        struct mli_String **str)
{
        uint64_t idx;
        struct mli_String *txt = NULL;
        chk(mli_Map_find(&self->filenames, filename, &idx));
        txt = &self->textfiles.array[idx];
        (*str) = txt;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

uint64_t mli_Archive_size(const struct mli_Archive *self)
{
        return mli_Map_size(&self->filenames);
}

uint64_t mli_Archive_num_filename_prefix_sufix(
        const struct mli_Archive *self,
        const char *prefix,
        const char *sufix)
{
        uint64_t i = 0;
        uint64_t match;
        uint64_t num_matches = 0;
        for (i = 0; i < self->textfiles.size; i++) {
                struct mliMapItem *item = &self->filenames.items.array[i];

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

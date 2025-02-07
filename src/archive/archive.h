/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_ARCHIVE_H_
#define MLI_ARCHIVE_H_

#include <stdint.h>
#include "../bool/bool.h"
#include "../chk/chk.h"
#include "../io/io.h"
#include "../string/string.h"
#include "../string/string_vector.h"
#include "../map/map.h"

struct mli_Archive {
        struct mli_StringVector textfiles;
        struct mli_Map filenames;
};

struct mli_Archive mli_Archive_init(void);

void mli_Archive_free(struct mli_Archive *self);
chk_rc mli_Archive_malloc(struct mli_Archive *self);
chk_rc mli_Archive_from_io(struct mli_Archive *self, struct mli_IO *f);
chk_rc mli_Archive__from_path_cstr(struct mli_Archive *self, const char *path);
chk_rc mli_Archive_push_back(
        struct mli_Archive *self,
        const struct mli_String *filename,
        const struct mli_String *payload);
mli_bool mli_Archive_has(
        const struct mli_Archive *self,
        const struct mli_String *filename);
chk_rc mli_Archive_get(
        const struct mli_Archive *self,
        const struct mli_String *filename,
        struct mli_String **str);
uint64_t mli_Archive_size(const struct mli_Archive *self);
uint64_t mli_Archive_num_filename_prefix_sufix(
        const struct mli_Archive *self,
        const char *prefix,
        const char *sufix);

#endif

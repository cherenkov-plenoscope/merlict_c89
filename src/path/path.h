/* Copyright Sebastian Achim Mueller */
#ifndef MLI_PATH_H_
#define MLI_PATH_H_
#include "../chk/chk.h"
#include "../string/string.h"

chk_rc mli_path_strip_this_dir(
        const struct mli_String *src,
        struct mli_String *dst);
chk_rc mli_path_basename(const struct mli_String *src, struct mli_String *dst);
chk_rc mli_path_splitext(
        const struct mli_String *src,
        struct mli_String *dst,
        struct mli_String *ext);

#endif

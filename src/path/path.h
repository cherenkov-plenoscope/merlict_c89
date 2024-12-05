/* Copyright Sebastian Achim Mueller */
#ifndef MLI_PATH_H_
#define MLI_PATH_H_
#include "../string/string.h"

int mli_path_strip_this_dir(
        const struct mtl_String *src,
        struct mtl_String *dst);
int mli_path_basename(const struct mtl_String *src, struct mtl_String *dst);
int mli_path_splitext(
        const struct mtl_String *src,
        struct mtl_String *dst,
        struct mtl_String *ext);

#endif

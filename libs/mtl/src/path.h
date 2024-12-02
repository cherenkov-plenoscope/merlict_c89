/* Copyright Sebastian Achim Mueller */
#ifndef mtl_path_H_
#define mtl_path_H_
#include "string.h"

int mli_path_strip_this_dir(
        const struct mtl_String *src,
        struct mtl_String *dst);
int mli_path_basename(const struct mtl_String *src, struct mtl_String *dst);
int mli_path_splitext(
        const struct mtl_String *src,
        struct mtl_String *dst,
        struct mtl_String *ext);

#endif

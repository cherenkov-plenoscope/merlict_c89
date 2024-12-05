#include "../chk/chk.h"
#include "path.h"

int mtl_path_strip_this_dir(
        const struct mtl_String *src,
        struct mtl_String *dst)
{
        uint64_t i = 0;
        struct mtl_String cpysrc = mtl_String_init();
        chk_msg(src->array, "Expected src-string to be allocated.");
        chk_msg(mtl_String_copy(&cpysrc, src), "Can not copy input.");
        mtl_String_free(dst);

        if (cpysrc.array == NULL) {
                return 1;
        }

        while (i + 1 < cpysrc.size) {
                if (cpysrc.array[i] == '.' && cpysrc.array[i + 1] == '/') {
                        i += 2;
                } else {
                        break;
                }
        }
        chk(mtl_String_malloc(dst, cpysrc.size - i));
        strcpy(dst->array, &cpysrc.array[i]);
        mtl_String_free(&cpysrc);
        return 1;
chk_error:
        mtl_String_free(&cpysrc);
        mtl_String_free(dst);
        return 0;
}

int mtl_path_basename(const struct mtl_String *src, struct mtl_String *dst)
{
        int64_t pos_last_del = -1;
        mtl_String_free(dst);
        chk_msg(src->array != NULL, "Expected src-path to be allocated");

        pos_last_del = mtl_String_rfind(src, '/');

        if (pos_last_del < 0) {
                chk(mtl_String_from_cstr_fromat(dst, src->array));
        } else {
                chk(mtl_String_from_cstr_fromat(
                        dst, &src->array[pos_last_del + 1]));
        }
        return 1;
chk_error:
        mtl_String_free(dst);
        return 0;
}

int mli_path_splitext(
        const struct mtl_String *src,
        struct mtl_String *dst,
        struct mtl_String *ext)
{
        int64_t p = -1;
        int64_t d = -1;
        struct mtl_String tmp = mtl_String_init();
        chk_msg(src->array != NULL, "Expected src-path to be allocated");
        chk(mtl_String_copy(&tmp, src));

        mtl_String_free(dst);
        mtl_String_free(ext);

        p = mtl_String_rfind(&tmp, '.');
        d = mtl_String_rfind(&tmp, '/');

        if (p <= 0 || d > p || ((d + 1 == p) && (p + 1 < (int64_t)tmp.size))) {
                chk(mtl_String_from_cstr_fromat(dst, tmp.array));
                chk(mtl_String_from_cstr_fromat(ext, ""));
        } else {
                chk(mtl_String_malloc(dst, p));
                strncpy(dst->array, tmp.array, p);
                chk(mtl_String_malloc(ext, tmp.size - p));
                strncpy(ext->array, &tmp.array[p + 1], tmp.size - p);
        }

        mtl_String_free(&tmp);
        return 1;
chk_error:
        mtl_String_free(&tmp);
        mtl_String_free(dst);
        mtl_String_free(ext);
        return 0;
}

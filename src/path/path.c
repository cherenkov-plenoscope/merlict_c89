#include "../chk/chk.h"
#include "path.h"

int mli_path_strip_this_dir(
        const struct mli_String *src,
        struct mli_String *dst)
{
        uint64_t i = 0;
        struct mli_String cpysrc = mli_String_init();
        chk_msg(src->array, "Expected src-string to be allocated.");
        chk_msg(mli_String_copy(&cpysrc, src), "Can not copy input.");
        mli_String_free(dst);

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
        chk(mli_String_malloc(dst, cpysrc.size - i));
        strcpy(dst->array, &cpysrc.array[i]);
        mli_String_free(&cpysrc);
        return 1;
chk_error:
        mli_String_free(&cpysrc);
        mli_String_free(dst);
        return 0;
}

int mli_path_basename(const struct mli_String *src, struct mli_String *dst)
{
        int64_t pos_last_del = -1;
        mli_String_free(dst);
        chk_msg(src->array != NULL, "Expected src-path to be allocated");

        pos_last_del = mli_String_rfind(src, '/');

        if (pos_last_del < 0) {
                chk(mli_String_from_cstr_fromat(dst, src->array));
        } else {
                chk(mli_String_from_cstr_fromat(
                        dst, &src->array[pos_last_del + 1]));
        }
        return 1;
chk_error:
        mli_String_free(dst);
        return 0;
}

int mli_path_splitext(
        const struct mli_String *src,
        struct mli_String *dst,
        struct mli_String *ext)
{
        int64_t p = -1;
        int64_t d = -1;
        struct mli_String tmp = mli_String_init();
        chk_msg(src->array != NULL, "Expected src-path to be allocated");
        chk(mli_String_copy(&tmp, src));

        mli_String_free(dst);
        mli_String_free(ext);

        p = mli_String_rfind(&tmp, '.');
        d = mli_String_rfind(&tmp, '/');

        if (p <= 0 || d > p || ((d + 1 == p) && (p + 1 < (int64_t)tmp.size))) {
                chk(mli_String_from_cstr_fromat(dst, tmp.array));
                chk(mli_String_from_cstr_fromat(ext, ""));
        } else {
                chk(mli_String_malloc(dst, p));
                strncpy(dst->array, tmp.array, p);
                chk(mli_String_malloc(ext, tmp.size - p));
                strncpy(ext->array, &tmp.array[p + 1], tmp.size - p);
        }

        mli_String_free(&tmp);
        return 1;
chk_error:
        mli_String_free(&tmp);
        mli_String_free(dst);
        mli_String_free(ext);
        return 0;
}

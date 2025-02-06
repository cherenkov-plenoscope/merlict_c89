/* Copyright Sebastian Achim Mueller */
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../chk/chk.h"
#include "../cstr/cstr.h"

MLI_VECTOR_IMPLEMENTATION_ZERO_TERMINATION(mli_String, char)

int mli_String_from_cstr_fromat(
        struct mli_String *self,
        const char *format,
        ...)
{
        va_list args;
        va_start(args, format);
        chk_msg(mli_String_from_vargs(self, format, args),
                "Failed to malloc String from variadic args.");
        va_end(args);
        return 1;
chk_error:
        va_end(args);
        return 0;
}

int mli_String_from_vargs(
        struct mli_String *self,
        const char *format,
        va_list args)
{
        struct mli_String tmp = mli_String_init();
        const int64_t tmp_capacity = 10 * strlen(format);
        chk(mli_String_malloc(&tmp, tmp_capacity));

        vsprintf(tmp.array, format, args);
        chk_msg(mli_String__discover_size(&tmp) <= tmp_capacity,
                "Probably 'vsprintf' caused a buffer overflow.");
        tmp.size = strlen(tmp.array);
        chk(mli_String_copy(self, &tmp));

        mli_String_free(&tmp);
        return 1;
chk_error:
        mli_String_free(&tmp);
        return 0;
}

int mli_String_from_cstr(struct mli_String *self, const char *s)
{
        size_t length = strlen(s);
        chk(mli_String_malloc(self, length));
        self->size = length;
        strncpy(self->array, s, self->size);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_String_ends_with(
        const struct mli_String *self,
        const struct mli_String *suffix)
{
        if (!self->array || !suffix->array) {
                return 0;
        }
        if (suffix->size > self->size) {
                return 0;
        }
        return strncmp(self->array + self->size - suffix->size,
                       suffix->array,
                       suffix->size) == 0;
}

int mli_String_ends_with_cstr(const struct mli_String *self, const char *cstr)
{
        return mli_cstr_ends_with(self->array, cstr);
}

int mli_String_starts_with(
        const struct mli_String *self,
        const struct mli_String *prefix)
{
        if (!self->array || !prefix->array) {
                return 0;
        }
        if (prefix->size > self->size) {
                return 0;
        }
        return strncmp(self->array, prefix->array, prefix->size) == 0;
}

int mli_String_starts_with_cstr(const struct mli_String *self, const char *cstr)
{
        return mli_cstr_starts_with(self->array, cstr);
}

int mli_String_has_prefix_suffix(
        const struct mli_String *self,
        const struct mli_String *prefix,
        const struct mli_String *suffix)
{
        uint64_t has_pre = 1;
        uint64_t has_suf = 1;
        if (prefix->array != NULL) {
                has_pre = mli_String_starts_with(self, prefix);
        }

        if (suffix->array != NULL) {
                has_suf = mli_String_ends_with(self, suffix);
        }

        if (has_pre == 1 && has_suf == 1) {
                return 1;
        } else {
                return 0;
        }
}

int64_t mli_String_rfind(const struct mli_String *self, const char c)
{
        int64_t i;
        for (i = self->size - 1; i >= 0; i--) {
                if (self->array[i] == '\0') {
                        return -1;
                } else if (self->array[i] == c) {
                        return i;
                }
        }
        return -1;
}

int64_t mli_String_find(const struct mli_String *self, const char c)
{
        int64_t i;
        for (i = 0; i < (int64_t)self->size; i++) {
                if (self->array[i] == '\0') {
                        return -1;
                } else if (self->array[i] == c) {
                        return i;
                }
        }
        return -1;
}

int mli_String_match_templeate(
        const struct mli_String *s,
        const struct mli_String *t,
        const char digit_wildcard)
{
        uint64_t i;
        if (s->size != t->size) {
                return 0;
        }
        for (i = 0; i < s->size; i++) {
                if (t->array[i] == digit_wildcard) {
                        if (!isdigit(s->array[i])) {
                                return 0;
                        }
                } else {
                        if (s->array[i] != t->array[i]) {
                                return 0;
                        }
                }
        }
        return 1;
}

int mli_String_strip(const struct mli_String *src, struct mli_String *dst)
{
        int64_t start = 0;
        int64_t stop = 0;
        int64_t len = -1;
        struct mli_String cpysrc = mli_String_init();

        chk_msg(src->array, "Expected src-string to be allocated.");
        chk_msg(mli_String_copy(&cpysrc, src), "Can not copy input.");
        mli_String_free(dst);

        while (start < (int64_t)cpysrc.size && isspace(cpysrc.array[start])) {
                start += 1;
        }

        stop = cpysrc.size - 1;
        while (stop >= 0 && isspace(cpysrc.array[stop])) {
                stop -= 1;
        }

        len = stop - start;

        if (len < 0) {
                chk(mli_String_from_cstr_fromat(dst, ""));
        } else {
                chk(mli_String_copyn(dst, &cpysrc, start, len + 1));
        }
        mli_String_free(&cpysrc);
        return 1;
chk_error:
        mli_String_free(&cpysrc);
        mli_String_free(dst);
        return 0;
}

uint64_t mli_String_countn(
        const struct mli_String *self,
        const char match,
        const uint64_t num_chars_to_scan)
{
        uint64_t i = 0;
        uint64_t count = 0u;
        while (i < self->size && i < num_chars_to_scan) {
                if (self->array[i] == match) {
                        count++;
                }
                i++;
        }
        return count;
}

int mli_String_equal_cstr(const struct mli_String *self, const char *cstr)
{
        if (self->array == NULL) {
                return 0;
        }
        if (strcmp(self->array, cstr) == 0) {
                return 1;
        } else {
                return 0;
        }
}

int64_t mli_String_compare(
        const struct mli_String *s1,
        const struct mli_String *s2)
{
        return strcmp(s1->array, s2->array);
}

int mli_String_convert_line_break_CRLF_CR_to_LF(
        struct mli_String *dst,
        const struct mli_String *src)
{
        uint64_t i = 0;
        struct mli_String cpysrc = mli_String_init();
        chk(mli_String_malloc(&cpysrc, src->size));

        while (i < src->size) {
                if (mli_cstr_is_CRLF((char *)&src->array[i])) {
                        chk(mli_String_push_back(&cpysrc, '\n'));
                        i += 2;
                } else if (mli_cstr_is_CR((char *)&src->array[i])) {
                        chk(mli_String_push_back(&cpysrc, '\n'));
                        i += 1;
                } else {
                        chk(mli_String_push_back(&cpysrc, src->array[i]));
                        i += 1;
                }
        }
        chk(mli_String_copy(dst, &cpysrc));
        mli_String_free(&cpysrc);
        return 1;
chk_error:
        mli_String_free(&cpysrc);
        return 0;
}

int64_t mli_String__discover_size(const struct mli_String *self)
{
        int64_t i;
        for (i = 0; i < (int64_t)self->capacity; i++) {
                if (self->array[i] == '\0') {
                        break;
                }
        }

        if (i == (int64_t)self->capacity - 1) {
                if (self->array[i] != '\0') {
                        i = -1;
                }
        }
        return i;
}

int mli_String_equal(
        const struct mli_String *self,
        const struct mli_String *other)
{
        if (self->array == NULL || other->array == NULL) {
                return 0;
        }
        if (self->size == other->size) {
                size_t i;
                for (i = 0; i < self->size; i++) {
                        if (self->array[i] != other->array[i]) {
                                return 0;
                        }
                }
                return 1;
        } else {
                return 0;
        }
}

int mli_String_valid(const struct mli_String *self, const size_t min_size)
{
        const int64_t size = mli_String__discover_size(self);
        chk_msg(self->array != NULL, "Expected string to be allocated.");
        chk_msg(size >= (int64_t)min_size,
                "Expected string to have min_size "
                "and to be '\\0' terminated.");
        chk_msg(size == (int64_t)self->size,
                "Expected string.size to "
                "match zero termination.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_String__find_idx_with_cstr(
        const struct mli_String *names,
        const uint64_t num_names,
        const char *key,
        uint64_t *idx)
{
        uint64_t i;
        (*idx) = 0u;
        for (i = 0; i < num_names; i++) {
                if (mli_String_equal_cstr(&names[i], key)) {
                        (*idx) = i;
                        return 1;
                }
        }
        return 0;
}

/* Copyright Sebastian Achim Mueller */
#include "cstr.h"
#include <stdlib.h>
#include <ctype.h>
#include "../chk/chk.h"
#include "../bool/bool.h"
#include "../math/math.h"

mli_bool mli_cstr_ends_with(const char *str, const char *sufix)
{
        uint64_t len_str, len_sufix;
        if (!str || !sufix) {
                return MLI_FALSE;
        }
        len_str = strlen(str);
        len_sufix = strlen(sufix);
        if (len_sufix > len_str) {
                return MLI_FALSE;
        }
        return strncmp(str + len_str - len_sufix, sufix, len_sufix) == 0;
}

mli_bool mli_cstr_starts_with(const char *str, const char *prefix)
{
        uint64_t len_str, len_prefix;
        if (!str || !prefix) {
                return MLI_FALSE;
        }
        len_str = strlen(str);
        len_prefix = strlen(prefix);
        if (len_prefix > len_str) {
                return MLI_FALSE;
        }
        return strncmp(str, prefix, len_prefix) == 0;
}

mli_bool mli_cstr_is_CRLF(const char *s)
{
        if (s[0] == '\0') {
                return MLI_FALSE;
        }
        if (s[1] == '\0') {
                return MLI_FALSE;
        }
        if (s[0] == '\r' && s[1] == '\n') {
                return MLI_TRUE;
        }
        return MLI_FALSE;
}

mli_bool mli_cstr_is_CR(const char *s)
{
        if (s[0] == '\0') {
                return MLI_FALSE;
        }
        if (s[0] == '\r') {
                return MLI_TRUE;
        }
        return MLI_FALSE;
}

mli_bool mli_cstr_assert_only_NUL_LF_TAB_controls(const char *str)
{
        return mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(str, 1);
}

mli_bool mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(
        const char *str,
        const int dbg)
{
        uint64_t pos = 0;
        while (str[pos] != '\0') {
                if (str[pos] >= 32 && str[pos] < 127) {
                        /* all fine */
                } else {
                        if (str[pos] == '\n') {
                                /* fine */
                        } else if (str[pos] == '\t') {
                                /* fine */
                        } else {
                                if (dbg) {
                                        fprintf(stderr,
                                                "Control code %u "
                                                "at column %ld in string.\n",
                                                (uint8_t)str[pos],
                                                pos);
                                }
                                return MLI_FALSE;
                        }
                }
                pos += 1;
        }
        return MLI_TRUE;
}

mli_bool mli_cstr_match_templeate(
        const char *s,
        const char *t,
        const char digit_wildcard)
{
        uint64_t i;
        if (strlen(s) != strlen(t)) {
                return MLI_FALSE;
        }
        for (i = 0; i < strlen(s); i++) {
                if (t[i] == digit_wildcard) {
                        if (!isdigit(s[i])) {
                                return MLI_FALSE;
                        }
                } else {
                        if (s[i] != t[i]) {
                                return MLI_FALSE;
                        }
                }
        }
        return MLI_TRUE;
}

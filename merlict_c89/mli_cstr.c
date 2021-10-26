/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_cstr.h"
#include <stdlib.h>

int mli_string_ends_with(const char *str, const char *sufix)
{
        uint64_t len_str, len_sufix;
        if (!str || !sufix) {
                return 0;
        }
        len_str = strlen(str);
        len_sufix = strlen(sufix);
        if (len_sufix > len_str) {
                return 0;
        }
        return strncmp(str + len_str - len_sufix, sufix, len_sufix) == 0;
}

int mli_string_starts_with(const char *str, const char *prefix)
{
        uint64_t len_str, len_prefix;
        if (!str || !prefix) {
                return 0;
        }
        len_str = strlen(str);
        len_prefix = strlen(prefix);
        if (len_prefix > len_str) {
                return 0;
        }
        return strncmp(str, prefix, len_prefix) == 0;
}

int mli_string_has_prefix_suffix(
        const char *str,
        const char *prefix,
        const char *sufix)
{
        uint64_t has_pre = 1;
        uint64_t has_suf = 1;
        if (prefix != NULL) {
                has_pre = mli_string_starts_with(str, prefix);
        }

        if (sufix != NULL) {
                has_suf = mli_string_ends_with(str, sufix);
        }

        if (has_pre == 1 && has_suf == 1) {
                return 1;
        } else {
                return 0;
        }
}

int mli_string_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length)
{
        uint64_t i = 0;
        memset(token, '\0', token_length);
        for (i = 0; i < token_length; i++) {
                if (str[i] == '\0') {
                        break;
                } else if (str[i] == delimiter) {
                        break;
                } else {
                        token[i] = str[i];
                }
        }
        return i;
}

int mli_is_CRLF_line_break(const char *s)
{
        if (s[0] == '\0') {
                return 0;
        }
        if (s[1] == '\0') {
                return 0;
        }
        if (s[0] == '\r' && s[1] == '\n') {
                return 1;
        }
        return 0;
}

int mli_is_CR_line_break(const char *s)
{
        if (s[0] == '\0') {
                return 0;
        }
        if (s[0] == '\r') {
                return 1;
        }
        return 0;
}

int mli_string_assert_only_NUL_LF_TAB_controls(const char *str)
{
        return mli_string_assert_only_NUL_LF_TAB_controls_dbg(str, 1);
}

int mli_string_assert_only_NUL_LF_TAB_controls_dbg(
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
                                return 0;
                        }
                }
                pos += 1;
        }
        return 1;
}

uint64_t mli_string_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan)
{
        uint64_t i = 0;
        uint64_t count = 0u;
        while (str[i] != '\0' && i < num_chars_to_scan) {
                if (str[i] == c) {
                        count++;
                }
                i++;
        }
        return count;
}

int mli_fprint_line_match(
        FILE *f,
        const int64_t line,
        const int64_t line_number)
{
        chk(fprintf(f, "% 6d", (int32_t)line));
        if (line == line_number) {
                chk(fprintf(f, "->|  "));
        } else {
                chk(fprintf(f, "  |  "));
        }
        return 1;
error:
        return 0;
}

int mli_lines_info_fprint(
        FILE *f,
        const char *text,
        const uint64_t line_number,
        const uint64_t line_radius)
{
        int64_t _line_number = (int64_t)line_number;
        int64_t _line_radius = (int64_t)line_radius;
        int64_t line_start = MLI_MAX2(_line_number - _line_radius, 1);
        int64_t line_stop = line_number + line_radius;
        int64_t line = 1;
        int64_t i = 0;

        chk_msg(line_radius > 1, "Expected line_radius > 1.");

        chk(fprintf(f, "  line     text\n"));
        chk(fprintf(f, "        |\n"));

        while (text[i]) {
                int prefix = (line + 1 >= line_start) && (line < line_stop);
                int valid = (line >= line_start) && (line <= line_stop);
                if (text[i] == '\n') {
                        line++;
                }
                if (prefix && i == 0) {
                        chk(mli_fprint_line_match(f, line, _line_number));
                }
                if (valid) {
                        chk(putc(text[i], f));
                }
                if (prefix && text[i] == '\n') {
                        chk(mli_fprint_line_match(f, line, _line_number));
                }
                i++;
        }
        chk(putc('\n', f));

        return 1;
error:
        return 0;
}

void mli_strip_this_dir(char *dst, const char *src)
{
        const char *_src = &src[0];
        memset(dst, '\0', strlen(src));
        while (mli_string_starts_with(_src, "./") && _src[0] != '\0') {
                _src += 2;
        }
        strcpy(dst, _src);
}

void mli_cstr_path_basename_without_extension(const char *filename, char *key)
{
        uint64_t i = 0u;
        uint64_t o = 0u;

        while (1) {
                if (filename[i] == '\0') {
                        goto finalize;
                }
                if (filename[i] == '/') {
                        i += 1;
                        break;
                }
                i += 1;
        }

        while (1) {
                if (filename[i] == '\0') {
                        goto finalize;
                }
                if (filename[i] == '.') {
                        i += 1;
                        break;
                }
                key[o] = filename[i];
                i += 1;
                o += 1;
        }

finalize:
        key[o] = '\0';
}

void mli_cstr_strip_spaces(const char *in, char *out)
{
        uint64_t i = 0u;
        uint64_t o = 0u;
        while (in[i] && isspace(in[i])) {
                i += 1;
        }
        while (in[i] && !isspace(in[i])) {
                out[o] = in[i];
                i += 1;
                o += 1;
        }
        out[o] = '\0';
}

int mli_cstr_match_templeate(
        const char *str,
        const char *template,
        const char digit_wildcard)
{
        uint64_t i;
        if (strlen(str) != strlen(template)) {
                return 0;
        }
        for (i = 0; i < strlen(str); i++) {
                if (template[i] == digit_wildcard) {
                        if (!isdigit(str[i])) {
                                return 0;
                        }
                } else {
                        if (str[i] != template[i]) {
                                return 0;
                        }
                }
        }
        return 1;
}

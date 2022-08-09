/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mli_cstr.h"

CASE("mli_cstr_ends_with")
{
        CHECK(mli_cstr_ends_with("123", ""));
        CHECK(mli_cstr_ends_with("", ""));
        CHECK(!mli_cstr_ends_with("", "123"));
        CHECK(mli_cstr_ends_with("123", "123"));
        CHECK(mli_cstr_ends_with("my_file.json", ".json"));
        CHECK(!mli_cstr_ends_with("my_file.json.stuff", ".json"));
}

CASE("mli_cstr_starts_with")
{
        CHECK(mli_cstr_starts_with("123", ""));
        CHECK(mli_cstr_starts_with("", ""));
        CHECK(!mli_cstr_starts_with("", "123"));
        CHECK(mli_cstr_starts_with("123", "123"));
        CHECK(mli_cstr_starts_with("my_file.json", "my_file"));
        CHECK(mli_cstr_starts_with("my_file.json.stuff", "my_file.json"));
        CHECK(mli_cstr_starts_with("functions/hans.csv", "functions/"));
}

CASE("mli_cstr_has_prefix_suffix")
{
        CHECK(mli_cstr_has_prefix_suffix("", "", ""));
        CHECK(mli_cstr_has_prefix_suffix("", NULL, NULL));
        CHECK(mli_cstr_has_prefix_suffix("abc", NULL, NULL));
        CHECK(mli_cstr_has_prefix_suffix("abc", "a", "c"));
        CHECK(!mli_cstr_has_prefix_suffix("abc", "a", "d"));
        CHECK(!mli_cstr_has_prefix_suffix("_abc", "a", "c"));
}

CASE("mli_cstr_split_valid")
{
        char token[128];
        char str[128];
        char delimiter = '\n';
        int token_size;
        int p = 0;

        CHECK(snprintf(str, 127, "first\nsecond\n\nthird"));
        token_size = mli_cstr_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 5);
        CHECK(0 == strcmp(token, "first"));
        p += token_size + 1;

        token_size = mli_cstr_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 6);
        CHECK(0 == strcmp(token, "second"));
        p += token_size + 1;

        token_size = mli_cstr_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 0);
        CHECK(0 == strcmp(token, ""));
        p += token_size + 1;

        token_size = mli_cstr_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 5);
        CHECK(0 == strcmp(token, "third"));
        CHECK(str[p + token_size] == '\0');
}

CASE("mli_cstr_split_empty")
{
        char token[128];
        char str[128];
        char delimiter = '\n';
        int token_size;
        int p = 0;

        memset(str, '\0', 128);
        token_size = mli_cstr_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 0);
        CHECK(token[0] == '\0');
}

CASE("mli_cstr_path_strip_this_dir")
{
        char src[128];
        char dst[128];

        CHECK(0 == strcmp("", ""));

        memset(src, '\0', sizeof(src));
        memset(dst, '\0', sizeof(dst));

        CHECK(snprintf(src, 127, "/a/b/c"));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, src));

        CHECK(snprintf(src, 127, "./a/b/c"));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a/b/c"));
        CHECK(0 == strcmp(src, "./a/b/c"));

        CHECK(snprintf(src, 127, "./functions/hans.csv"));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "functions/hans.csv"));
        CHECK(0 == strcmp(src, "./functions/hans.csv"));

        CHECK(snprintf(src, 127, "././././f/h.csv"));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "f/h.csv"));
        CHECK(0 == strcmp(src, "././././f/h.csv"));

        CHECK(snprintf(src, 127, "a/b"));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a/b"));
        CHECK(0 == strcmp(src, "a/b"));

        CHECK(snprintf(src, 127, ".a/b"));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, ".a/b"));
        CHECK(0 == strcmp(src, ".a/b"));

        CHECK(snprintf(src, 127, "a./b"));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a./b"));
        CHECK(0 == strcmp(src, "a./b"));

        memset(src, '\0', sizeof(src));
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strlen(src));
        CHECK(0 == strlen(dst));
        CHECK(0 == strcmp(dst, ""));
        CHECK(0 == strcmp(src, ""));
}

CASE("find CRLF and CR linebreaks")
{
        char txt[128];
        memset(txt, '\0', sizeof(txt));
        CHECK(!mli_cstr_is_CRLF(&txt[0]));
        CHECK(!mli_cstr_is_CR(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 127, "\r\n"));
        CHECK(mli_cstr_is_CRLF(&txt[0]));
        CHECK(mli_cstr_is_CR(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 127, "01\r\n23"));
        CHECK(!mli_cstr_is_CRLF(&txt[0]));
        CHECK(!mli_cstr_is_CR(&txt[0]));

        CHECK(!mli_cstr_is_CRLF(&txt[1]));
        CHECK(!mli_cstr_is_CR(&txt[1]));

        CHECK(mli_cstr_is_CRLF(&txt[2]));
        CHECK(mli_cstr_is_CR(&txt[2]));

        CHECK(!mli_cstr_is_CRLF(&txt[3]));
        CHECK(!mli_cstr_is_CR(&txt[3]));

        CHECK(!mli_cstr_is_CRLF(&txt[4]));
        CHECK(!mli_cstr_is_CR(&txt[4]));

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 127, "\r"));
        CHECK(!mli_cstr_is_CRLF(&txt[0]));
        CHECK(mli_cstr_is_CR(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 127, "0\r1"));
        CHECK(!mli_cstr_is_CRLF(&txt[0]));
        CHECK(!mli_cstr_is_CR(&txt[0]));

        CHECK(!mli_cstr_is_CRLF(&txt[1]));
        CHECK(mli_cstr_is_CR(&txt[1]));

        CHECK(!mli_cstr_is_CRLF(&txt[2]));
        CHECK(!mli_cstr_is_CR(&txt[2]));
}

CASE("replace CRLF and CR linebreaks with LF")
{
        struct mliStr src = mliStr_init();
        struct mliStr dst = mliStr_init();

        CHECK(mliStr_malloc_capacity(&src, 32));
        CHECK(mliStr_malloc_capacity(&dst, 32));

        /* all '\0' */
        /* -------- */
        CHECK(src.cstr[0] == '\0');
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(dst.cstr[0] == '\0');

        /* minimal CR */
        /* ---------- */
        memset(src.cstr, '\0', src.capacity);
        CHECK(snprintf(src.cstr, src.capacity - 1, "\r"));
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, "\n"));

        /* minimal CRLF */
        /* ------------ */
        memset(src.cstr, '\0', src.capacity);
        CHECK(snprintf(src.cstr, src.capacity - 1, "\r\n"));
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, "\n"));

        /* minimal text CRLF */
        /* ----------------- */
        memset(src.cstr, '\0', src.capacity);
        CHECK(snprintf(src.cstr, src.capacity - 1, "hans\r\npeter"));
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, "hans\npeter"));

        /* minimal text CR */
        /* ----------------- */
        memset(src.cstr, '\0', src.capacity);
        CHECK(snprintf(src.cstr, src.capacity - 1, "hans\rpeter"));
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, "hans\npeter"));

        /* complex text CRLF */
        /* ----------------- */
        memset(src.cstr, '\0', src.capacity);
        CHECK(snprintf(
                src.cstr,
                src.capacity - 1,
                "\r\nflower\r\ncar\r\n\r\nhouse\r\n"));
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, "\nflower\ncar\n\nhouse\n"));

        /* complex text CR */
        /* ----------------- */
        memset(src.cstr, '\0', src.capacity);
        CHECK(snprintf(src.cstr, src.capacity - 1, "\rflower\rcar\r\rhouse\r"));
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, "\nflower\ncar\n\nhouse\n"));

        mliStr_free(&src);
        mliStr_free(&dst);
}

CASE("assert no unexpected control codes in ascii-text.")
{
        char txt[32];
        uint8_t i;

        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg("", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg("\n", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg("\0", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg("house", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg("house\n", 0));

        CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg("\r", 0));

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 31, "%c", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 1; i < 9; i++) {
                memset(txt, '\0', sizeof(txt));
                CHECK(snprintf(txt, 31, "%c", i));
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 31, "%c", 9));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 31, "%c", 10));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 11; i < 32; i++) {
                memset(txt, '\0', sizeof(txt));
                CHECK(snprintf(txt, 31, "%c", i));
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        CHECK(snprintf(txt, 31, "%c", 127));
        CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 128; i < 255; i++) {
                memset(txt, '\0', sizeof(txt));
                CHECK(snprintf(txt, 31, "%c", i));
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }
}

CASE("line info fprint")
{
        struct mliStr s = mliStr_init();
        FILE *f;
        CHECK(mliStr_malloc_from_path(
                &s,
                "merlict/"
                "tests/"
                "resources/"
                "sceneries/"
                "002/"
                "objects/"
                "cube_with_materials.obj"));
        f = fopen("merlict/tests/resources/lines_info.tmp", "w");
        CHECK(f);
        CHECK(mli_cstr_lines_fprint(f, s.cstr, 1, 3));
        CHECK(mli_cstr_lines_fprint(f, s.cstr, 10, 3));
        CHECK(mli_cstr_lines_fprint(f, s.cstr, 35, 3));
        fclose(f);

        mliStr_free(&s);
}

CASE("mliStr")
{
        uint64_t i;
        struct mliStr s = mliStr_init();
        CHECK(s.capacity == 0u);
        CHECK(s.length == 0u);
        CHECK(s.cstr == NULL);

        CHECK(mliStr_malloc(&s));
        CHECK(s.length == 0);
        CHECK(s.capacity == 2);
        CHECK(s.cstr[0] == '\0');
        CHECK(s.cstr[1] == '\0');

        CHECK(mliStr_add_cstr(&s, "012"));

        CHECK(s.capacity > s.length);
        CHECK(s.length == 3);
        CHECK(s.cstr[0] == '0');
        CHECK(s.cstr[1] == '1');
        CHECK(s.cstr[2] == '2');
        CHECK(s.cstr[3] == '\0');
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.cstr[i] == '\0');
        }

        CHECK(mliStr_add_cstr(&s, "\n"));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 4);
        CHECK(s.cstr[3] == '\n');
        CHECK(s.cstr[4] == '\0');
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.cstr[i] == '\0');
        }

        CHECK(mliStr_add_cstr(&s, "456"));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 7);
        CHECK(s.cstr[4] == '4');
        CHECK(s.cstr[5] == '5');
        CHECK(s.cstr[6] == '6');
        CHECK(s.cstr[7] == '\0');
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.cstr[i] == '\0');
        }

        CHECK(mliStr_add_cstr(&s, ""));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 7);
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.cstr[i] == '\0');
        }

        CHECK(mliStr_add_cstr(&s, "\0"));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 7);
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.cstr[i] == '\0');
        }

        mliStr_free(&s);
        CHECK(s.capacity == 0u);
        CHECK(s.length == 0u);
        CHECK(s.cstr == NULL);
}

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

        sprintf(str, "first\nsecond\n\nthird");
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

        sprintf(src, "/a/b/c");
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, src));

        sprintf(src, "./a/b/c");
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a/b/c"));
        CHECK(0 == strcmp(src, "./a/b/c"));

        sprintf(src, "./functions/hans.csv");
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "functions/hans.csv"));
        CHECK(0 == strcmp(src, "./functions/hans.csv"));

        sprintf(src, "././././f/h.csv");
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "f/h.csv"));
        CHECK(0 == strcmp(src, "././././f/h.csv"));

        sprintf(src, "a/b");
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a/b"));
        CHECK(0 == strcmp(src, "a/b"));

        sprintf(src, ".a/b");
        mli_cstr_path_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, ".a/b"));
        CHECK(0 == strcmp(src, ".a/b"));

        sprintf(src, "a./b");
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
        sprintf(txt, "\r\n");
        CHECK(mli_cstr_is_CRLF(&txt[0]));
        CHECK(mli_cstr_is_CR(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "01\r\n23");
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
        sprintf(txt, "\r");
        CHECK(!mli_cstr_is_CRLF(&txt[0]));
        CHECK(mli_cstr_is_CR(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "0\r1");
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

        CHECK(mliStr_malloc(&src, 32));
        CHECK(mliStr_malloc(&dst, 32));

        /* all '\0' */
        /* -------- */
        CHECK(src.cstr[0] == '\0');
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(dst.cstr[0] == '\0');

        /* minimal CR */
        /* ---------- */
        memset((char *)src.cstr, '\0', src.length);
        sprintf((char *)src.cstr, "\r");
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "\n"));

        /* minimal CRLF */
        /* ------------ */
        memset((char *)src.cstr, '\0', src.length);
        sprintf((char *)src.cstr, "\r\n");
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "\n"));

        /* minimal text CRLF */
        /* ----------------- */
        memset((char *)src.cstr, '\0', src.length);
        sprintf((char *)src.cstr, "hans\r\npeter");
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "hans\npeter"));

        /* minimal text CR */
        /* ----------------- */
        memset((char *)src.cstr, '\0', src.length);
        sprintf((char *)src.cstr, "hans\rpeter");
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "hans\npeter"));

        /* complex text CRLF */
        /* ----------------- */
        memset((char *)src.cstr, '\0', src.length);
        sprintf((char *)src.cstr, "\r\nflower\r\ncar\r\n\r\nhouse\r\n");
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "\nflower\ncar\n\nhouse\n"));

        /* complex text CR */
        /* ----------------- */
        memset((char *)src.cstr, '\0', src.length);
        sprintf((char *)src.cstr, "\rflower\rcar\r\rhouse\r");
        CHECK(mliStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "\nflower\ncar\n\nhouse\n"));

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
        sprintf(txt, "%c", 0);
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 1; i < 9; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 9);
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 10);
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 11; i < 32; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 127);
        CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 128; i < 255; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }
}

CASE("line info fprint")
{
        struct mliIo s = mliIo_init();
        FILE *f;
        CHECK(mliIo_malloc_from_path(
                &s,
                "tests/"
                "resources/"
                "sceneries/"
                "002/"
                "geometry/"
                "objects/"
                "cube_with_materials.obj"));
        f = fopen("tests/resources/lines_info.tmp", "w");
        CHECK(f);
        CHECK(mli_cstr_lines_fprint(f, (char *)s.cstr, 1, 3));
        CHECK(mli_cstr_lines_fprint(f, (char *)s.cstr, 10, 3));
        CHECK(mli_cstr_lines_fprint(f, (char *)s.cstr, 35, 3));
        fclose(f);

        mliIo_free(&s);
}

CASE("basename")
{
        char filename[256] = {'\0'};
        char key[256] = {'\0'};
        char expected[256] = {'\0'};
        sprintf(filename, "geometry/objects/rale.obj");
        sprintf(expected, "rale");
        mli_cstr_path_basename_without_extension(filename, key);
        mli_cstr_path_basename_without_extension(key, key);
        fprintf(stderr, "%s vs %s\n", key, expected);
        CHECK(strcmp(key, expected) == 0);
}

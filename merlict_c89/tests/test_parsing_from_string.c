/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("string to int")
{
        int64_t i;
        char s[256];

        /* Simple case. */
        sprintf(s, "11");
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == 11);

        /* Negative number . */
        sprintf(s, "-11");
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == -11);

        /* Different base. */
        sprintf(s, "11");
        CHECK(mli_string_to_int(&i, s, 16));
        CHECK(i == 17);

        /* 0 */
        sprintf(s, "0");
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == 0);

        /* INT_MAX. */
        sprintf(s, "%d", INT_MAX);
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == INT_MAX);

        /* INT_MIN. */
        sprintf(s, "%d", INT_MIN);
        CHECK(mli_string_to_int(&i, s, 10));
        CHECK(i == INT_MIN);

        /* Leading and trailing space. */
        sprintf(s, " 1");
        CHECK(!mli_string_to_int(&i, s, 10));
        sprintf(s, "1 ");
        CHECK(!mli_string_to_int(&i, s, 10));
        CHECK(mli_nstring_to_int(&i, s, 10, 1));
        CHECK(i == 1);

        /* Trash characters. */
        sprintf(s, "a10 ");
        CHECK(!mli_string_to_int(&i, s, 10));
        sprintf(s, "10a ");
        CHECK(!mli_string_to_int(&i, s, 10));
        CHECK(mli_nstring_to_int(&i, s, 10, 2));
        CHECK(i == 10);

        /* long overflow */
        sprintf(s, "%ld0", LONG_MAX);
        CHECK(!mli_string_to_int(&i, s, 10));

        /* long underflow */
        sprintf(s, "%ld0", LONG_MIN);
        CHECK(!mli_string_to_int(&i, s, 10));
}

CASE("uint to string")
{
        char s[64] = {'\0'};

        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(0, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "0") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(123, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "123") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(1233456789, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "1233456789") == 0);

        /* octal */
        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(0, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "0") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(1337, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "2471") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(42, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "52") == 0);

        /* binary */
        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(13, s, sizeof(s), 2u, 0u));
        CHECK(strcmp(s, "1101") == 0);

        CHECK(!mli_uint_to_string(13, s, sizeof(s), 1u, 0u));
        CHECK(!mli_uint_to_string(13, s, sizeof(s), 11u, 0u));

        /* leading zeros */
        memset(s, '\0', sizeof(s));
        CHECK(mli_uint_to_string(123, s, sizeof(s), 10u, 9u));
        CHECK(strcmp(s,
                     "000"
                     "000"
                     "123") == 0);
}

CASE("string to float")
{
        double i;
        char s[256];

        /* Simple case. */
        sprintf(s, "11.");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 11.);

        /* Negative number . */
        sprintf(s, "-11.");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == -11);

        sprintf(s, ".11");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == .11);

        /* 0 */
        sprintf(s, "0");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 0);
        sprintf(s, "0.");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 0);
        sprintf(s, ".0");
        CHECK(mli_string_to_float(&i, s));
        CHECK(i == 0);

        /* Leading and trailing space. */
        sprintf(s, " 1");
        CHECK(!mli_string_to_float(&i, s));
        sprintf(s, "1 ");
        CHECK(!mli_string_to_float(&i, s));
        CHECK(mli_nstring_to_float(&i, s, 1));
        CHECK(i == 1.0);

        /* Trash characters. */
        sprintf(s, "a10");
        CHECK(!mli_string_to_float(&i, s));
        sprintf(s, "10a");
        CHECK(!mli_string_to_float(&i, s));
        CHECK(mli_nstring_to_float(&i, s, 2));
        CHECK(i == 10.0);

        /* long overflow */
        sprintf(s, "9.9e9999");
        CHECK(!mli_string_to_float(&i, s));

        /* long underflow */
        sprintf(s, "-9.9e9999");
        CHECK(!mli_string_to_float(&i, s));
}

CASE("mli_string_ends_with")
{
        CHECK(mli_string_ends_with("123", ""));
        CHECK(mli_string_ends_with("", ""));
        CHECK(!mli_string_ends_with("", "123"));
        CHECK(mli_string_ends_with("123", "123"));
        CHECK(mli_string_ends_with("my_file.json", ".json"));
        CHECK(!mli_string_ends_with("my_file.json.stuff", ".json"));
}

CASE("mli_string_starts_with")
{
        CHECK(mli_string_starts_with("123", ""));
        CHECK(mli_string_starts_with("", ""));
        CHECK(!mli_string_starts_with("", "123"));
        CHECK(mli_string_starts_with("123", "123"));
        CHECK(mli_string_starts_with("my_file.json", "my_file"));
        CHECK(mli_string_starts_with("my_file.json.stuff", "my_file.json"));
        CHECK(mli_string_starts_with("functions/hans.csv", "functions/"));
}

CASE("mli_string_has_prefix_suffix")
{
        CHECK(mli_string_has_prefix_suffix("", "", ""));
        CHECK(mli_string_has_prefix_suffix("", NULL, NULL));
        CHECK(mli_string_has_prefix_suffix("abc", NULL, NULL));
        CHECK(mli_string_has_prefix_suffix("abc", "a", "c"));
        CHECK(!mli_string_has_prefix_suffix("abc", "a", "d"));
        CHECK(!mli_string_has_prefix_suffix("_abc", "a", "c"));
}

CASE("mli_string_split_valid")
{
        char token[128];
        char str[128];
        char delimiter = '\n';
        int token_size;
        int p = 0;

        sprintf(str, "first\nsecond\n\nthird");
        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 5);
        CHECK(0 == strcmp(token, "first"));
        p += token_size + 1;

        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 6);
        CHECK(0 == strcmp(token, "second"));
        p += token_size + 1;

        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 0);
        CHECK(0 == strcmp(token, ""));
        p += token_size + 1;

        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 5);
        CHECK(0 == strcmp(token, "third"));
        CHECK(str[p + token_size] == '\0');
}

CASE("mli_string_split_empty")
{
        char token[128];
        char str[128];
        char delimiter = '\n';
        int token_size;
        int p = 0;

        memset(str, '\0', 128);
        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 0);
        CHECK(token[0] == '\0');
}

CASE("mli_strip_this_dir")
{
        char src[128];
        char dst[128];

        CHECK(0 == strcmp("", ""));

        memset(src, '\0', sizeof(src));
        memset(dst, '\0', sizeof(dst));

        sprintf(src, "/a/b/c");
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, src));

        sprintf(src, "./a/b/c");
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a/b/c"));
        CHECK(0 == strcmp(src, "./a/b/c"));

        sprintf(src, "./functions/hans.csv");
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "functions/hans.csv"));
        CHECK(0 == strcmp(src, "./functions/hans.csv"));

        sprintf(src, "././././f/h.csv");
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "f/h.csv"));
        CHECK(0 == strcmp(src, "././././f/h.csv"));

        sprintf(src, "a/b");
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a/b"));
        CHECK(0 == strcmp(src, "a/b"));

        sprintf(src, ".a/b");
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, ".a/b"));
        CHECK(0 == strcmp(src, ".a/b"));

        sprintf(src, "a./b");
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strcmp(dst, "a./b"));
        CHECK(0 == strcmp(src, "a./b"));

        memset(src, '\0', sizeof(src));
        _mli_strip_this_dir(dst, src);
        CHECK(0 == strlen(src));
        CHECK(0 == strlen(dst));
        CHECK(0 == strcmp(dst, ""));
        CHECK(0 == strcmp(src, ""));
}

CASE("find CRLF and CR linebreaks")
{
        char txt[128];
        memset(txt, '\0', sizeof(txt));
        CHECK(!_mli_is_CRLF_line_break(&txt[0]));
        CHECK(!_mli_is_CR_line_break(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "\r\n");
        CHECK(_mli_is_CRLF_line_break(&txt[0]));
        CHECK(_mli_is_CR_line_break(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "01\r\n23");
        CHECK(!_mli_is_CRLF_line_break(&txt[0]));
        CHECK(!_mli_is_CR_line_break(&txt[0]));

        CHECK(!_mli_is_CRLF_line_break(&txt[1]));
        CHECK(!_mli_is_CR_line_break(&txt[1]));

        CHECK(_mli_is_CRLF_line_break(&txt[2]));
        CHECK(_mli_is_CR_line_break(&txt[2]));

        CHECK(!_mli_is_CRLF_line_break(&txt[3]));
        CHECK(!_mli_is_CR_line_break(&txt[3]));

        CHECK(!_mli_is_CRLF_line_break(&txt[4]));
        CHECK(!_mli_is_CR_line_break(&txt[4]));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "\r");
        CHECK(!_mli_is_CRLF_line_break(&txt[0]));
        CHECK(_mli_is_CR_line_break(&txt[0]));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "0\r1");
        CHECK(!_mli_is_CRLF_line_break(&txt[0]));
        CHECK(!_mli_is_CR_line_break(&txt[0]));

        CHECK(!_mli_is_CRLF_line_break(&txt[1]));
        CHECK(_mli_is_CR_line_break(&txt[1]));

        CHECK(!_mli_is_CRLF_line_break(&txt[2]));
        CHECK(!_mli_is_CR_line_break(&txt[2]));
}

CASE("replace CRLF and CR linebreaks with LF")
{
        struct mliDynStr src = mliDynStr_init();
        struct mliDynStr dst = mliDynStr_init();

        CHECK(mliDynStr_malloc(&src, 32));
        CHECK(mliDynStr_malloc(&dst, 32));

        /* all '\0' */
        /* -------- */
        CHECK(src.c_str[0] == '\0');
        CHECK(mliDynStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(dst.c_str[0] == '\0');

        /* minimal CR */
        /* ---------- */
        memset(src.c_str, '\0', src.capacity);
        sprintf(src.c_str, "\r");
        CHECK(mliDynStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.c_str, "\n"));

        /* minimal CRLF */
        /* ------------ */
        memset(src.c_str, '\0', src.capacity);
        sprintf(src.c_str, "\r\n");
        CHECK(mliDynStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.c_str, "\n"));

        /* minimal text CRLF */
        /* ----------------- */
        memset(src.c_str, '\0', src.capacity);
        sprintf(src.c_str, "hans\r\npeter");
        CHECK(mliDynStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.c_str, "hans\npeter"));

        /* minimal text CR */
        /* ----------------- */
        memset(src.c_str, '\0', src.capacity);
        sprintf(src.c_str, "hans\rpeter");
        CHECK(mliDynStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.c_str, "hans\npeter"));

        /* complex text CRLF */
        /* ----------------- */
        memset(src.c_str, '\0', src.capacity);
        sprintf(src.c_str, "\r\nflower\r\ncar\r\n\r\nhouse\r\n");
        CHECK(mliDynStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.c_str, "\nflower\ncar\n\nhouse\n"));

        /* complex text CR */
        /* ----------------- */
        memset(src.c_str, '\0', src.capacity);
        sprintf(src.c_str, "\rflower\rcar\r\rhouse\r");
        CHECK(mliDynStr_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(0 == strcmp(dst.c_str, "\nflower\ncar\n\nhouse\n"));

        mliDynStr_free(&src);
        mliDynStr_free(&dst);
}

CASE("assert no unexpected control codes in ascii-text.")
{
        char txt[32];
        uint8_t i;

        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg("", 0));
        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg("\n", 0));
        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg("\0", 0));
        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg("house", 0));
        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg("house\n", 0));

        CHECK(!mli_string_assert_only_NUL_LF_TAB_controls_dbg("\r", 0));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 0);
        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 1; i < 9; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_string_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 9);
        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 10);
        CHECK(mli_string_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 11; i < 32; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_string_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 127);
        CHECK(!mli_string_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));

        for (i = 128; i < 255; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_string_assert_only_NUL_LF_TAB_controls_dbg(txt, 0));
        }
}

CASE("line info fprint")
{
        struct mliDynStr s = mliDynStr_init();
        FILE *f;
        CHECK(mliDynStr_malloc_from_path(
                &s,
                "merlict_c89/"
                "tests/"
                "resources/"
                "sceneries/"
                "002/"
                "objects/"
                "cube_with_materials.obj"));
        f = fopen("merlict_c89/tests/resources/lines_info.tmp", "w");
        CHECK(f);
        CHECK(mli_lines_info_fprint(f, s.c_str, 1, 3));
        CHECK(mli_lines_info_fprint(f, s.c_str, 10, 3));
        CHECK(mli_lines_info_fprint(f, s.c_str, 35, 3));
        fclose(f);

        mliDynStr_free(&s);
}

CASE("DynStr")
{
        uint64_t i;
        struct mliDynStr s = mliDynStr_init();
        CHECK(s.capacity == 0u);
        CHECK(s.length == 0u);
        CHECK(s.c_str == NULL);

        CHECK(mliDynStr_malloc(&s, 0u));
        CHECK(s.length == 0);
        CHECK(s.capacity == 2);
        CHECK(s.c_str[0] == '\0');
        CHECK(s.c_str[1] == '\0');

        CHECK(mliDynStr_push_back(&s, "012"));

        CHECK(s.capacity > s.length);
        CHECK(s.length == 3);
        CHECK(s.c_str[0] == '0');
        CHECK(s.c_str[1] == '1');
        CHECK(s.c_str[2] == '2');
        CHECK(s.c_str[3] == '\0');
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.c_str[i] == '\0');
        }

        CHECK(mliDynStr_push_back(&s, "\n"));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 4);
        CHECK(s.c_str[3] == '\n');
        CHECK(s.c_str[4] == '\0');
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.c_str[i] == '\0');
        }

        CHECK(mliDynStr_push_back(&s, "456"));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 7);
        CHECK(s.c_str[4] == '4');
        CHECK(s.c_str[5] == '5');
        CHECK(s.c_str[6] == '6');
        CHECK(s.c_str[7] == '\0');
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.c_str[i] == '\0');
        }

        CHECK(mliDynStr_push_back(&s, ""));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 7);
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.c_str[i] == '\0');
        }

        CHECK(mliDynStr_push_back(&s, "\0"));
        CHECK(s.capacity > s.length);
        CHECK(s.length == 7);
        for (i = s.length; i < s.capacity; i++) {
                CHECK(s.c_str[i] == '\0');
        }

        mliDynStr_free(&s);
        CHECK(s.capacity == 0u);
        CHECK(s.length == 0u);
        CHECK(s.c_str == NULL);
}

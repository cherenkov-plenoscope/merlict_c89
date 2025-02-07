/* Copyright 2019-2020 Sebastian Achim Mueller                                */

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

CASE("assert no unexpected control codes in ascii-text.")
{
        char txt[32];
        uint8_t i;

        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg("", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg("\n", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg("\0", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg("house", 0));
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg("house\n", 0));

        CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls__dbg("\r", 0));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 0);
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(txt, 0));

        for (i = 1; i < 9; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 9);
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(txt, 0));

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 10);
        CHECK(mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(txt, 0));

        for (i = 11; i < 32; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(txt, 0));
        }

        memset(txt, '\0', sizeof(txt));
        sprintf(txt, "%c", 127);
        CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(txt, 0));

        for (i = 128; i < 255; i++) {
                memset(txt, '\0', sizeof(txt));
                sprintf(txt, "%c", i);
                CHECK(!mli_cstr_assert_only_NUL_LF_TAB_controls__dbg(txt, 0));
        }
}

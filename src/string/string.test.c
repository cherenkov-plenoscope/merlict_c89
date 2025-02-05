/* Copyright Sebastian Achim Mueller                                          */

CASE("mli_String_rfind")
{
        struct mli_String str = mli_String_init();

        mli_String_free(&str);
        CHECK(mli_String_find(&str, 'a') == -1);
        CHECK(mli_String_rfind(&str, 'a') == -1);

        CHECK(mli_String_from_cstr_fromat(&str, ""));
        CHECK(mli_String_find(&str, 'a') == -1);
        CHECK(mli_String_rfind(&str, 'a') == -1);

        CHECK(mli_String_from_cstr_fromat(&str, "a"));
        CHECK(mli_String_find(&str, 'a') == 0);
        CHECK(mli_String_rfind(&str, 'a') == 0);

        CHECK(mli_String_from_cstr_fromat(&str, "abc"));
        CHECK(mli_String_find(&str, 'a') == 0);
        CHECK(mli_String_rfind(&str, 'a') == 0);

        CHECK(mli_String_from_cstr_fromat(&str, "123abc"));
        CHECK(mli_String_find(&str, 'a') == 3);
        CHECK(mli_String_rfind(&str, 'a') == 3);
        CHECK(mli_String_rfind(&str, 'c') == 5);

        CHECK(mli_String_from_cstr_fromat(&str, "123abc"));
        CHECK(mli_String_find(&str, 'A') == -1);
        CHECK(mli_String_rfind(&str, 'A') == -1);

        mli_String_free(&str);
}

CASE("mli_String_ends_with")
{
        struct mli_String one = mli_String_init();
        struct mli_String two = mli_String_init();

        CHECK(mli_String_from_cstr_fromat(&one, "123"));
        CHECK(mli_String_from_cstr_fromat(&two, ""));
        CHECK(mli_String_ends_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, ""));
        CHECK(mli_String_from_cstr_fromat(&two, ""));
        CHECK(mli_String_ends_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, ""));
        CHECK(mli_String_from_cstr_fromat(&two, "123"));
        CHECK(!mli_String_ends_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, "123"));
        CHECK(mli_String_from_cstr_fromat(&two, "123"));
        CHECK(mli_String_ends_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, "my_file.json"));
        CHECK(mli_String_from_cstr_fromat(&two, ".json"));
        CHECK(mli_String_ends_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, "my_file.json.stuff"));
        CHECK(mli_String_from_cstr_fromat(&two, ".json"));
        CHECK(!mli_String_ends_with(&one, &two));

        mli_String_free(&one);
        mli_String_free(&two);
}

CASE("mli_String_starts_with")
{
        struct mli_String one = mli_String_init();
        struct mli_String two = mli_String_init();

        CHECK(mli_String_from_cstr_fromat(&one, "123"));
        CHECK(mli_String_from_cstr_fromat(&two, ""));
        CHECK(mli_String_starts_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, ""));
        CHECK(mli_String_from_cstr_fromat(&two, ""));
        CHECK(mli_String_starts_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, ""));
        CHECK(mli_String_from_cstr_fromat(&two, "123"));
        CHECK(!mli_String_starts_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, "123"));
        CHECK(mli_String_from_cstr_fromat(&two, "123"));
        CHECK(mli_String_starts_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, "my_file.json"));
        CHECK(mli_String_from_cstr_fromat(&two, "my_file"));
        CHECK(mli_String_starts_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, "my_file.json.stuff"));
        CHECK(mli_String_from_cstr_fromat(&two, "my_file.json"));
        CHECK(mli_String_starts_with(&one, &two));

        CHECK(mli_String_from_cstr_fromat(&one, "functions/hans.csv"));
        CHECK(mli_String_from_cstr_fromat(&two, "functions/"));
        CHECK(mli_String_starts_with(&one, &two));

        mli_String_free(&one);
        mli_String_free(&two);
}

CASE("mli_String_has_prefix_suffix")
{
        struct mli_String str = mli_String_init();
        struct mli_String pre = mli_String_init();
        struct mli_String suf = mli_String_init();

        CHECK(mli_String_from_cstr_fromat(&str, ""));
        CHECK(mli_String_from_cstr_fromat(&pre, ""));
        CHECK(mli_String_from_cstr_fromat(&suf, ""));
        CHECK(mli_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mli_String_from_cstr_fromat(&str, ""));
        mli_String_free(&pre);
        mli_String_free(&suf);
        CHECK(mli_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mli_String_from_cstr_fromat(&str, "abc"));
        mli_String_free(&pre);
        mli_String_free(&suf);
        CHECK(mli_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mli_String_from_cstr_fromat(&str, "abc"));
        CHECK(mli_String_from_cstr_fromat(&pre, "a"));
        CHECK(mli_String_from_cstr_fromat(&suf, "c"));
        CHECK(mli_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mli_String_from_cstr_fromat(&str, "abc"));
        CHECK(mli_String_from_cstr_fromat(&pre, "a"));
        CHECK(mli_String_from_cstr_fromat(&suf, "d"));
        CHECK(!mli_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mli_String_from_cstr_fromat(&str, "_abc"));
        CHECK(mli_String_from_cstr_fromat(&pre, "a"));
        CHECK(mli_String_from_cstr_fromat(&suf, "c"));
        CHECK(!mli_String_has_prefix_suffix(&str, &pre, &suf));

        mli_String_free(&str);
        mli_String_free(&pre);
        mli_String_free(&suf);
}

CASE("mli_String_copy")
{
        struct mli_String src = mli_String_init();
        struct mli_String dst = mli_String_init();

        CHECK(mli_String_from_cstr_fromat(&src, "abb"));
        CHECK(mli_String_copy(&dst, &src));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mli_String_from_cstr_fromat(&src, ""));
        CHECK(mli_String_copy(&dst, &src));
        CHECK(0 == strcmp(dst.array, ""));

        mli_String_free(&src);
        CHECK(!mli_String_copy(&dst, &src));

        mli_String_free(&src);
        mli_String_free(&dst);
}

CASE("mli_String_strip")
{
        struct mli_String src = mli_String_init();
        struct mli_String dst = mli_String_init();

        CHECK(mli_String_from_cstr_fromat(&src, ""));
        CHECK(mli_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, ""));

        CHECK(mli_String_from_cstr_fromat(&src, "abb"));
        CHECK(mli_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mli_String_from_cstr_fromat(&src, "  abb"));
        CHECK(mli_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mli_String_from_cstr_fromat(&src, "abb  "));
        CHECK(mli_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mli_String_from_cstr_fromat(&src, "   abb  "));
        CHECK(mli_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mli_String_from_cstr_fromat(&src, "   a b  b  "));
        CHECK(mli_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "a b  b"));

        mli_String_free(&src);
        CHECK(!mli_String_strip(&src, &dst));

        mli_String_free(&src);
        mli_String_free(&dst);
}

CASE("mli_String_strip")
{
        struct mli_String src = mli_String_init();

        mli_String_countn(&src, 'a', 100);

        CHECK(mli_String_from_cstr_fromat(&src, "   a b  b  "));
        CHECK(2 == mli_String_countn(&src, 'b', src.size));

        CHECK(mli_String_from_cstr_fromat(&src, "a b  b  "));
        CHECK(1 == mli_String_countn(&src, 'b', 3));

        mli_String_free(&src);
}

CASE("mli_String_copyn")
{
        struct mli_String src = mli_String_init();
        struct mli_String dst = mli_String_init();

        CHECK(mli_String_from_cstr_fromat(&src, "123456789.json"));

        CHECK(mli_String_copyn(&dst, &src, 0, src.size));
        CHECK(0 == strcmp(dst.array, "123456789.json"));

        CHECK(!mli_String_copyn(&dst, &src, 0, src.size + 1));

        CHECK(mli_String_copyn(&dst, &src, 0, 3));
        CHECK(0 == strcmp(dst.array, "123"));

        CHECK(mli_String_copyn(&dst, &src, 8, 3));
        CHECK(0 == strcmp(dst.array, "9.j"));

        CHECK(mli_String_copyn(&dst, &src, 8, 0));
        CHECK(0 == strcmp(dst.array, ""));

        mli_String_free(&dst);
        mli_String_free(&src);
}

CASE("replace CRLF and CR linebreaks with LF")
{
        struct mli_String src = mli_String_init();
        struct mli_String dst = mli_String_init();

        /* all '\0' */
        /* -------- */
        CHECK(mli_String_from_cstr_fromat(&src, ""));
        CHECK(mli_String_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(mli_String_equal_cstr(&dst, ""));

        /* minimal CR */
        /* ---------- */
        CHECK(mli_String_from_cstr_fromat(&src, "\r"));
        CHECK(mli_String_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(mli_String_equal_cstr(&dst, "\n"));

        /* minimal CRLF */
        /* ------------ */
        CHECK(mli_String_from_cstr_fromat(&src, "\r\n"));
        CHECK(mli_String_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(mli_String_equal_cstr(&dst, "\n"));

        /* minimal text CRLF */
        /* ----------------- */
        CHECK(mli_String_from_cstr_fromat(&src, "hans\r\npeter"));
        CHECK(mli_String_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(mli_String_equal_cstr(&dst, "hans\npeter"));

        /* minimal text CR */
        /* ----------------- */
        CHECK(mli_String_from_cstr_fromat(&src, "hans\rpeter"));
        CHECK(mli_String_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(mli_String_equal_cstr(&dst, "hans\npeter"));

        /* complex text CRLF */
        /* ----------------- */
        CHECK(mli_String_from_cstr_fromat(
                &src, "\r\nflower\r\ncar\r\n\r\nhouse\r\n"));
        CHECK(mli_String_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(mli_String_equal_cstr(&dst, "\nflower\ncar\n\nhouse\n"));

        /* complex text CR */
        /* ----------------- */
        CHECK(mli_String_from_cstr_fromat(&src, "\rflower\rcar\r\rhouse\r"));
        CHECK(mli_String_convert_line_break_CRLF_CR_to_LF(&dst, &src));
        CHECK(mli_String_equal_cstr(&dst, "\nflower\ncar\n\nhouse\n"));

        mli_String_free(&src);
        mli_String_free(&dst);
}

CASE("string valid")
{
        struct mli_String s = mli_String_init();
        CHECK(!mli_String_valid(&s, 0));

        CHECK(mli_String_from_cstr_fromat(&s, "text"));
        CHECK(mli_String_valid(&s, 0));

        mli_String_free(&s);
}

CASE("serialize an empty string")
{
        struct mli_String s = mli_String_init();
        struct mli_IO f = mli_IO_init();
        CHECK(mli_IO_open_memory(&f));

        CHECK(mli_String_from_cstr_fromat(&s, ""));
        CHECK(mli_String_to_io(&s, &f));

        mli_String_free(&s);
        mli_IO_close(&f);
}

/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("string to int")
{
        int64_t i;
        char s[256] = {'\0'};

        /* Simple case. */
        CHECK(snprintf(s, 255, "11"));
        CHECK(mli_cstr_to_int64(&i, s, 10));
        CHECK(i == 11);

        /* Negative number . */
        CHECK(snprintf(s, 255, "-11"));
        CHECK(mli_cstr_to_int64(&i, s, 10));
        CHECK(i == -11);

        /* Different base. */
        CHECK(snprintf(s, 255, "11"));
        CHECK(mli_cstr_to_int64(&i, s, 16));
        CHECK(i == 17);

        /* 0 */
        CHECK(snprintf(s, 255, "0"));
        CHECK(mli_cstr_to_int64(&i, s, 10));
        CHECK(i == 0);

        /* INT_MAX. */
        CHECK(snprintf(s, 255, "%d", INT_MAX));
        CHECK(mli_cstr_to_int64(&i, s, 10));
        CHECK(i == INT_MAX);

        /* INT_MIN. */
        CHECK(snprintf(s, 255, "%d", INT_MIN));
        CHECK(mli_cstr_to_int64(&i, s, 10));
        CHECK(i == INT_MIN);

        /* Leading and trailing space. */
        CHECK(snprintf(s, 255, " 1"));
        CHECK(!mli_cstr_to_int64(&i, s, 10));
        CHECK(snprintf(s, 255, "1 "));
        CHECK(!mli_cstr_to_int64(&i, s, 10));
        CHECK(mli_cstr_nto_int64(&i, s, 10, 1));
        CHECK(i == 1);

        /* Trash characters. */
        CHECK(snprintf(s, 255, "a10 "));
        CHECK(!mli_cstr_to_int64(&i, s, 10));
        CHECK(snprintf(s, 255, "10a "));
        CHECK(!mli_cstr_to_int64(&i, s, 10));
        CHECK(mli_cstr_nto_int64(&i, s, 10, 2));
        CHECK(i == 10);

        /* long overflow */
        CHECK(snprintf(s, 255, "%ld0", LONG_MAX));
        CHECK(!mli_cstr_to_int64(&i, s, 10));

        /* long underflow */
        CHECK(snprintf(s, 255, "%ld0", LONG_MIN));
        CHECK(!mli_cstr_to_int64(&i, s, 10));
}

CASE("uint to string")
{
        char s[64] = {'\0'};

        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(0, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "0") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(123, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "123") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(1233456789, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "1233456789") == 0);

        /* octal */
        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(0, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "0") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(1337, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "2471") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(42, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "52") == 0);

        /* binary */
        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(13, s, sizeof(s), 2u, 0u));
        CHECK(strcmp(s, "1101") == 0);

        CHECK(!mli_cstr_print_uint64(13, s, sizeof(s), 1u, 0u));
        CHECK(!mli_cstr_print_uint64(13, s, sizeof(s), 11u, 0u));

        /* leading zeros */
        memset(s, '\0', sizeof(s));
        CHECK(mli_cstr_print_uint64(123, s, sizeof(s), 10u, 9u));
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
        CHECK(snprintf(s, 255,"11."));
        CHECK(mli_cstr_to_double(&i, s));
        CHECK(i == 11.);

        /* Negative number . */
        CHECK(snprintf(s, 255,"-11."));
        CHECK(mli_cstr_to_double(&i, s));
        CHECK(i == -11);

        CHECK(snprintf(s, 255,".11"));
        CHECK(mli_cstr_to_double(&i, s));
        CHECK(i == .11);

        /* 0 */
        CHECK(snprintf(s, 255,"0"));
        CHECK(mli_cstr_to_double(&i, s));
        CHECK(i == 0);
        CHECK(snprintf(s, 255,"0."));
        CHECK(mli_cstr_to_double(&i, s));
        CHECK(i == 0);
        CHECK(snprintf(s, 255,".0"));
        CHECK(mli_cstr_to_double(&i, s));
        CHECK(i == 0);

        /* Leading and trailing space. */
        CHECK(snprintf(s, 255," 1"));
        CHECK(!mli_cstr_to_double(&i, s));
        CHECK(snprintf(s, 255,"1 "));
        CHECK(!mli_cstr_to_double(&i, s));
        CHECK(mli_cstr_nto_double(&i, s, 1));
        CHECK(i == 1.0);

        /* Trash characters. */
        CHECK(snprintf(s, 255,"a10"));
        CHECK(!mli_cstr_to_double(&i, s));
        CHECK(snprintf(s, 255,"10a"));
        CHECK(!mli_cstr_to_double(&i, s));
        CHECK(mli_cstr_nto_double(&i, s, 2));
        CHECK(i == 10.0);

        /* long overflow */
        CHECK(snprintf(s, 255,"9.9e9999"));
        CHECK(!mli_cstr_to_double(&i, s));

        /* long underflow */
        CHECK(snprintf(s, 255,"-9.9e9999"));
        CHECK(!mli_cstr_to_double(&i, s));
}

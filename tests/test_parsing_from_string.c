/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("string to int")
{
        int64_t i;
        /* Lazy to calculate this size properly. */
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

        /* Trash characters. */
        sprintf(s, "a10 ");
        CHECK(!mli_string_to_int(&i, s, 10));
        sprintf(s, "10a ");
        CHECK(!mli_string_to_int(&i, s, 10));

        /* long overflow */
        sprintf(s, "%ld0", LONG_MAX);
        CHECK(!mli_string_to_int(&i, s, 10));

        /* long underflow */
        sprintf(s, "%ld0", LONG_MIN);
        CHECK(!mli_string_to_int(&i, s, 10));
}

CASE("string to float")
{
        double i;
        /* Lazy to calculate this size properly. */
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

        /* Trash characters. */
        sprintf(s, "a10");
        CHECK(!mli_string_to_float(&i, s));
        sprintf(s, "10a");
        CHECK(!mli_string_to_float(&i, s));

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
        CHECK(token_size == 6);
        CHECK(0 == strcmp(token, "first"));
        p += token_size;

        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 7);
        CHECK(0 == strcmp(token, "second"));
        p += token_size;

        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 1);
        CHECK(0 == strcmp(token, ""));
        p += token_size;

        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 5);
        CHECK(0 == strcmp(token, "third"));
        p += token_size;

        CHECK(str[p] == '\0');

        token_size = mli_string_split(&str[p], delimiter, token, 128);
        CHECK(token_size == 0);
        CHECK(token[0] == '\0');
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

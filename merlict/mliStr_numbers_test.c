/* Copyright 2019-2020 Sebastian Achim Mueller                                */
#include <limits.h>

CASE("mliStr_nto_int64")
{
        int64_t i;
        struct mliStr s = mliStr_init();

        /* Simple case. */
        mliStr_mallocf(&s, "11");
        CHECK(mliStr_to_int64(&i, &s, 10));
        CHECK(i == 11);

        /* Negative number . */
        mliStr_mallocf(&s, "-11");
        CHECK(mliStr_to_int64(&i, &s, 10));
        CHECK(i == -11);

        /* Different base. */
        mliStr_mallocf(&s, "11");
        CHECK(mliStr_to_int64(&i, &s, 16));
        CHECK(i == 17);

        /* 0 */
        mliStr_mallocf(&s, "0");
        CHECK(mliStr_to_int64(&i, &s, 10));
        CHECK(i == 0);

        /* INT_MAX. */
        mliStr_mallocf(&s, "%d", INT_MAX);
        CHECK(mliStr_to_int64(&i, &s, 10));
        CHECK(i == INT_MAX);

        /* INT_MIN. */
        mliStr_mallocf(&s, "%d", INT_MIN);
        CHECK(mliStr_to_int64(&i, &s, 10));
        CHECK(i == INT_MIN);

        /* Leading and trailing space. */
        mliStr_mallocf(&s, " 1");
        CHECK(!mliStr_to_int64(&i, &s, 10));
        mliStr_mallocf(&s, "1 ");
        CHECK(!mliStr_to_int64(&i, &s, 10));
        CHECK(mliStr_nto_int64(&i, &s, 10, 1));
        CHECK(i == 1);

        /* Trash characters. */
        mliStr_mallocf(&s, "a10 ");
        CHECK(!mliStr_to_int64(&i, &s, 10));
        mliStr_mallocf(&s, "10a ");
        CHECK(!mliStr_to_int64(&i, &s, 10));
        CHECK(mliStr_nto_int64(&i, &s, 10, 2));
        CHECK(i == 10);

        /* long overflow */
        mliStr_mallocf(&s, "%ld0", LONG_MAX);
        CHECK(!mliStr_to_int64(&i, &s, 10));

        /* long underflow */
        mliStr_mallocf(&s, "%ld0", LONG_MIN);
        CHECK(!mliStr_to_int64(&i, &s, 10));

        mliStr_free(&s);
}

CASE("mliStr_to_double")
{
        double i;
        struct mliStr s = mliStr_init();

        /* Simple case. */
        mliStr_mallocf(&s, "11.");
        CHECK(mliStr_to_double(&i, &s));
        CHECK(i == 11.);

        /* Negative number . */
        mliStr_mallocf(&s, "-11.");
        CHECK(mliStr_to_double(&i, &s));
        CHECK(i == -11);

        mliStr_mallocf(&s, ".11");
        CHECK(mliStr_to_double(&i, &s));
        CHECK(i == .11);

        /* 0 */
        mliStr_mallocf(&s, "0");
        CHECK(mliStr_to_double(&i, &s));
        CHECK(i == 0);
        mliStr_mallocf(&s, "0.");
        CHECK(mliStr_to_double(&i, &s));
        CHECK(i == 0);
        mliStr_mallocf(&s, ".0");
        CHECK(mliStr_to_double(&i, &s));
        CHECK(i == 0);

        /* Leading and trailing space. */
        mliStr_mallocf(&s, " 1");
        CHECK(!mliStr_to_double(&i, &s));
        mliStr_mallocf(&s, "1 ");
        CHECK(!mliStr_to_double(&i, &s));
        CHECK(mliStr_nto_double(&i, &s, 1));
        CHECK(i == 1.0);

        /* Trash characters. */
        mliStr_mallocf(&s, "a10");
        CHECK(!mliStr_to_double(&i, &s));
        mliStr_mallocf(&s, "10a");
        CHECK(!mliStr_to_double(&i, &s));
        CHECK(mliStr_nto_double(&i, &s, 2));
        CHECK(i == 10.0);

        /* long overflow */
        mliStr_mallocf(&s, "9.9e9999");
        CHECK(!mliStr_to_double(&i, &s));

        /* long underflow */
        mliStr_mallocf(&s, "-9.9e9999");
        CHECK(!mliStr_to_double(&i, &s));

        mliStr_free(&s);
}

CASE("mliStr_print_uint64")
{
        struct mliStr s = mliStr_init();

        CHECK(mliStr_print_uint64(0, &s, 10u, 0u, '0'));
        CHECK(strcmp(s.cstr, "0") == 0);
        CHECK(strlen(s.cstr) == s.length);

        CHECK(mliStr_print_uint64(123, &s, 10u, 0u, '0'));
        CHECK(strcmp(s.cstr, "123") == 0);
        CHECK(strlen(s.cstr) == s.length);

        CHECK(mliStr_print_uint64(1233456789, &s, 10u, 0u, '0'));
        CHECK(strcmp(s.cstr, "1233456789") == 0);
        CHECK(strlen(s.cstr) == s.length);

        /* octal */
        CHECK(mliStr_print_uint64(0, &s, 8u, 0u, '0'));
        CHECK(strcmp(s.cstr, "0") == 0);
        CHECK(strlen(s.cstr) == s.length);

        CHECK(mliStr_print_uint64(1337, &s, 8u, 0u, '0'));
        CHECK(strcmp(s.cstr, "2471") == 0);
        CHECK(strlen(s.cstr) == s.length);

        CHECK(mliStr_print_uint64(42, &s, 8u, 0u, '0'));
        CHECK(strcmp(s.cstr, "52") == 0);
        CHECK(strlen(s.cstr) == s.length);

        /* binary */
        CHECK(mliStr_print_uint64(13, &s, 2u, 0u, '0'));
        CHECK(strcmp(s.cstr, "1101") == 0);
        CHECK(strlen(s.cstr) == s.length);

        CHECK(!mliStr_print_uint64(13, &s, 1u, 0u, '0'));
        CHECK(strlen(s.cstr) == s.length);

        /* hex */
        CHECK(mliStr_print_uint64(10, &s, 16u, 0u, '0'));
        CHECK(strcmp(s.cstr, "A") == 0);
        CHECK(strlen(s.cstr) == s.length);

        CHECK(mliStr_print_uint64(255, &s, 16u, 0u, '0'));
        CHECK(strcmp(s.cstr, "FF") == 0);
        CHECK(strlen(s.cstr) == s.length);

        /* base too large */
        CHECK(!mliStr_print_uint64(13, &s, 17u, 0u, '0'));
        CHECK(strlen(s.cstr) == s.length);

        /* leading zeros */
        CHECK(mliStr_print_uint64(123, &s, 10u, 9u, '0'));
        CHECK(strcmp(s.cstr,
                     "000"
                     "000"
                     "123") == 0);
        CHECK(strlen(s.cstr) == s.length);

        CHECK(mliStr_print_uint64(123, &s, 10u, 9u, ' '));
        CHECK(strcmp(s.cstr,
                     "   "
                     "   "
                     "123") == 0);
        CHECK(strlen(s.cstr) == s.length);

        mliStr_free(&s);
}

/* Copyright 2019-2020 Sebastian Achim Mueller                                */
#include <limits.h>

CASE("mli_String_nto_int64")
{
        int64_t i;
        struct mli_String s = mli_String_init();

        /* Simple case. */
        mli_String_from_cstr_fromat(&s, "11");
        CHECK(mli_String_to_int64(&i, &s, 10));
        CHECK(i == 11);

        /* Negative number . */
        mli_String_from_cstr_fromat(&s, "-11");
        CHECK(mli_String_to_int64(&i, &s, 10));
        CHECK(i == -11);

        /* Different base. */
        mli_String_from_cstr_fromat(&s, "11");
        CHECK(mli_String_to_int64(&i, &s, 16));
        CHECK(i == 17);

        /* 0 */
        mli_String_from_cstr_fromat(&s, "0");
        CHECK(mli_String_to_int64(&i, &s, 10));
        CHECK(i == 0);

        /* INT_MAX. */
        mli_String_from_cstr_fromat(&s, "%d", INT_MAX);
        CHECK(mli_String_to_int64(&i, &s, 10));
        CHECK(i == INT_MAX);

        /* INT_MIN. */
        mli_String_from_cstr_fromat(&s, "%d", INT_MIN);
        CHECK(mli_String_to_int64(&i, &s, 10));
        CHECK(i == INT_MIN);

        /* Leading and trailing space. */
        mli_String_from_cstr_fromat(&s, " 1");
        CHECK(!mli_String_to_int64(&i, &s, 10));
        mli_String_from_cstr_fromat(&s, "1 ");
        CHECK(!mli_String_to_int64(&i, &s, 10));
        CHECK(mli_String_nto_int64(&i, &s, 10, 1));
        CHECK(i == 1);

        /* Trash characters. */
        mli_String_from_cstr_fromat(&s, "a10 ");
        CHECK(!mli_String_to_int64(&i, &s, 10));
        mli_String_from_cstr_fromat(&s, "10a ");
        CHECK(!mli_String_to_int64(&i, &s, 10));
        CHECK(mli_String_nto_int64(&i, &s, 10, 2));
        CHECK(i == 10);

        /* long overflow */
        mli_String_from_cstr_fromat(&s, "%ld0", LONG_MAX);
        CHECK(!mli_String_to_int64(&i, &s, 10));

        /* long underflow */
        mli_String_from_cstr_fromat(&s, "%ld0", LONG_MIN);
        CHECK(!mli_String_to_int64(&i, &s, 10));

        mli_String_free(&s);
}

CASE("mli_String_to_double")
{
        double i;
        struct mli_String s = mli_String_init();

        /* Simple case. */
        mli_String_from_cstr_fromat(&s, "11.");
        CHECK(mli_String_to_double(&i, &s));
        CHECK(i == 11.);

        /* Negative number . */
        mli_String_from_cstr_fromat(&s, "-11.");
        CHECK(mli_String_to_double(&i, &s));
        CHECK(i == -11);

        mli_String_from_cstr_fromat(&s, ".11");
        CHECK(mli_String_to_double(&i, &s));
        CHECK(i == .11);

        /* 0 */
        mli_String_from_cstr_fromat(&s, "0");
        CHECK(mli_String_to_double(&i, &s));
        CHECK(i == 0);
        mli_String_from_cstr_fromat(&s, "0.");
        CHECK(mli_String_to_double(&i, &s));
        CHECK(i == 0);
        mli_String_from_cstr_fromat(&s, ".0");
        CHECK(mli_String_to_double(&i, &s));
        CHECK(i == 0);

        /* Leading and trailing space. */
        mli_String_from_cstr_fromat(&s, " 1");
        CHECK(!mli_String_to_double(&i, &s));
        mli_String_from_cstr_fromat(&s, "1 ");
        CHECK(!mli_String_to_double(&i, &s));
        CHECK(mli_String_nto_double(&i, &s, 1));
        CHECK(i == 1.0);

        /* Trash characters. */
        mli_String_from_cstr_fromat(&s, "a10");
        CHECK(!mli_String_to_double(&i, &s));
        mli_String_from_cstr_fromat(&s, "10a");
        CHECK(!mli_String_to_double(&i, &s));
        CHECK(mli_String_nto_double(&i, &s, 2));
        CHECK(i == 10.0);

        /* long overflow */
        mli_String_from_cstr_fromat(&s, "9.9e9999");
        CHECK(!mli_String_to_double(&i, &s));

        /* long underflow */
        mli_String_from_cstr_fromat(&s, "-9.9e9999");
        CHECK(!mli_String_to_double(&i, &s));

        mli_String_free(&s);
}

CASE("mli_String_print_uint64")
{
        struct mli_String s = mli_String_init();

        CHECK(mli_String_print_uint64(0, &s, 10u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "0"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        CHECK(mli_String_print_uint64(123, &s, 10u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "123"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        CHECK(mli_String_print_uint64(1233456789, &s, 10u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "1233456789"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        /* octal */
        CHECK(mli_String_print_uint64(0, &s, 8u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "0"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        CHECK(mli_String_print_uint64(1337, &s, 8u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "2471"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        CHECK(mli_String_print_uint64(42, &s, 8u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "52"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        /* binary */
        CHECK(mli_String_print_uint64(13, &s, 2u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "1101"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        CHECK(!mli_String_print_uint64(13, &s, 1u, 0u, '0'));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        /* hex */
        CHECK(mli_String_print_uint64(10, &s, 16u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "A"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        CHECK(mli_String_print_uint64(255, &s, 16u, 0u, '0'));
        CHECK(mli_String_equal_cstr(&s, "FF"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        /* base too large */
        CHECK(!mli_String_print_uint64(13, &s, 17u, 0u, '0'));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        /* leading zeros */
        CHECK(mli_String_print_uint64(123, &s, 10u, 9u, '0'));
        CHECK(mli_String_equal_cstr(
                &s,
                "000"
                "000"
                "123"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        CHECK(mli_String_print_uint64(123, &s, 10u, 9u, ' '));
        CHECK(mli_String_equal_cstr(
                &s,
                "   "
                "   "
                "123"));
        CHECK(mli_String__discover_size(&s) == (int64_t)s.size);

        mli_String_free(&s);
}

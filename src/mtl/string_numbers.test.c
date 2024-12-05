/* Copyright 2019-2020 Sebastian Achim Mueller                                */
#include <limits.h>

CASE("mtl_String_nto_int64")
{
        int64_t i;
        struct mtl_String s = mtl_String_init();

        /* Simple case. */
        mtl_String_from_cstr_fromat(&s, "11");
        CHECK(mtl_String_to_int64(&i, &s, 10));
        CHECK(i == 11);

        /* Negative number . */
        mtl_String_from_cstr_fromat(&s, "-11");
        CHECK(mtl_String_to_int64(&i, &s, 10));
        CHECK(i == -11);

        /* Different base. */
        mtl_String_from_cstr_fromat(&s, "11");
        CHECK(mtl_String_to_int64(&i, &s, 16));
        CHECK(i == 17);

        /* 0 */
        mtl_String_from_cstr_fromat(&s, "0");
        CHECK(mtl_String_to_int64(&i, &s, 10));
        CHECK(i == 0);

        /* INT_MAX. */
        mtl_String_from_cstr_fromat(&s, "%d", INT_MAX);
        CHECK(mtl_String_to_int64(&i, &s, 10));
        CHECK(i == INT_MAX);

        /* INT_MIN. */
        mtl_String_from_cstr_fromat(&s, "%d", INT_MIN);
        CHECK(mtl_String_to_int64(&i, &s, 10));
        CHECK(i == INT_MIN);

        /* Leading and trailing space. */
        mtl_String_from_cstr_fromat(&s, " 1");
        CHECK(!mtl_String_to_int64(&i, &s, 10));
        mtl_String_from_cstr_fromat(&s, "1 ");
        CHECK(!mtl_String_to_int64(&i, &s, 10));
        CHECK(mtl_String_nto_int64(&i, &s, 10, 1));
        CHECK(i == 1);

        /* Trash characters. */
        mtl_String_from_cstr_fromat(&s, "a10 ");
        CHECK(!mtl_String_to_int64(&i, &s, 10));
        mtl_String_from_cstr_fromat(&s, "10a ");
        CHECK(!mtl_String_to_int64(&i, &s, 10));
        CHECK(mtl_String_nto_int64(&i, &s, 10, 2));
        CHECK(i == 10);

        /* long overflow */
        mtl_String_from_cstr_fromat(&s, "%ld0", LONG_MAX);
        CHECK(!mtl_String_to_int64(&i, &s, 10));

        /* long underflow */
        mtl_String_from_cstr_fromat(&s, "%ld0", LONG_MIN);
        CHECK(!mtl_String_to_int64(&i, &s, 10));

        mtl_String_free(&s);
}

CASE("mtl_String_to_double")
{
        double i;
        struct mtl_String s = mtl_String_init();

        /* Simple case. */
        mtl_String_from_cstr_fromat(&s, "11.");
        CHECK(mtl_String_to_double(&i, &s));
        CHECK(i == 11.);

        /* Negative number . */
        mtl_String_from_cstr_fromat(&s, "-11.");
        CHECK(mtl_String_to_double(&i, &s));
        CHECK(i == -11);

        mtl_String_from_cstr_fromat(&s, ".11");
        CHECK(mtl_String_to_double(&i, &s));
        CHECK(i == .11);

        /* 0 */
        mtl_String_from_cstr_fromat(&s, "0");
        CHECK(mtl_String_to_double(&i, &s));
        CHECK(i == 0);
        mtl_String_from_cstr_fromat(&s, "0.");
        CHECK(mtl_String_to_double(&i, &s));
        CHECK(i == 0);
        mtl_String_from_cstr_fromat(&s, ".0");
        CHECK(mtl_String_to_double(&i, &s));
        CHECK(i == 0);

        /* Leading and trailing space. */
        mtl_String_from_cstr_fromat(&s, " 1");
        CHECK(!mtl_String_to_double(&i, &s));
        mtl_String_from_cstr_fromat(&s, "1 ");
        CHECK(!mtl_String_to_double(&i, &s));
        CHECK(mtl_String_nto_double(&i, &s, 1));
        CHECK(i == 1.0);

        /* Trash characters. */
        mtl_String_from_cstr_fromat(&s, "a10");
        CHECK(!mtl_String_to_double(&i, &s));
        mtl_String_from_cstr_fromat(&s, "10a");
        CHECK(!mtl_String_to_double(&i, &s));
        CHECK(mtl_String_nto_double(&i, &s, 2));
        CHECK(i == 10.0);

        /* long overflow */
        mtl_String_from_cstr_fromat(&s, "9.9e9999");
        CHECK(!mtl_String_to_double(&i, &s));

        /* long underflow */
        mtl_String_from_cstr_fromat(&s, "-9.9e9999");
        CHECK(!mtl_String_to_double(&i, &s));

        mtl_String_free(&s);
}

CASE("mtl_String_print_uint64")
{
        struct mtl_String s = mtl_String_init();

        CHECK(mtl_String_print_uint64(0, &s, 10u, 0u, '0'));
        CHECK(strcmp(s.array, "0") == 0);
        CHECK(strlen(s.array) == s.size);

        CHECK(mtl_String_print_uint64(123, &s, 10u, 0u, '0'));
        CHECK(strcmp(s.array, "123") == 0);
        CHECK(strlen(s.array) == s.size);

        CHECK(mtl_String_print_uint64(1233456789, &s, 10u, 0u, '0'));
        CHECK(strcmp(s.array, "1233456789") == 0);
        CHECK(strlen(s.array) == s.size);

        /* octal */
        CHECK(mtl_String_print_uint64(0, &s, 8u, 0u, '0'));
        CHECK(strcmp(s.array, "0") == 0);
        CHECK(strlen(s.array) == s.size);

        CHECK(mtl_String_print_uint64(1337, &s, 8u, 0u, '0'));
        CHECK(strcmp(s.array, "2471") == 0);
        CHECK(strlen(s.array) == s.size);

        CHECK(mtl_String_print_uint64(42, &s, 8u, 0u, '0'));
        CHECK(strcmp(s.array, "52") == 0);
        CHECK(strlen(s.array) == s.size);

        /* binary */
        CHECK(mtl_String_print_uint64(13, &s, 2u, 0u, '0'));
        CHECK(strcmp(s.array, "1101") == 0);
        CHECK(strlen(s.array) == s.size);

        CHECK(!mtl_String_print_uint64(13, &s, 1u, 0u, '0'));
        CHECK(strlen(s.array) == s.size);

        /* hex */
        CHECK(mtl_String_print_uint64(10, &s, 16u, 0u, '0'));
        CHECK(strcmp(s.array, "A") == 0);
        CHECK(strlen(s.array) == s.size);

        CHECK(mtl_String_print_uint64(255, &s, 16u, 0u, '0'));
        CHECK(strcmp(s.array, "FF") == 0);
        CHECK(strlen(s.array) == s.size);

        /* base too large */
        CHECK(!mtl_String_print_uint64(13, &s, 17u, 0u, '0'));
        CHECK(strlen(s.array) == s.size);

        /* leading zeros */
        CHECK(mtl_String_print_uint64(123, &s, 10u, 9u, '0'));
        CHECK(strcmp(s.array,
                     "000"
                     "000"
                     "123") == 0);
        CHECK(strlen(s.array) == s.size);

        CHECK(mtl_String_print_uint64(123, &s, 10u, 9u, ' '));
        CHECK(strcmp(s.array,
                     "   "
                     "   "
                     "123") == 0);
        CHECK(strlen(s.array) == s.size);

        mtl_String_free(&s);
}

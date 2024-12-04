/* Copyright 2019-2020 Sebastian Achim Mueller                                */
#include <limits.h>

CASE("string to int")
{
        int64_t i;
        char s[256];

        /* Simple case. */
        sprintf(s, "11");
        CHECK(mtl_cstr_to_int64(&i, s, 10));
        CHECK(i == 11);

        /* Negative number . */
        sprintf(s, "-11");
        CHECK(mtl_cstr_to_int64(&i, s, 10));
        CHECK(i == -11);

        /* Different base. */
        sprintf(s, "11");
        CHECK(mtl_cstr_to_int64(&i, s, 16));
        CHECK(i == 17);

        /* 0 */
        sprintf(s, "0");
        CHECK(mtl_cstr_to_int64(&i, s, 10));
        CHECK(i == 0);

        /* INT_MAX. */
        sprintf(s, "%d", INT_MAX);
        CHECK(mtl_cstr_to_int64(&i, s, 10));
        CHECK(i == INT_MAX);

        /* INT_MIN. */
        sprintf(s, "%d", INT_MIN);
        CHECK(mtl_cstr_to_int64(&i, s, 10));
        CHECK(i == INT_MIN);

        /* Leading and trailing space. */
        sprintf(s, " 1");
        CHECK(!mtl_cstr_to_int64(&i, s, 10));
        sprintf(s, "1 ");
        CHECK(!mtl_cstr_to_int64(&i, s, 10));
        CHECK(mtl_cstr_nto_int64(&i, s, 10, 1));
        CHECK(i == 1);

        /* Trash characters. */
        sprintf(s, "a10 ");
        CHECK(!mtl_cstr_to_int64(&i, s, 10));
        sprintf(s, "10a ");
        CHECK(!mtl_cstr_to_int64(&i, s, 10));
        CHECK(mtl_cstr_nto_int64(&i, s, 10, 2));
        CHECK(i == 10);

        /* long overflow */
        sprintf(s, "%ld0", LONG_MAX);
        CHECK(!mtl_cstr_to_int64(&i, s, 10));

        /* long underflow */
        sprintf(s, "%ld0", LONG_MIN);
        CHECK(!mtl_cstr_to_int64(&i, s, 10));
}

CASE("uint to string")
{
        char s[64] = {'\0'};

        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(0, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "0") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(123, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "123") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(1233456789, s, sizeof(s), 10u, 0u));
        CHECK(strcmp(s, "1233456789") == 0);

        /* octal */
        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(0, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "0") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(1337, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "2471") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(42, s, sizeof(s), 8u, 0u));
        CHECK(strcmp(s, "52") == 0);

        /* binary */
        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(13, s, sizeof(s), 2u, 0u));
        CHECK(strcmp(s, "1101") == 0);

        CHECK(!mtl_cstr_print_uint64(13, s, sizeof(s), 1u, 0u));

        /* hex */
        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(10, s, sizeof(s), 16u, 0u));
        CHECK(strcmp(s, "A") == 0);

        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(255, s, sizeof(s), 16u, 0u));
        CHECK(strcmp(s, "FF") == 0);

        /* base too large */
        CHECK(!mtl_cstr_print_uint64(13, s, sizeof(s), 17u, 0u));

        /* leading zeros */
        memset(s, '\0', sizeof(s));
        CHECK(mtl_cstr_print_uint64(123, s, sizeof(s), 10u, 9u));
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
        CHECK(mtl_cstr_to_double(&i, s));
        CHECK(i == 11.);

        /* Negative number . */
        sprintf(s, "-11.");
        CHECK(mtl_cstr_to_double(&i, s));
        CHECK(i == -11);

        sprintf(s, ".11");
        CHECK(mtl_cstr_to_double(&i, s));
        CHECK(i == .11);

        /* 0 */
        sprintf(s, "0");
        CHECK(mtl_cstr_to_double(&i, s));
        CHECK(i == 0);
        sprintf(s, "0.");
        CHECK(mtl_cstr_to_double(&i, s));
        CHECK(i == 0);
        sprintf(s, ".0");
        CHECK(mtl_cstr_to_double(&i, s));
        CHECK(i == 0);

        /* Leading and trailing space. */
        sprintf(s, " 1");
        CHECK(!mtl_cstr_to_double(&i, s));
        sprintf(s, "1 ");
        CHECK(!mtl_cstr_to_double(&i, s));
        CHECK(mtl_cstr_nto_double(&i, s, 1));
        CHECK(i == 1.0);

        /* Trash characters. */
        sprintf(s, "a10");
        CHECK(!mtl_cstr_to_double(&i, s));
        sprintf(s, "10a");
        CHECK(!mtl_cstr_to_double(&i, s));
        CHECK(mtl_cstr_nto_double(&i, s, 2));
        CHECK(i == 10.0);

        /* long overflow */
        sprintf(s, "9.9e9999");
        CHECK(!mtl_cstr_to_double(&i, s));

        /* long underflow */
        sprintf(s, "-9.9e9999");
        CHECK(!mtl_cstr_to_double(&i, s));
}

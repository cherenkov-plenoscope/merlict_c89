/* Copyright Sebastian Achim Mueller                                          */

#include "../../mli_testing/src/mli_testing.h"

CASE("mtl_String_rfind")
{
        struct mtl_String str = mtl_String_init();

        mtl_String_free(&str);
        CHECK(mtl_String_find(&str, 'a') == -1);
        CHECK(mtl_String_rfind(&str, 'a') == -1);

        CHECK(mtl_String_mallocf(&str, ""));
        CHECK(mtl_String_find(&str, 'a') == -1);
        CHECK(mtl_String_rfind(&str, 'a') == -1);

        CHECK(mtl_String_mallocf(&str, "a"));
        CHECK(mtl_String_find(&str, 'a') == 0);
        CHECK(mtl_String_rfind(&str, 'a') == 0);

        CHECK(mtl_String_mallocf(&str, "abc"));
        CHECK(mtl_String_find(&str, 'a') == 0);
        CHECK(mtl_String_rfind(&str, 'a') == 0);

        CHECK(mtl_String_mallocf(&str, "123abc"));
        CHECK(mtl_String_find(&str, 'a') == 3);
        CHECK(mtl_String_rfind(&str, 'a') == 3);
        CHECK(mtl_String_rfind(&str, 'c') == 5);

        CHECK(mtl_String_mallocf(&str, "123abc"));
        CHECK(mtl_String_find(&str, 'A') == -1);
        CHECK(mtl_String_rfind(&str, 'A') == -1);

        mtl_String_free(&str);
}

CASE("mtl_String_ends_with")
{
        struct mtl_String one = mtl_String_init();
        struct mtl_String two = mtl_String_init();

        CHECK(mtl_String_mallocf(&one, "123"));
        CHECK(mtl_String_mallocf(&two, ""));
        CHECK(mtl_String_ends_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, ""));
        CHECK(mtl_String_mallocf(&two, ""));
        CHECK(mtl_String_ends_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, ""));
        CHECK(mtl_String_mallocf(&two, "123"));
        CHECK(!mtl_String_ends_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, "123"));
        CHECK(mtl_String_mallocf(&two, "123"));
        CHECK(mtl_String_ends_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, "my_file.json"));
        CHECK(mtl_String_mallocf(&two, ".json"));
        CHECK(mtl_String_ends_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, "my_file.json.stuff"));
        CHECK(mtl_String_mallocf(&two, ".json"));
        CHECK(!mtl_String_ends_with(&one, &two));

        mtl_String_free(&one);
        mtl_String_free(&two);
}

CASE("mtl_String_starts_with")
{
        struct mtl_String one = mtl_String_init();
        struct mtl_String two = mtl_String_init();

        CHECK(mtl_String_mallocf(&one, "123"));
        CHECK(mtl_String_mallocf(&two, ""));
        CHECK(mtl_String_starts_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, ""));
        CHECK(mtl_String_mallocf(&two, ""));
        CHECK(mtl_String_starts_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, ""));
        CHECK(mtl_String_mallocf(&two, "123"));
        CHECK(!mtl_String_starts_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, "123"));
        CHECK(mtl_String_mallocf(&two, "123"));
        CHECK(mtl_String_starts_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, "my_file.json"));
        CHECK(mtl_String_mallocf(&two, "my_file"));
        CHECK(mtl_String_starts_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, "my_file.json.stuff"));
        CHECK(mtl_String_mallocf(&two, "my_file.json"));
        CHECK(mtl_String_starts_with(&one, &two));

        CHECK(mtl_String_mallocf(&one, "functions/hans.csv"));
        CHECK(mtl_String_mallocf(&two, "functions/"));
        CHECK(mtl_String_starts_with(&one, &two));

        mtl_String_free(&one);
        mtl_String_free(&two);
}

CASE("mtl_String_has_prefix_suffix")
{
        struct mtl_String str = mtl_String_init();
        struct mtl_String pre = mtl_String_init();
        struct mtl_String suf = mtl_String_init();

        CHECK(mtl_String_mallocf(&str, ""));
        CHECK(mtl_String_mallocf(&pre, ""));
        CHECK(mtl_String_mallocf(&suf, ""));
        CHECK(mtl_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mtl_String_mallocf(&str, ""));
        mtl_String_free(&pre);
        mtl_String_free(&suf);
        CHECK(mtl_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mtl_String_mallocf(&str, "abc"));
        mtl_String_free(&pre);
        mtl_String_free(&suf);
        CHECK(mtl_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mtl_String_mallocf(&str, "abc"));
        CHECK(mtl_String_mallocf(&pre, "a"));
        CHECK(mtl_String_mallocf(&suf, "c"));
        CHECK(mtl_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mtl_String_mallocf(&str, "abc"));
        CHECK(mtl_String_mallocf(&pre, "a"));
        CHECK(mtl_String_mallocf(&suf, "d"));
        CHECK(!mtl_String_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mtl_String_mallocf(&str, "_abc"));
        CHECK(mtl_String_mallocf(&pre, "a"));
        CHECK(mtl_String_mallocf(&suf, "c"));
        CHECK(!mtl_String_has_prefix_suffix(&str, &pre, &suf));

        mtl_String_free(&str);
        mtl_String_free(&pre);
        mtl_String_free(&suf);
}

CASE("mtl_String_copy")
{
        struct mtl_String src = mtl_String_init();
        struct mtl_String dst = mtl_String_init();

        CHECK(mtl_String_mallocf(&src, "abb"));
        CHECK(mtl_String_copy(&dst, &src));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mtl_String_mallocf(&src, ""));
        CHECK(mtl_String_copy(&dst, &src));
        CHECK(0 == strcmp(dst.array, ""));

        mtl_String_free(&src);
        CHECK(!mtl_String_copy(&dst, &src));

        mtl_String_free(&src);
        mtl_String_free(&dst);
}

CASE("mtl_String_strip")
{
        struct mtl_String src = mtl_String_init();
        struct mtl_String dst = mtl_String_init();

        CHECK(mtl_String_mallocf(&src, ""));
        CHECK(mtl_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, ""));

        CHECK(mtl_String_mallocf(&src, "abb"));
        CHECK(mtl_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mtl_String_mallocf(&src, "  abb"));
        CHECK(mtl_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mtl_String_mallocf(&src, "abb  "));
        CHECK(mtl_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mtl_String_mallocf(&src, "   abb  "));
        CHECK(mtl_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "abb"));

        CHECK(mtl_String_mallocf(&src, "   a b  b  "));
        CHECK(mtl_String_strip(&src, &dst));
        CHECK(0 == strcmp(dst.array, "a b  b"));

        mtl_String_free(&src);
        CHECK(!mtl_String_strip(&src, &dst));

        mtl_String_free(&src);
        mtl_String_free(&dst);
}

CASE("mtl_String_strip")
{
        struct mtl_String src = mtl_String_init();

        mtl_String_countn(&src, 'a', 100);

        CHECK(mtl_String_mallocf(&src, "   a b  b  "));
        CHECK(2 == mtl_String_countn(&src, 'b', src.size));

        CHECK(mtl_String_mallocf(&src, "a b  b  "));
        CHECK(1 == mtl_String_countn(&src, 'b', 3));

        mtl_String_free(&src);
}

CASE("mtl_String_copyn")
{
        struct mtl_String src = mtl_String_init();
        struct mtl_String dst = mtl_String_init();

        CHECK(mtl_String_mallocf(&src, "123456789.json"));

        CHECK(mtl_String_copyn(&dst, &src, 0, src.size));
        CHECK(0 == strcmp(dst.array, "123456789.json"));

        CHECK(!mtl_String_copyn(&dst, &src, 0, src.size + 1));

        CHECK(mtl_String_copyn(&dst, &src, 0, 3));
        CHECK(0 == strcmp(dst.array, "123"));

        CHECK(mtl_String_copyn(&dst, &src, 8, 3));
        CHECK(0 == strcmp(dst.array, "9.j"));

        CHECK(mtl_String_copyn(&dst, &src, 8, 0));
        CHECK(0 == strcmp(dst.array, ""));

        mtl_String_free(&dst);
        mtl_String_free(&src);
}

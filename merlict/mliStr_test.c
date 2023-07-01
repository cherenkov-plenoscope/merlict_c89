/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliStr.h"


CASE("mliStr_rfind")
{
        struct mliStr str = mliStr_init();

        mliStr_free(&str);
        CHECK(mliStr_find(&str, 'a') == -1);
        CHECK(mliStr_rfind(&str, 'a') == -1);

        CHECK(mliStr_mallocf(&str, ""));
        CHECK(mliStr_find(&str, 'a') == -1);
        CHECK(mliStr_rfind(&str, 'a') == -1);

        CHECK(mliStr_mallocf(&str, "a"));
        CHECK(mliStr_find(&str, 'a') == 0);
        CHECK(mliStr_rfind(&str, 'a') == 0);

        CHECK(mliStr_mallocf(&str, "abc"));
        CHECK(mliStr_find(&str, 'a') == 0);
        CHECK(mliStr_rfind(&str, 'a') == 0);

        CHECK(mliStr_mallocf(&str, "123abc"));
        CHECK(mliStr_find(&str, 'a') == 3);
        CHECK(mliStr_rfind(&str, 'a') == 3);
        CHECK(mliStr_rfind(&str, 'c') == 5);

        CHECK(mliStr_mallocf(&str, "123abc"));
        CHECK(mliStr_find(&str, 'A') == -1);
        CHECK(mliStr_rfind(&str, 'A') == -1);

        mliStr_free(&str);
}


CASE("mliStr_ends_with")
{
        struct mliStr one = mliStr_init();
        struct mliStr two = mliStr_init();

        CHECK(mliStr_mallocf(&one, "123"));
        CHECK(mliStr_mallocf(&two, ""));
        CHECK(mliStr_ends_with(&one, &two));

        CHECK(mliStr_mallocf(&one, ""));
        CHECK(mliStr_mallocf(&two, ""));
        CHECK(mliStr_ends_with(&one, &two));

        CHECK(mliStr_mallocf(&one, ""));
        CHECK(mliStr_mallocf(&two, "123"));
        CHECK(!mliStr_ends_with(&one, &two));

        CHECK(mliStr_mallocf(&one, "123"));
        CHECK(mliStr_mallocf(&two, "123"));
        CHECK(mliStr_ends_with(&one, &two));

        CHECK(mliStr_mallocf(&one, "my_file.json"));
        CHECK(mliStr_mallocf(&two, ".json"));
        CHECK(mliStr_ends_with(&one, &two));

        CHECK(mliStr_mallocf(&one, "my_file.json.stuff"));
        CHECK(mliStr_mallocf(&two, ".json"));
        CHECK(!mliStr_ends_with(&one, &two));

        mliStr_free(&one);
        mliStr_free(&two);
}

CASE("mliStr_starts_with")
{
        struct mliStr one = mliStr_init();
        struct mliStr two = mliStr_init();

        CHECK(mliStr_mallocf(&one, "123"));
        CHECK(mliStr_mallocf(&two, ""));
        CHECK(mliStr_starts_with(&one, &two));

        CHECK(mliStr_mallocf(&one, ""));
        CHECK(mliStr_mallocf(&two, ""));
        CHECK(mliStr_starts_with(&one, &two));

        CHECK(mliStr_mallocf(&one, ""));
        CHECK(mliStr_mallocf(&two, "123"));
        CHECK(!mliStr_starts_with(&one, &two));

        CHECK(mliStr_mallocf(&one, "123"));
        CHECK(mliStr_mallocf(&two, "123"));
        CHECK(mliStr_starts_with(&one, &two));

        CHECK(mliStr_mallocf(&one, "my_file.json"));
        CHECK(mliStr_mallocf(&two, "my_file"));
        CHECK(mliStr_starts_with(&one, &two));

        CHECK(mliStr_mallocf(&one, "my_file.json.stuff"));
        CHECK(mliStr_mallocf(&two, "my_file.json"));
        CHECK(mliStr_starts_with(&one, &two));

        CHECK(mliStr_mallocf(&one, "functions/hans.csv"));
        CHECK(mliStr_mallocf(&two, "functions/"));
        CHECK(mliStr_starts_with(&one, &two));

        mliStr_free(&one);
        mliStr_free(&two);
}

CASE("mliStr_has_prefix_suffix")
{
        struct mliStr str = mliStr_init();
        struct mliStr pre = mliStr_init();
        struct mliStr suf = mliStr_init();

        CHECK(mliStr_mallocf(&str, ""));
        CHECK(mliStr_mallocf(&pre, ""));
        CHECK(mliStr_mallocf(&suf, ""));
        CHECK(mliStr_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mliStr_mallocf(&str, ""));
        mliStr_free(&pre);
        mliStr_free(&suf);
        CHECK(mliStr_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mliStr_mallocf(&str, "abc"));
        mliStr_free(&pre);
        mliStr_free(&suf);
        CHECK(mliStr_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mliStr_mallocf(&str, "abc"));
        CHECK(mliStr_mallocf(&pre, "a"));
        CHECK(mliStr_mallocf(&suf, "c"));
        CHECK(mliStr_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mliStr_mallocf(&str, "abc"));
        CHECK(mliStr_mallocf(&pre, "a"));
        CHECK(mliStr_mallocf(&suf, "d"));
        CHECK(!mliStr_has_prefix_suffix(&str, &pre, &suf));

        CHECK(mliStr_mallocf(&str, "_abc"));
        CHECK(mliStr_mallocf(&pre, "a"));
        CHECK(mliStr_mallocf(&suf, "c"));
        CHECK(!mliStr_has_prefix_suffix(&str, &pre, &suf));

        mliStr_free(&str);
        mliStr_free(&pre);
        mliStr_free(&suf);
}

CASE("mliStr_malloc_copy")
{
        struct mliStr src = mliStr_init();
        struct mliStr dst = mliStr_init();

        CHECK(mliStr_mallocf(&src, "abb"));
        CHECK(mliStr_malloc_copy(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, "abb"));

        CHECK(mliStr_mallocf(&src, ""));
        CHECK(mliStr_malloc_copy(&dst, &src));
        CHECK(0 == strcmp(dst.cstr, ""));

        mliStr_free(&src);
        CHECK(!mliStr_malloc_copy(&dst, &src));

        mliStr_free(&src);
        mliStr_free(&dst);
}

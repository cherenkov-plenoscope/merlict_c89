/* Copyright Sebastian Achim Mueller */

CASE("mli_path_strip_this_dir")
{
        struct mli_String src = mli_String_init();
        struct mli_String dst = mli_String_init();

        CHECK(mli_String_from_cstr_fromat(&src, "/a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mli_String_equal_cstr(&dst, src.array));

        CHECK(mli_String_from_cstr_fromat(&src, "./a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mli_String_equal_cstr(&dst, "a/b/c"));

        CHECK(mli_String_from_cstr_fromat(&src, "./functions/hans.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mli_String_equal_cstr(&dst, "functions/hans.csv"));

        CHECK(mli_String_from_cstr_fromat(&src, "././././f/h.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mli_String_equal_cstr(&dst, "f/h.csv"));

        CHECK(mli_String_from_cstr_fromat(&src, "a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mli_String_equal_cstr(&dst, "a/b"));

        CHECK(mli_String_from_cstr_fromat(&src, ".a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mli_String_equal_cstr(&dst, ".a/b"));

        CHECK(mli_String_from_cstr_fromat(&src, "a./b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mli_String_equal_cstr(&dst, "a./b"));

        mli_String_free(&src);
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == src.size);
        CHECK(0 == dst.size);
        CHECK(NULL == dst.array);

        mli_String_free(&src);
        mli_String_free(&dst);
}

CASE("mli_path_basename")
{
        struct mli_String path = mli_String_init();
        struct mli_String base = mli_String_init();

        mli_String_free(&path);
        CHECK(!mli_path_basename(&path, &base));

        CHECK(mli_String_from_cstr_fromat(&path, ""));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mli_String_equal_cstr(&base, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mli_String_equal_cstr(&base, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "//"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mli_String_equal_cstr(&base, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "a/b/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mli_String_equal_cstr(&base, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "a/b/c"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mli_String_equal_cstr(&base, "c"));

        mli_String_free(&path);
        mli_String_free(&base);
}

CASE("mli_path_splitext")
{
        struct mli_String path = mli_String_init();
        struct mli_String base = mli_String_init();
        struct mli_String ext = mli_String_init();

        mli_String_free(&path);
        CHECK(!mli_path_splitext(&path, &base, &ext));

        CHECK(mli_String_from_cstr_fromat(&path, ""));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, ""));
        CHECK(mli_String_equal_cstr(&ext, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "a.b"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "a"));
        CHECK(mli_String_equal_cstr(&ext, "b"));

        CHECK(mli_String_from_cstr_fromat(&path, ".a"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, ".a"));
        CHECK(mli_String_equal_cstr(&ext, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "."));
        CHECK(mli_String_equal_cstr(&ext, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "./"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./"));
        CHECK(mli_String_equal_cstr(&ext, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "./."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./"));
        CHECK(mli_String_equal_cstr(&ext, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "./.abc"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./.abc"));
        CHECK(mli_String_equal_cstr(&ext, ""));

        CHECK(mli_String_from_cstr_fromat(&path, "./.abc.json.tmp"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./.abc.json"));
        CHECK(mli_String_equal_cstr(&ext, "tmp"));

        CHECK(mli_String_from_cstr_fromat(&path, "./name.1.2.3"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./name.1.2"));
        CHECK(mli_String_equal_cstr(&ext, "3"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./name.1"));
        CHECK(mli_String_equal_cstr(&ext, "2"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./name"));
        CHECK(mli_String_equal_cstr(&ext, "1"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(mli_String_equal_cstr(&base, "./name"));
        CHECK(mli_String_equal_cstr(&ext, ""));

        mli_String_free(&path);
        mli_String_free(&base);
        mli_String_free(&ext);
}

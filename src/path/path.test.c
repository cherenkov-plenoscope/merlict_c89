/* Copyright Sebastian Achim Mueller */

CASE("mli_path_strip_this_dir")
{
        struct mtl_String src = mtl_String_init();
        struct mtl_String dst = mtl_String_init();

        CHECK(mtl_String_from_cstr_fromat(&src, "/a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mtl_String_equal_cstr(&dst, src.array));

        CHECK(mtl_String_from_cstr_fromat(&src, "./a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mtl_String_equal_cstr(&dst, "a/b/c"));

        CHECK(mtl_String_from_cstr_fromat(&src, "./functions/hans.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mtl_String_equal_cstr(&dst, "functions/hans.csv"));

        CHECK(mtl_String_from_cstr_fromat(&src, "././././f/h.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mtl_String_equal_cstr(&dst, "f/h.csv"));

        CHECK(mtl_String_from_cstr_fromat(&src, "a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mtl_String_equal_cstr(&dst, "a/b"));

        CHECK(mtl_String_from_cstr_fromat(&src, ".a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mtl_String_equal_cstr(&dst, ".a/b"));

        CHECK(mtl_String_from_cstr_fromat(&src, "a./b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(mtl_String_equal_cstr(&dst, "a./b"));

        mtl_String_free(&src);
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == src.size);
        CHECK(0 == dst.size);
        CHECK(NULL == dst.array);

        mtl_String_free(&src);
        mtl_String_free(&dst);
}

CASE("mli_path_basename")
{
        struct mtl_String path = mtl_String_init();
        struct mtl_String base = mtl_String_init();

        mtl_String_free(&path);
        CHECK(!mli_path_basename(&path, &base));

        CHECK(mtl_String_from_cstr_fromat(&path, ""));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mtl_String_equal_cstr(&base, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mtl_String_equal_cstr(&base, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "//"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mtl_String_equal_cstr(&base, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "a/b/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mtl_String_equal_cstr(&base, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "a/b/c"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(mtl_String_equal_cstr(&base, "c"));

        mtl_String_free(&path);
        mtl_String_free(&base);
}

CASE("mli_path_splitext")
{
        struct mtl_String path = mtl_String_init();
        struct mtl_String base = mtl_String_init();
        struct mtl_String ext = mtl_String_init();

        mtl_String_free(&path);
        CHECK(!mli_path_splitext(&path, &base, &ext));

        CHECK(mtl_String_from_cstr_fromat(&path, ""));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, ""));
        CHECK(mtl_String_equal_cstr(&ext, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "a.b"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "a"));
        CHECK(mtl_String_equal_cstr(&ext, "b"));

        CHECK(mtl_String_from_cstr_fromat(&path, ".a"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, ".a"));
        CHECK(mtl_String_equal_cstr(&ext, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "."));
        CHECK(mtl_String_equal_cstr(&ext, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "./"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./"));
        CHECK(mtl_String_equal_cstr(&ext, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "./."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./"));
        CHECK(mtl_String_equal_cstr(&ext, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "./.abc"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./.abc"));
        CHECK(mtl_String_equal_cstr(&ext, ""));

        CHECK(mtl_String_from_cstr_fromat(&path, "./.abc.json.tmp"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./.abc.json"));
        CHECK(mtl_String_equal_cstr(&ext, "tmp"));

        CHECK(mtl_String_from_cstr_fromat(&path, "./name.1.2.3"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./name.1.2"));
        CHECK(mtl_String_equal_cstr(&ext, "3"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./name.1"));
        CHECK(mtl_String_equal_cstr(&ext, "2"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./name"));
        CHECK(mtl_String_equal_cstr(&ext, "1"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(mtl_String_equal_cstr(&base, "./name"));
        CHECK(mtl_String_equal_cstr(&ext, ""));

        mtl_String_free(&path);
        mtl_String_free(&base);
        mtl_String_free(&ext);
}

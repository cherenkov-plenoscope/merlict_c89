/* Copyright Sebastian Achim Mueller */

#include "../../mli_testing/src/mli_testing.h"

CASE("mli_path_strip_this_dir")
{
        struct mtl_String src = mtl_String_init();
        struct mtl_String dst = mtl_String_init();

        CHECK(mtl_String_mallocf(&src, "/a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.array, src.array));

        CHECK(mtl_String_mallocf(&src, "./a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.array, "a/b/c"));

        CHECK(mtl_String_mallocf(&src, "./functions/hans.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.array, "functions/hans.csv"));

        CHECK(mtl_String_mallocf(&src, "././././f/h.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.array, "f/h.csv"));

        CHECK(mtl_String_mallocf(&src, "a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.array, "a/b"));

        CHECK(mtl_String_mallocf(&src, ".a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.array, ".a/b"));

        CHECK(mtl_String_mallocf(&src, "a./b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.array, "a./b"));

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

        CHECK(mtl_String_mallocf(&path, ""));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.array, ""));

        CHECK(mtl_String_mallocf(&path, "/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.array, ""));

        CHECK(mtl_String_mallocf(&path, "//"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.array, ""));

        CHECK(mtl_String_mallocf(&path, "a/b/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.array, ""));

        CHECK(mtl_String_mallocf(&path, "a/b/c"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.array, "c"));

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

        CHECK(mtl_String_mallocf(&path, ""));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, ""));
        CHECK(0 == strcmp(ext.array, ""));

        CHECK(mtl_String_mallocf(&path, "a.b"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, "a"));
        CHECK(0 == strcmp(ext.array, "b"));

        CHECK(mtl_String_mallocf(&path, ".a"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, ".a"));
        CHECK(0 == strcmp(ext.array, ""));

        CHECK(mtl_String_mallocf(&path, "."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, "."));
        CHECK(0 == strcmp(ext.array, ""));

        CHECK(mtl_String_mallocf(&path, "./"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, "./"));
        CHECK(0 == strcmp(ext.array, ""));

        CHECK(mtl_String_mallocf(&path, "./."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, "./"));
        CHECK(0 == strcmp(ext.array, ""));

        CHECK(mtl_String_mallocf(&path, "./.abc"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, "./.abc"));
        CHECK(0 == strcmp(ext.array, ""));

        CHECK(mtl_String_mallocf(&path, "./.abc.json.tmp"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, "./.abc.json"));
        CHECK(0 == strcmp(ext.array, "tmp"));

        CHECK(mtl_String_mallocf(&path, "./name.1.2.3"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.array, "./name.1.2"));
        CHECK(0 == strcmp(ext.array, "3"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(0 == strcmp(base.array, "./name.1"));
        CHECK(0 == strcmp(ext.array, "2"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(0 == strcmp(base.array, "./name"));
        CHECK(0 == strcmp(ext.array, "1"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(0 == strcmp(base.array, "./name"));
        CHECK(0 == strcmp(ext.array, ""));

        mtl_String_free(&path);
        mtl_String_free(&base);
        mtl_String_free(&ext);
}

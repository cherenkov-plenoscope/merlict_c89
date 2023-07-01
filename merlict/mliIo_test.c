/* Copyright 2018-2023 Sebastian Achim Mueller */

#include "mli_testing.h"
#include "mliIo.h"

CASE("BytesIo_init")
{
        struct mliIo byt = mliIo_init();

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}


CASE("BytesIo_malloc")
{
        struct mliIo byt = mliIo_init();
        CHECK(mliIo_malloc(&byt));

        CHECK(byt.cstr != NULL);
        CHECK(byt.capacity == 2u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);

        mliIo_free(&byt);
}


CASE("BytesIo_putc")
{
        struct mliIo byt = mliIo_init();
        uint64_t i;
        CHECK(mliIo_malloc(&byt));

        for (i = 0; i < 20; i++) {
                CHECK(mliIo_putc(&byt, 'A'));

                CHECK(byt.cstr != NULL);
                CHECK(byt.pos == i + 1);
                CHECK(byt.pos == byt.size);
                CHECK(byt.capacity > byt.size);

                /* always terminated with '\0' */
                CHECK(byt.cstr[byt.pos] == '\0');
                CHECK(strlen((char *)byt.cstr) == byt.size);
        }
        mliIo_free(&byt);

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}


CASE("BytesIo_putc_rewind_getc")
{
        struct mliIo byt = mliIo_init();
        CHECK(mliIo_malloc(&byt));

        CHECK(mliIo_putc(&byt, 'A'));
        CHECK(mliIo_putc(&byt, 'B'));
        CHECK(mliIo_putc(&byt, 'C'));

        mliIo_rewind(&byt);
        CHECK(byt.pos == 0u);

        CHECK(mliIo_getc(&byt) == 'A');
        CHECK(mliIo_getc(&byt) == 'B');
        CHECK(mliIo_getc(&byt) == 'C');
        CHECK(mliIo_getc(&byt) == EOF);

        mliIo_free(&byt);
}

CASE("BytesIo_write_rewind_read")
{
        struct mliIo byt = mliIo_init();
        int32_t evth[273];
        int32_t back[273];
        uint64_t i;
        int64_t rc;
        for (i = 0; i < 273; i++) {
                evth[i] = i;
        }

        CHECK(mliIo_malloc(&byt));

        rc = mliIo_write(&byt, evth, sizeof(float), 273);
        CHECK(rc == 273);

        CHECK(byt.cstr != NULL);
        CHECK(byt.size <= byt.capacity);
        CHECK(byt.size == 273 * 4);
        CHECK(byt.pos == byt.size);

        mliIo_rewind(&byt);
        CHECK(byt.pos == 0u);

        rc = mliIo_read(&byt, back, sizeof(float), 273);
        CHECK(rc == 273);

        for (i = 0; i < 273; i++) {
                CHECK(evth[i] == back[i]);
        }

        rc = mliIo_read(&byt, back, sizeof(float), 1);
        CHECK(rc == EOF);

        mliIo_free(&byt);
}

CASE("BytesIo_printf")
{
        struct mliIo byt = mliIo_init();

        mliIo_printf(&byt, "Hans %03d Lok %.3fh!", 8, 3.141);
        mliIo_rewind(&byt);

        CHECK(0 == strcmp((char *)byt.cstr, "Hans 008 Lok 3.141h!"));

        mliIo_free(&byt);
}

CASE("mli_readline")
{
        struct mliIo file = mliIo_init();
        struct mliStr line = mliStr_init();

        mliIo_printf(&file, "first-line\nsecond-line\n\nfourth-line\n");
        mliIo_rewind(&file);

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(0 == strcmp((char *)line.cstr, "first-line"));

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(0 == strcmp((char *)line.cstr, "second-line"));

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(line.length == 0);

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(0 == strcmp((char *)line.cstr, "fourth-line"));

        mliIo_free(&file);
        mliStr_free(&line);
}

CASE("mli_readline_empty")
{
        struct mliIo file = mliIo_init();
        struct mliStr line = mliStr_init();

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(line.length == 0);

        mliIo_free(&file);
        mliStr_free(&line);
}

CASE("mli_path_strip_this_dir")
{
        struct mliStr src = mliStr_init();
        struct mliStr dst = mliStr_init();

        CHECK(mliStr_mallocf(&src, "/a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.cstr, src.cstr));

        CHECK(mliStr_mallocf(&src, "./a/b/c"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.cstr, "a/b/c"));

        CHECK(mliStr_mallocf(&src, "./functions/hans.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.cstr, "functions/hans.csv"));

        CHECK(mliStr_mallocf(&src, "././././f/h.csv"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.cstr, "f/h.csv"));

        CHECK(mliStr_mallocf(&src, "a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.cstr, "a/b"));

        CHECK(mliStr_mallocf(&src, ".a/b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.cstr, ".a/b"));

        CHECK(mliStr_mallocf(&src, "a./b"));
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == strcmp(dst.cstr, "a./b"));

        mliStr_free(&src);
        mli_path_strip_this_dir(&src, &dst);
        CHECK(0 == src.length);
        CHECK(0 == dst.length);
        CHECK(NULL == dst.cstr);

        mliStr_free(&src);
        mliStr_free(&dst);
}

CASE("mli_path_basename")
{
        struct mliStr path = mliStr_init();
        struct mliStr base = mliStr_init();

        mliStr_free(&path);
        CHECK(!mli_path_basename(&path, &base));

        CHECK(mliStr_mallocf(&path, ""));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.cstr, ""));

        CHECK(mliStr_mallocf(&path, "/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.cstr, ""));

        CHECK(mliStr_mallocf(&path, "//"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.cstr, ""));

        CHECK(mliStr_mallocf(&path, "a/b/"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.cstr, ""));

        CHECK(mliStr_mallocf(&path, "a/b/c"));
        CHECK(mli_path_basename(&path, &base));
        CHECK(0 == strcmp(base.cstr, "c"));

        mliStr_free(&path);
        mliStr_free(&base);
}

CASE("mli_path_splitext")
{
        struct mliStr path = mliStr_init();
        struct mliStr base = mliStr_init();
        struct mliStr ext = mliStr_init();

        mliStr_free(&path);
        CHECK(!mli_path_splitext(&path, &base, &ext));

        CHECK(mliStr_mallocf(&path, ""));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, ""));
        CHECK(0 == strcmp(ext.cstr, ""));

        CHECK(mliStr_mallocf(&path, "a.b"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "a"));
        CHECK(0 == strcmp(ext.cstr, "b"));

        CHECK(mliStr_mallocf(&path, ".a"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, ".a"));
        CHECK(0 == strcmp(ext.cstr, ""));

        CHECK(mliStr_mallocf(&path, "."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "."));
        CHECK(0 == strcmp(ext.cstr, ""));

        CHECK(mliStr_mallocf(&path, "./"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./"));
        CHECK(0 == strcmp(ext.cstr, ""));

        CHECK(mliStr_mallocf(&path, "./."));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./"));
        CHECK(0 == strcmp(ext.cstr, ""));

        CHECK(mliStr_mallocf(&path, "./.abc"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./.abc"));
        CHECK(0 == strcmp(ext.cstr, ""));

        CHECK(mliStr_mallocf(&path, "./.abc.json.tmp"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./.abc.json"));
        CHECK(0 == strcmp(ext.cstr, "tmp"));

        CHECK(mliStr_mallocf(&path, "./name.1.2.3"));
        CHECK(mli_path_splitext(&path, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./name.1.2"));
        CHECK(0 == strcmp(ext.cstr, "3"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./name.1"));
        CHECK(0 == strcmp(ext.cstr, "2"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./name"));
        CHECK(0 == strcmp(ext.cstr, "1"));

        CHECK(mli_path_splitext(&base, &base, &ext));
        CHECK(0 == strcmp(base.cstr, "./name"));
        CHECK(0 == strcmp(ext.cstr, ""));

        mliStr_free(&path);
        mliStr_free(&base);
        mliStr_free(&ext);
}

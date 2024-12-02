/* Copyright 2018-2023 Sebastian Achim Mueller */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliIo.h"

CASE("mliIo_init")
{
        struct mliIo byt = mliIo_init();

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}

CASE("mliIo_reset")
{
        struct mliIo byt = mliIo_init();
        CHECK(mliIo_reset(&byt));

        CHECK(byt.cstr != NULL);
        CHECK(byt.capacity == 2u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);

        mliIo_free(&byt);
}

CASE("mliIo_copy")
{
        struct mliIo src = mliIo_init();
        struct mliIo dst = mliIo_init();

        CHECK(mliIo_write_cstr_format(&src, "abb"));
        CHECK(mliIo_copy(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "abb"));

        CHECK(mliIo_reset(&src));
        CHECK(mliIo_write_cstr_format(&src, ""));
        CHECK(mliIo_copy(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, ""));

        mliIo_free(&src);
        CHECK(!mliIo_copy(&dst, &src));

        mliIo_free(&dst);
        mliIo_free(&src);
}

CASE("mliIo_shrink_to_fit")
{
        struct mliIo str = mliIo_init();

        CHECK(mliIo_write_cstr_format(&str, "0123456789"));

        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 10);
        CHECK(str.pos == 10);

        CHECK(mliIo_reset(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 0);
        CHECK(str.pos == 0);

        CHECK(mliIo_write_cstr_format(&str, "abc"));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        CHECK(mliIo_shrink_to_fit(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 3);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        mliIo_free(&str);
}

CASE("mliIo_copy_start_num")
{
        struct mliIo src = mliIo_init();
        struct mliIo dst = mliIo_init();

        CHECK(mliIo_write_cstr_format(&src, "123456789.json"));

        CHECK(mliIo_copy_start_num(&dst, &src, 0, src.size));
        CHECK(0 == strcmp((char *)dst.cstr, "123456789.json"));

        CHECK(!mliIo_copy_start_num(&dst, &src, 0, src.size + 1));

        CHECK(mliIo_copy_start_num(&dst, &src, 0, 3));
        CHECK(0 == strcmp((char *)dst.cstr, "123"));

        CHECK(mliIo_copy_start_num(&dst, &src, 8, 3));
        CHECK(0 == strcmp((char *)dst.cstr, "9.j"));

        CHECK(mliIo_copy_start_num(&dst, &src, 8, 0));
        CHECK(0 == strcmp((char *)dst.cstr, ""));

        mliIo_free(&dst);
        mliIo_free(&src);
}

CASE("BytesIo_putc")
{
        struct mliIo byt = mliIo_init();
        uint64_t i;
        CHECK(mliIo_reset(&byt));

        for (i = 0; i < 20; i++) {
                CHECK(mliIo_write_unsigned_char(&byt, 'A'));

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
        CHECK(mliIo_reset(&byt));

        CHECK(mliIo_write_unsigned_char(&byt, 'A'));
        CHECK(mliIo_write_unsigned_char(&byt, 'B'));
        CHECK(mliIo_write_unsigned_char(&byt, 'C'));

        mliIo_rewind(&byt);
        CHECK(byt.pos == 0u);

        CHECK(mliIo_read_char(&byt) == 'A');
        CHECK(mliIo_read_char(&byt) == 'B');
        CHECK(mliIo_read_char(&byt) == 'C');
        CHECK(mliIo_read_char(&byt) == EOF);

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

        CHECK(mliIo_reset(&byt));

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

        mliIo_write_cstr_format(&byt, "Hans %03d Lok %.3fh!", 8, 3.141);
        mliIo_rewind(&byt);

        CHECK(0 == strcmp((char *)byt.cstr, "Hans 008 Lok 3.141h!"));

        mliIo_free(&byt);
}

CASE("mli_readline")
{
        struct mliIo file = mliIo_init();
        struct mtl_String line = mtl_String_init();

        mliIo_write_cstr_format(
                &file, "first-line\nsecond-line\n\nfourth-line\n");
        mliIo_rewind(&file);

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(0 == strcmp((char *)line.array, "first-line"));

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(0 == strcmp((char *)line.array, "second-line"));

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(line.size == 0);

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(0 == strcmp((char *)line.array, "fourth-line"));

        mliIo_free(&file);
        mtl_String_free(&line);
}

CASE("mli_readline_empty")
{
        struct mliIo file = mliIo_init();
        struct mtl_String line = mtl_String_init();

        CHECK(mli_readline(&file, &line, '\n'));
        CHECK(line.size == 0);

        mliIo_free(&file);
        mtl_String_free(&line);
}

CASE("mli_line_viewer_write")
{
        struct mliIo f = mliIo_init();
        struct mtl_String text = mtl_String_init();

        CHECK(mliIo_write_cstr_format(&f, "TEXT\n"));
        CHECK(mliIo_write_cstr_format(&f, "====\n"));
        CHECK(mliIo_write_cstr_format(&f, "auto hirsch flasche bat\n"));
        CHECK(mliIo_write_cstr_format(&f, "tisch rad wein\n"));
        CHECK(mliIo_write_cstr_format(&f, "\n"));
        CHECK(mliIo_write_cstr_format(&f, "Ausserdem: Stuhl\n"));
        CHECK(mliIo_write_cstr_format(&f, "1.)\n"));
        CHECK(mliIo_write_cstr_format(&f, " - soziooekonomisch\n"));
        CHECK(mliIo_write_cstr_format(&f, "ENDE\n"));
        mliIo_rewind(&f);

        CHECK(mtl_String_from_cstr(&text, (char *)f.cstr));
        mliIo_free(&f);

        CHECK(mli_line_viewer_write(&f, &text, 1, 3));
        CHECK(mliIo_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_line_viewer_write(&f, &text, 4, 5));
        CHECK(mliIo_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_line_viewer_write(&f, &text, 7, 2));

        mliIo_rewind(&f);
        CHECK(mliIo_read_to_path(
                &f, "libs/mli/tests/resources/lines_info.tmp"));

        mliIo_free(&f);
        mtl_String_free(&text);
}

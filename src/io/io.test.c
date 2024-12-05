/* Copyright Sebastian Achim Mueller */

CASE("mli_IO_init")
{
        struct mli_IO byt = mli_IO_init();

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}

CASE("mli_IO_reset")
{
        struct mli_IO byt = mli_IO_init();
        CHECK(mli_IO_reset(&byt));

        CHECK(byt.cstr != NULL);
        CHECK(byt.capacity == 2u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);

        mli_IO_free(&byt);
}

CASE("mli_IO_copy")
{
        struct mli_IO src = mli_IO_init();
        struct mli_IO dst = mli_IO_init();

        CHECK(mli_IO_write_cstr_format(&src, "abb"));
        CHECK(mli_IO_copy(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, "abb"));

        CHECK(mli_IO_reset(&src));
        CHECK(mli_IO_write_cstr_format(&src, ""));
        CHECK(mli_IO_copy(&dst, &src));
        CHECK(0 == strcmp((char *)dst.cstr, ""));

        mli_IO_free(&src);
        CHECK(!mli_IO_copy(&dst, &src));

        mli_IO_free(&dst);
        mli_IO_free(&src);
}

CASE("mli_IO_shrink_to_fit")
{
        struct mli_IO str = mli_IO_init();

        CHECK(mli_IO_write_cstr_format(&str, "0123456789"));

        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 10);
        CHECK(str.pos == 10);

        CHECK(mli_IO_reset(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 0);
        CHECK(str.pos == 0);

        CHECK(mli_IO_write_cstr_format(&str, "abc"));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        CHECK(mli_IO_shrink_to_fit(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 3);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        mli_IO_free(&str);
}

CASE("mli_IO_copy_start_num")
{
        struct mli_IO src = mli_IO_init();
        struct mli_IO dst = mli_IO_init();

        CHECK(mli_IO_write_cstr_format(&src, "123456789.json"));

        CHECK(mli_IO_copy_start_num(&dst, &src, 0, src.size));
        CHECK(0 == strcmp((char *)dst.cstr, "123456789.json"));

        CHECK(!mli_IO_copy_start_num(&dst, &src, 0, src.size + 1));

        CHECK(mli_IO_copy_start_num(&dst, &src, 0, 3));
        CHECK(0 == strcmp((char *)dst.cstr, "123"));

        CHECK(mli_IO_copy_start_num(&dst, &src, 8, 3));
        CHECK(0 == strcmp((char *)dst.cstr, "9.j"));

        CHECK(mli_IO_copy_start_num(&dst, &src, 8, 0));
        CHECK(0 == strcmp((char *)dst.cstr, ""));

        mli_IO_free(&dst);
        mli_IO_free(&src);
}

CASE("BytesIo_putc")
{
        struct mli_IO byt = mli_IO_init();
        uint64_t i;
        CHECK(mli_IO_reset(&byt));

        for (i = 0; i < 20; i++) {
                CHECK(mli_IO_write_unsigned_char(&byt, 'A'));

                CHECK(byt.cstr != NULL);
                CHECK(byt.pos == i + 1);
                CHECK(byt.pos == byt.size);
                CHECK(byt.capacity > byt.size);

                /* always terminated with '\0' */
                CHECK(byt.cstr[byt.pos] == '\0');
                CHECK(strlen((char *)byt.cstr) == byt.size);
        }
        mli_IO_free(&byt);

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}

CASE("BytesIo_putc_rewind_getc")
{
        struct mli_IO byt = mli_IO_init();
        CHECK(mli_IO_reset(&byt));

        CHECK(mli_IO_write_unsigned_char(&byt, 'A'));
        CHECK(mli_IO_write_unsigned_char(&byt, 'B'));
        CHECK(mli_IO_write_unsigned_char(&byt, 'C'));

        mli_IO_rewind(&byt);
        CHECK(byt.pos == 0u);

        CHECK(mli_IO_read_char(&byt) == 'A');
        CHECK(mli_IO_read_char(&byt) == 'B');
        CHECK(mli_IO_read_char(&byt) == 'C');
        CHECK(mli_IO_read_char(&byt) == EOF);

        mli_IO_free(&byt);
}

CASE("BytesIo_write_rewind_read")
{
        struct mli_IO byt = mli_IO_init();
        int32_t evth[273];
        int32_t back[273];
        uint64_t i;
        int64_t rc;
        for (i = 0; i < 273; i++) {
                evth[i] = i;
        }

        CHECK(mli_IO_reset(&byt));

        rc = mli_IO_write(&byt, evth, sizeof(float), 273);
        CHECK(rc == 273);

        CHECK(byt.cstr != NULL);
        CHECK(byt.size <= byt.capacity);
        CHECK(byt.size == 273 * 4);
        CHECK(byt.pos == byt.size);

        mli_IO_rewind(&byt);
        CHECK(byt.pos == 0u);

        rc = mli_IO_read(&byt, back, sizeof(float), 273);
        CHECK(rc == 273);

        for (i = 0; i < 273; i++) {
                CHECK(evth[i] == back[i]);
        }

        rc = mli_IO_read(&byt, back, sizeof(float), 1);
        CHECK(rc == EOF);

        mli_IO_free(&byt);
}

CASE("BytesIo_printf")
{
        struct mli_IO byt = mli_IO_init();

        mli_IO_write_cstr_format(&byt, "Hans %03d Lok %.3fh!", 8, 3.141);
        mli_IO_rewind(&byt);

        CHECK(0 == strcmp((char *)byt.cstr, "Hans 008 Lok 3.141h!"));

        mli_IO_free(&byt);
}

CASE("mli_IO_readline")
{
        struct mli_IO file = mli_IO_init();
        struct mli_String line = mli_String_init();

        mli_IO_write_cstr_format(
                &file, "first-line\nsecond-line\n\nfourth-line\n");
        mli_IO_rewind(&file);

        CHECK(mli_IO_readline(&file, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "first-line"));

        CHECK(mli_IO_readline(&file, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "second-line"));

        CHECK(mli_IO_readline(&file, &line, '\n'));
        CHECK(line.size == 0);

        CHECK(mli_IO_readline(&file, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "fourth-line"));

        mli_IO_free(&file);
        mli_String_free(&line);
}

CASE("mli_IO_readline_empty")
{
        struct mli_IO file = mli_IO_init();
        struct mli_String line = mli_String_init();

        CHECK(mli_IO_readline(&file, &line, '\n'));
        CHECK(line.size == 0);

        mli_IO_free(&file);
        mli_String_free(&line);
}

CASE("mli_line_viewer_write")
{
        struct mli_IO f = mli_IO_init();
        struct mli_String text = mli_String_init();

        CHECK(mli_IO_write_cstr_format(&f, "TEXT\n"));
        CHECK(mli_IO_write_cstr_format(&f, "====\n"));
        CHECK(mli_IO_write_cstr_format(&f, "auto hirsch flasche bat\n"));
        CHECK(mli_IO_write_cstr_format(&f, "tisch rad wein\n"));
        CHECK(mli_IO_write_cstr_format(&f, "\n"));
        CHECK(mli_IO_write_cstr_format(&f, "Ausserdem: Stuhl\n"));
        CHECK(mli_IO_write_cstr_format(&f, "1.)\n"));
        CHECK(mli_IO_write_cstr_format(&f, " - soziooekonomisch\n"));
        CHECK(mli_IO_write_cstr_format(&f, "ENDE\n"));
        mli_IO_rewind(&f);

        CHECK(mli_String_from_cstr(&text, (char *)f.cstr));
        mli_IO_free(&f);

        CHECK(mli_line_viewer_write(&f, &text, 1, 3));
        CHECK(mli_IO_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_line_viewer_write(&f, &text, 4, 5));
        CHECK(mli_IO_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_line_viewer_write(&f, &text, 7, 2));

        mli_IO_rewind(&f);
        CHECK(mli_IO_read_to_path(
                &f, "data/mli/tests/resources/lines_info.tmp"));

        mli_IO_free(&f);
        mli_String_free(&text);
}

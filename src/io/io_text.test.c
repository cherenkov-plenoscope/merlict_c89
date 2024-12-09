
CASE("BytesIo_putc")
{
        struct mli_IO byt = mli_IO_init();
        uint64_t i;
        CHECK(mli_IO_open(&byt));

        for (i = 0; i < 20; i++) {
                CHECK(mli_IO_text_putc(&byt, 'A'));

                CHECK(byt.cstr != NULL);
                CHECK(byt.pos == i + 1);
                CHECK(byt.pos == byt.size);
                CHECK(byt.capacity > byt.size);

                /* always terminated with '\0' */
                CHECK(byt.cstr[byt.pos] == '\0');
                CHECK(strlen((char *)byt.cstr) == byt.size);
        }
        mli_IO_close(&byt);

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}

CASE("BytesIo_putc_rewind_getc")
{
        struct mli_IO byt = mli_IO_init();
        CHECK(mli_IO_open(&byt));

        CHECK(mli_IO_text_putc(&byt, 'A'));
        CHECK(mli_IO_text_putc(&byt, 'B'));
        CHECK(mli_IO_text_putc(&byt, 'C'));

        mli_IO_rewind(&byt);
        CHECK(byt.pos == 0u);

        CHECK(mli_IO_text_getc(&byt) == 'A');
        CHECK(mli_IO_text_getc(&byt) == 'B');
        CHECK(mli_IO_text_getc(&byt) == 'C');
        CHECK(mli_IO_text_getc(&byt) == EOF);

        mli_IO_close(&byt);
}

CASE("mli_IO_text_read_line")
{
        struct mli_IO file = mli_IO_init();
        struct mli_String line = mli_String_init();

        mli_IO_write_cstr_format(
                &file, "first-line\nsecond-line\n\nfourth-line\n");
        mli_IO_rewind(&file);

        CHECK(mli_IO_text_read_line(&file, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "first-line"));

        CHECK(mli_IO_text_read_line(&file, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "second-line"));

        CHECK(mli_IO_text_read_line(&file, &line, '\n'));
        CHECK(line.size == 0);

        CHECK(mli_IO_text_read_line(&file, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "fourth-line"));

        mli_IO_close(&file);
        mli_String_free(&line);
}

CASE("mli_IO_text_read_line_empty")
{
        struct mli_IO file = mli_IO_init();
        struct mli_String line = mli_String_init();

        CHECK(mli_IO_text_read_line(&file, &line, '\n'));
        CHECK(line.size == 0);

        mli_IO_close(&file);
        mli_String_free(&line);
}

CASE("mli_IO_text_write_multi_line_debug_view")
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
        mli_IO_close(&f);

        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 1, 3));
        CHECK(mli_IO_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 4, 5));
        CHECK(mli_IO_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 7, 2));

        mli_IO_rewind(&f);
        CHECK(mli_IO_read_to_path(
                &f, "data/mli/tests/resources/lines_info.tmp"));

        mli_IO_close(&f);
        mli_String_free(&text);
}

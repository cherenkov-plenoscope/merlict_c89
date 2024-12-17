
CASE("BytesIo_putc")
{
        struct mli_IO io = mli_IO_init();
        int64_t i;
        CHECK(mli_IO_open_memory(&io));

        for (i = 0; i < 20; i++) {
                CHECK(mli_IO_text_putc(&io, 'A' + i));
        }
        mli_IO_rewind(&io);
        for (i = 0; i < 20; i++) {
                int c = mli_IO_text_getc(&io);
                CHECK(c != EOF);
                CHECK(c == 'A' + i);
        }

        mli_IO_close(&io);
}

CASE("BytesIo_putc_rewind_getc")
{
        struct mli_IO io = mli_IO_init();
        CHECK(mli_IO_open_memory(&io));

        CHECK(mli_IO_text_putc(&io, 'A'));
        CHECK(mli_IO_text_putc(&io, 'B'));
        CHECK(mli_IO_text_putc(&io, 'C'));

        mli_IO_rewind(&io);

        CHECK(mli_IO_text_getc(&io) == 'A');
        CHECK(mli_IO_text_getc(&io) == 'B');
        CHECK(mli_IO_text_getc(&io) == 'C');
        CHECK(mli_IO_text_getc(&io) == EOF);

        mli_IO_close(&io);
}

CASE("mli_IO_text_read_line")
{
        struct mli_IO file = mli_IO_init();
        struct mli_String line = mli_String_init();

        CHECK(mli_IO_open_memory(&file));
        mli_IO_text_write_cstr_format(
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

        CHECK(mli_IO_open_memory(&file));
        CHECK(mli_IO_text_read_line(&file, &line, '\n'));
        CHECK(line.size == 0);

        mli_IO_close(&file);
        mli_String_free(&line);
}

CASE("mli_IO_text_write_multi_line_debug_view")
{
        struct mli_IO f = mli_IO_init();
        struct mli_String text = mli_String_init();
        struct mli_String line = mli_String_init();

        CHECK(mli_IO_open_memory(&f));
        CHECK(mli_IO_text_write_cstr(&f, "TEXT\n"));
        CHECK(mli_IO_text_write_cstr(&f, "====\n"));
        CHECK(mli_IO_text_write_cstr(&f, "auto hirsch flasche bat\n"));
        CHECK(mli_IO_text_write_cstr(&f, "tisch rad wein\n"));
        CHECK(mli_IO_text_write_cstr(&f, "\n"));
        CHECK(mli_IO_text_write_cstr(&f, "Ausserdem: Stuhl\n"));
        CHECK(mli_IO_text_write_cstr(&f, "1.)\n"));
        CHECK(mli_IO_text_write_cstr(&f, " - soziooekonomisch\n"));
        CHECK(mli_IO_text_write_cstr(&f, "ENDE\n"));
        mli_IO_rewind(&f);
        CHECK(mli_IO_text_read_string(&f, &text));
        mli_IO_close(&f);

        CHECK(mli_IO_open_memory(&f));
        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 1, 3));
        mli_IO_rewind(&f);
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "  line     text"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "        |"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     1->|  TEXT"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     2  |  ===="));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(
                &line, "     3  |  auto hirsch flasche bat"));
        mli_IO_close(&f);

        CHECK(mli_IO_open_memory(&f));
        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 4, 5));
        mli_IO_rewind(&f);
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "  line     text"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "        |"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     1  |  TEXT"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     2  |  ===="));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(
                &line, "     3  |  auto hirsch flasche bat"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     4->|  tisch rad wein"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     5  |  "));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     6  |  Ausserdem: Stuhl"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     7  |  1.)"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     8  |   - soziooekonomisch"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     9  |  ENDE"));
        mli_IO_rewind(&f);
        mli_IO_close(&f);

        CHECK(mli_IO_open_memory(&f));
        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 7, 2));
        mli_IO_rewind(&f);
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "  line     text"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "        |"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     5  |  "));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     6  |  Ausserdem: Stuhl"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     7->|  1.)"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     8  |   - soziooekonomisch"));
        CHECK(mli_IO_text_read_line(&f, &line, '\n'));
        CHECK(mli_String_equal_cstr(&line, "     9  |  ENDE"));
        mli_IO_close(&f);

        mli_String_free(&line);
        mli_String_free(&text);
}

CASE("mli_IO_text_write_cstr_format")
{
        struct mli_IO io = mli_IO_init();
        struct mli_String text = mli_String_init();

        CHECK(mli_IO_open_memory(&io));

        CHECK(mli_IO_text_write_cstr_format(
                &io, "Hans %03d Lok %.3fh!", 8, 3.141));
        mli_IO_rewind(&io);
        CHECK(mli_IO_text_read_string(&io, &text));
        mli_IO_close(&io);

        CHECK(mli_String_equal_cstr(&text, "Hans 008 Lok 3.141h!"));
        mli_String_free(&text);
}

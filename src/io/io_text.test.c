
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

        mli_IO_free(&file);
        mli_String_free(&line);
}

CASE("mli_IO_text_read_line_empty")
{
        struct mli_IO file = mli_IO_init();
        struct mli_String line = mli_String_init();

        CHECK(mli_IO_text_read_line(&file, &line, '\n'));
        CHECK(line.size == 0);

        mli_IO_free(&file);
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
        mli_IO_free(&f);

        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 1, 3));
        CHECK(mli_IO_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 4, 5));
        CHECK(mli_IO_write_cstr_format(&f, "\n---\n\n"));

        CHECK(mli_IO_text_write_multi_line_debug_view(&f, &text, 7, 2));

        mli_IO_rewind(&f);
        CHECK(mli_IO_read_to_path(
                &f, "data/mli/tests/resources/lines_info.tmp"));

        mli_IO_free(&f);
        mli_String_free(&text);
}

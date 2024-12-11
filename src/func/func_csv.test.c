/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Func_from_csv")
{
        struct mli_Func func = mli_Func_init();
        struct mli_String sx = mli_String_init();
        struct mli_String sy = mli_String_init();
        struct mli_IO io = mli_IO_init();

        CHECK(mli_IO_open(&io));
        CHECK(mli_IO_text_write_cstr_format(&io, "wavelength/m,something/1\n"));
        CHECK(mli_IO_text_write_cstr_format(&io, "300e-9,0.1\n"));
        CHECK(mli_IO_text_write_cstr_format(&io, "400e-9,0.2\n"));
        CHECK(mli_IO_text_write_cstr_format(&io, "500e-9,0.5\n"));
        CHECK(mli_IO_text_write_cstr_format(&io, "600e-9,0.3\n"));
        CHECK(mli_IO_text_write_cstr_format(&io, "700e-9,0.1\n"));
        CHECK(mli_IO_text_write_cstr_format(&io, "800e-9,0.0\n"));
        mli_IO_rewind(&io);

        CHECK(mli_Func_from_csv(&func, &sx, &sy, &io));

        CHECK(mli_String_equal_cstr(&sx, "wavelength/m"));
        CHECK(mli_String_equal_cstr(&sy, "something/1"));

        CHECK(func.num_points == 6);

        CHECK_MARGIN(func.x[0], 300e-9, 1e-9);
        CHECK_MARGIN(func.x[1], 400e-9, 1e-9);
        CHECK_MARGIN(func.x[2], 500e-9, 1e-9);
        CHECK_MARGIN(func.x[3], 600e-9, 1e-9);
        CHECK_MARGIN(func.x[4], 700e-9, 1e-9);
        CHECK_MARGIN(func.x[5], 800e-9, 1e-9);

        CHECK_MARGIN(func.y[0], 0.1, 1e-6);
        CHECK_MARGIN(func.y[1], 0.2, 1e-6);
        CHECK_MARGIN(func.y[2], 0.5, 1e-6);
        CHECK_MARGIN(func.y[3], 0.3, 1e-6);
        CHECK_MARGIN(func.y[4], 0.1, 1e-6);
        CHECK_MARGIN(func.y[5], 0.0, 1e-6);

        mli_String_free(&sx);
        mli_String_free(&sy);
        mli_IO_close(&io);
        mli_Func_free(&func);
}

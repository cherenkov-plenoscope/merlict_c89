/* Copyright Sebastian Achim Mueller */

CASE("mli_IO_init")
{
        struct mli_IO byt = mli_IO_init();

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}

CASE("mli_IO_open")
{
        struct mli_IO byt = mli_IO_init();
        CHECK(mli_IO_open(&byt));

        CHECK(byt.cstr != NULL);
        CHECK(byt.capacity == 2u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);

        mli_IO_close(&byt);
}

CASE("mli_IO__shrink_to_fit")
{
        struct mli_IO str = mli_IO_init();

        CHECK(mli_IO_text_write_cstr_format(&str, "0123456789"));

        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 10);
        CHECK(str.pos == 10);

        CHECK(mli_IO_open(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 0);
        CHECK(str.pos == 0);

        CHECK(mli_IO_text_write_cstr_format(&str, "abc"));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        CHECK(mli_IO__shrink_to_fit(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 3);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        mli_IO_close(&str);
}

CASE("BytesIo_write_rewind_read")
{
        struct mli_IO byt = mli_IO_init();
        int32_t evth[273];
        int32_t back[273];
        uint64_t i;
        size_t rc;
        for (i = 0; i < 273; i++) {
                evth[i] = i;
        }

        CHECK(mli_IO_open(&byt));

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
        CHECK(rc == 0);

        mli_IO_close(&byt);
}

CASE("BytesIo_printf")
{
        struct mli_IO byt = mli_IO_init();

        mli_IO_text_write_cstr_format(&byt, "Hans %03d Lok %.3fh!", 8, 3.141);
        mli_IO_rewind(&byt);

        CHECK(0 == strcmp((char *)byt.cstr, "Hans 008 Lok 3.141h!"));

        mli_IO_close(&byt);
}

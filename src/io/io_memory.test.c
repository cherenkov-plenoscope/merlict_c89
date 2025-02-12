/* Copyright Sebastian Achim Mueller */

CASE("mli_IoMemory_init")
{
        struct mli_IoMemory byt = mli_IoMemory_init();

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}

CASE("mli_IoMemory_open")
{
        struct mli_IoMemory byt = mli_IoMemory_init();
        CHECK(mli_IoMemory_open(&byt));

        CHECK(byt.cstr != NULL);
        CHECK(byt.capacity == 2u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);

        mli_IoMemory_close(&byt);
}

CASE("mli_IoMemory_rewind")
{
        struct mli_IoMemory byt = mli_IoMemory_init();
        unsigned char c;
        CHECK(mli_IoMemory_open(&byt));

        CHECK(byt.cstr != NULL);
        CHECK(byt.capacity == 2u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);

        c = 'a';
        CHECK(mli_IoMemory__write_unsigned_char(&byt, &c));
        c = 'b';
        CHECK(mli_IoMemory__write_unsigned_char(&byt, &c));
        c = 'c';
        CHECK(mli_IoMemory__write_unsigned_char(&byt, &c));
        c = '\0';
        CHECK(mli_IoMemory__write_unsigned_char(&byt, &c));
        CHECK(byt.size == 4u);
        CHECK(byt.pos == 4u);

        mli_IoMemory_rewind(&byt);

        CHECK(byt.size == 4u);
        CHECK(byt.pos == 0u);
        CHECK(mli_IoMemory__read_unsigned_char(&byt, &c));
        CHECK(c == 'a');
        CHECK(mli_IoMemory__read_unsigned_char(&byt, &c));
        CHECK(c == 'b');
        CHECK(mli_IoMemory__read_unsigned_char(&byt, &c));
        CHECK(c == 'c');
        CHECK(mli_IoMemory__read_unsigned_char(&byt, &c));
        CHECK(c == '\0');
        CHECK(byt.size == 4u);
        CHECK(byt.pos == 4u);

        mli_IoMemory_rewind(&byt);

        CHECK(byt.size == 4u);
        CHECK(byt.pos == 0u);
        c = 'x';
        CHECK(mli_IoMemory__write_unsigned_char(&byt, &c));
        c = 'y';
        CHECK(mli_IoMemory__write_unsigned_char(&byt, &c));
        c = '\0';
        CHECK(mli_IoMemory__write_unsigned_char(&byt, &c));
        CHECK(byt.size == 4u);
        CHECK(byt.pos == 3u);

        mli_IoMemory_close(&byt);
}

CASE("mli_IoMemory__shrink_to_fit")
{
        struct mli_IoMemory str = mli_IoMemory_init();

        CHECK(mli_IoMemory__write_cstr(&str, "0123456789"));

        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 10);
        CHECK(str.pos == 10);

        CHECK(mli_IoMemory_open(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 0);
        CHECK(str.pos == 0);

        CHECK(mli_IoMemory__write_cstr(&str, "abc"));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 16);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        CHECK(mli_IoMemory__shrink_to_fit(&str));
        CHECK(str.cstr != NULL);
        CHECK(str.capacity == 3);
        CHECK(str.size == 3);
        CHECK(str.pos == 3);

        mli_IoMemory_close(&str);
}

CASE("BytesIo_write_rewind_read")
{
        struct mli_IoMemory byt = mli_IoMemory_init();
        int32_t evth[273];
        int32_t back[273];
        uint64_t i;
        size_t rc;
        for (i = 0; i < 273; i++) {
                evth[i] = i;
        }

        CHECK(mli_IoMemory_open(&byt));

        rc = mli_IoMemory_write(evth, sizeof(float), 273, &byt);
        CHECK(rc == 273);

        CHECK(byt.cstr != NULL);
        CHECK(byt.size <= byt.capacity);
        CHECK(byt.size == 273 * 4);
        CHECK(byt.pos == byt.size);

        mli_IoMemory_rewind(&byt);
        CHECK(byt.pos == 0u);

        rc = mli_IoMemory_read(back, sizeof(float), 273, &byt);
        CHECK(rc == 273);

        for (i = 0; i < 273; i++) {
                CHECK(evth[i] == back[i]);
        }

        rc = mli_IoMemory_read(back, sizeof(float), 1, &byt);
        CHECK(rc == 0);

        mli_IoMemory_close(&byt);
}

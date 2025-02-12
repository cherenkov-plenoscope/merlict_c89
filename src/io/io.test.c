/* Copyright Sebastian Achim Mueller */

CASE("mli_IO_init")
{
        struct mli_IO stream = mli_IO_init();
        CHECK(stream.type == MLI_IO_TYPE_VOID);
}

CASE("mli_IO_open_memory")
{
        struct mli_IO stream = mli_IO_init();
        CHECK(mli_IO_open_memory(&stream));
        mli_IO_close(&stream);
}

CASE("mli_IO_memory write /read even and odd number of bytes.")
{
        struct mli_IO f = mli_IO_init();
        struct mli_String s = mli_String_init();
        struct mli_String b = mli_String_init();
        CHECK(mli_IO_open_memory(&f));

        CHECK(mli_String_from_cstr_fromat(&s, "1234"));
        CHECK(mli_String_to_io(&s, &f));
        mli_IO_rewind(&f);
        CHECK(mli_String_from_io(&b, &f));
        CHECK(mli_String_equal_cstr(&b, "1234"));

        mli_IO_rewind(&f);

        CHECK(mli_String_from_cstr_fromat(&s, "123"));
        CHECK(mli_String_to_io(&s, &f));
        mli_IO_rewind(&f);
        CHECK(mli_String_from_io(&b, &f));
        CHECK(mli_String_equal_cstr(&b, "123"));

        mli_IO_close(&f);
        mli_String_free(&s);
        mli_String_free(&b);
}

CASE("mli_IO_open_file")
{
        struct mli_String filename = mli_String_init();
        struct mli_String mode = mli_String_init();
        struct mli_String payload = mli_String_init();

        struct mli_IO fstream = mli_IO_init();
        struct mli_IO mstream = mli_IO_init();

        CHECK(mli_String_from_cstr(&filename, "data/mli_IO.txt.tmp"));
        CHECK(mli_String_from_cstr(&mode, "w"));

        CHECK(mli_IO_open_file(&fstream, &filename, &mode));

        CHECK(mli_String_from_cstr(&payload, "A simple text."));
        CHECK(mli_IO_write(
                (void *)(&payload.array),
                sizeof(char),
                payload.size,
                &fstream));

        mli_IO_close(&fstream);

        CHECK(mli_String_from_cstr(&mode, "r"));
        CHECK(mli_IO_open_file(&fstream, &filename, &mode));
        CHECK(mli_IO_open_memory(&mstream));

        while (1) {
                char c;
                size_t rc =
                        mli_IO_read((void *)(&c), sizeof(char), 1, &fstream);
                if (rc == 0) {
                        break;
                }
                CHECK(mli_IO_write((void *)(&c), sizeof(char), 1, &mstream));
        }
        CHECK(mli_IO_tell(&mstream) == (int64_t)payload.size);

        mli_IO_close(&mstream);
        mli_String_free(&filename);
        mli_String_free(&mode);
        mli_String_free(&payload);
}

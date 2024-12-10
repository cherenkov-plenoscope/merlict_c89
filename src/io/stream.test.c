/* Copyright Sebastian Achim Mueller */

CASE("mli_Stream_init")
{
        struct mli_Stream stream = mli_Stream_init();
        CHECK(stream.type == MLI_IO_TYPE_VOID);
}

CASE("mli_Stream_open_memory")
{
        struct mli_Stream stream = mli_Stream_init();
        CHECK(mli_Stream_open_memory(&stream));
        mli_Stream_close(&stream);
}

CASE("mli_Stream_open_file")
{
        struct mli_String filename = mli_String_init();
        struct mli_String mode = mli_String_init();

        struct mli_Stream stream = mli_Stream_init();

        fprintf(stderr, "%s,%d\n", __FILE__, __LINE__);

        CHECK(mli_String_from_cstr(
                &filename, "data/mli/tests/resources/mli_Stream.txt.tmp"));
        CHECK(mli_String_from_cstr(&mode, "w"));

        fprintf(stderr, "%s,%d\n", __FILE__, __LINE__);

        CHECK(mli_Stream_open_file(&stream, &filename, &mode));

        fprintf(stderr, "%s,%d\n", __FILE__, __LINE__);

        mli_Stream_close(&stream);
        mli_String_free(&filename);
        mli_String_free(&mode);
}

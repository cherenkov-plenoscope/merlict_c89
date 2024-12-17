/* Copyright Sebastian Achim Mueller                                          */

CASE("mli_String_serialize")
{
        struct mli_IO f = mli_IO_init();
        struct mli_String first = mli_String_init();
        struct mli_String second = mli_String_init();
        char path[] = "data/"
                      "mli/"
                      "tests/"
                      "resources/"
                      "mli_String.bin.tmp";

        CHECK(mli_String_from_cstr(&first, "Yolo421!"));

        CHECK(mli_IO__open_file_cstr(&f, path, "w"));
        CHECK(mli_String_fwrite(&first, &f));
        mli_IO_close(&f);

        CHECK(mli_IO__open_file_cstr(&f, path, "r"));
        CHECK(mli_String_malloc_fread(&second, &f));
        mli_IO_close(&f);

        CHECK(mli_String_equal(&first, &second));

        mli_String_free(&first);
        mli_String_free(&second);
}

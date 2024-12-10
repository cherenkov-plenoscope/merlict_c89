/* Copyright Sebastian Achim Mueller                                          */

CASE("mli_String_serialize")
{
        FILE *f = NULL;
        struct mli_String first = mli_String_init();
        struct mli_String second = mli_String_init();
        char path[] = "data/"
                      "mli/"
                      "tests/"
                      "resources/"
                      "mli_String.bin.tmp";

        CHECK(mli_String_from_cstr(&first, "Yolo421!"));

        f = fopen(path, "w");
        CHECK(f != NULL);
        CHECK(mli_String_fwrite(&first, f));
        fclose(f);

        f = fopen(path, "r");
        CHECK(mli_String_malloc_fread(&second, f));
        fclose(f);

        CHECK(mli_String_equal(&first, &second));

        mli_String_free(&first);
        mli_String_free(&second);
}

/* Copyright Sebastian Achim Mueller */

CASE("mli_IoFile_init")
{
        struct mli_IoFile ff = mli_IoFile_init();
        CHECK(ff.filename.size == 0);
        CHECK(ff.mode.size == 0);
        CHECK(ff.cfile == NULL);
}

CASE("mli_IoFile_open_and_close")
{
        struct mli_IoFile ff = mli_IoFile_init();
        struct mli_String filename = mli_String_init();
        struct mli_String mode = mli_String_init();

        CHECK(mli_String_from_cstr(
                &filename,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "mli_IoFile.txt.tmp"));
        CHECK(mli_String_from_cstr(&mode, "w"));

        CHECK(mli_IoFile_open(&ff, &filename, &mode));
        CHECK(mli_IoFile_close(&ff));

        mli_String_free(&filename);
        mli_String_free(&mode);
}

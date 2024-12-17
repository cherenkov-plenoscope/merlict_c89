/* Copyright Sebastian Achim Mueller */

CASE("mli_IoFile_init")
{
        struct mli_IoFile ff = mli_IoFile_init();
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

CASE("mli_IoFile_open_write_close_open_read_close")
{
        struct mli_IoFile ff = mli_IoFile_init();
        struct mli_String filename = mli_String_init();
        struct mli_String mode = mli_String_init();
        const uint64_t payload = 1234567;
        uint64_t back = -1;

        CHECK(mli_String_from_cstr(
                &filename,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "mli_IoFile.txt.tmp"));
        CHECK(mli_String_from_cstr(&mode, "w"));

        CHECK(mli_IoFile_open(&ff, &filename, &mode));
        CHECK(mli_IoFile_write((void *)(&payload), sizeof(uint64_t), 1, &ff));
        CHECK(mli_IoFile_close(&ff));

        CHECK(payload != back);
        CHECK(mli_String_from_cstr(&mode, "r"));

        CHECK(mli_IoFile_open(&ff, &filename, &mode));
        CHECK(mli_IoFile_read((void *)(&back), sizeof(uint64_t), 1, &ff));
        CHECK(mli_IoFile_close(&ff));

        CHECK(payload == back);

        mli_String_free(&filename);
        mli_String_free(&mode);
}

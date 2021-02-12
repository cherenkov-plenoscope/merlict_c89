/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("Write and read tape-archive")
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char payload[1024] = {'\0'};

        sprintf(payload, "%s", "Hello world!");

        CHECK(mliTar_open(&tar, "tests/resources/123.tar.tmp", "w"));
        CHECK(mliTar_write_dir_header(&tar, "resources"));
        CHECK(mliTar_write_file_header(
                &tar, "resources/hans.txt", strlen(payload)));
        CHECK(mliTar_write_data(&tar, payload, strlen(payload)));
        CHECK(mliTar_finalize(&tar));
        CHECK(mliTar_close(&tar));

        memset(payload, '\0', sizeof(payload));

        CHECK(mliTar_open(&tar, "tests/resources/123.tar.tmp", "r"));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("resources", tarh.name));
        CHECK(tarh.type == MLITAR_TDIR);

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("resources/hans.txt", tarh.name));
        CHECK(mliTar_read_data(&tar, payload, tarh.size));
        CHECK(0 == strcmp("Hello world!", payload));

        CHECK(mliTar_close(&tar));
}

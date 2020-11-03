/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("Read archive")
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char payload[128];

        CHECK(tar.pos == 0u);
        CHECK(tar.remaining_data == 0u);

        CHECK(mliTar_open(&tar, "tests/resources/minimal.tar", "r"));
        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("function.csv", tarh.name));
        CHECK(79 == tarh.size);

        memset(payload, '\0', sizeof(payload));
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("README.md", tarh.name));
        CHECK(75 == tarh.size);

        memset(payload, '\0', sizeof(payload));
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("scenery.json", tarh.name));
        CHECK(24 == tarh.size);

        memset(payload, '\0', sizeof(payload));
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("triangle.obj", tarh.name));
        CHECK(90 == tarh.size);

        memset(payload, '\0', sizeof(payload));
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_close(&tar));
}


CASE("Write and read tape-archive")
{
        uint64_t  i = 0;
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char payload[1024];

        sprintf(payload, "%s", "Hello world!");

        CHECK(mliTar_open(&tar, "tests/resources/123.tar.tmp", "w"));
        CHECK(mliTar_write_dir_header(&tar, "resources"));
        CHECK(mliTar_write_file_header(
                &tar, "resources/hans.txt",
                strlen(payload))
        );
        CHECK(mliTar_write_data(&tar, payload, strlen(payload)));
        CHECK(mliTar_finalize(&tar));
        CHECK(mliTar_close(&tar));

        for (i = 0; i < 1024; i++) {payload[i] = '\0';}

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

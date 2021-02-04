/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("Read archive")
{
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char *payload = NULL;
        const uint64_t payload_capacity = 1000 * 1000;

        payload = (char *)malloc(sizeof(char) * payload_capacity);
        CHECK(payload != NULL);

        CHECK(tar.pos == 0u);
        CHECK(tar.remaining_data == 0u);

        CHECK(mliTar_open(
                &tar,
                "tests/"
                "resources/"
                "sceneries/"
                "000.tar",
                "r"));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./", tarh.name));
        CHECK(MLITAR_TDIR == tarh.type);

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./README.md", tarh.name));
        CHECK(61 == tarh.size);
        memset(payload, '\0', payload_capacity);
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./functions/", tarh.name));
        CHECK(MLITAR_TDIR == tarh.type);

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./functions/zero.csv", tarh.name));
        CHECK(41 == tarh.size);
        memset(payload, '\0', payload_capacity);
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./materials.json", tarh.name));
        memset(payload, '\0', payload_capacity);
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./objects/", tarh.name));
        CHECK(MLITAR_TDIR == tarh.type);

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./objects/triangle.obj", tarh.name));
        CHECK(110 == tarh.size);
        memset(payload, '\0', payload_capacity);
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("./tree.json", tarh.name));
        CHECK(690 == tarh.size);
        memset(payload, '\0', payload_capacity);
        CHECK(mliTar_read_data(&tar, payload, tarh.size));

        CHECK(mliTar_close(&tar));
        free(payload);
}

CASE("Write and read tape-archive")
{
        uint64_t i = 0;
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char payload[1024];

        sprintf(payload, "%s", "Hello world!");

        CHECK(mliTar_open(&tar, "tests/resources/123.tar.tmp", "w"));
        CHECK(mliTar_write_dir_header(&tar, "resources"));
        CHECK(mliTar_write_file_header(
                &tar, "resources/hans.txt", strlen(payload)));
        CHECK(mliTar_write_data(&tar, payload, strlen(payload)));
        CHECK(mliTar_finalize(&tar));
        CHECK(mliTar_close(&tar));

        for (i = 0; i < 1024; i++) {
                payload[i] = '\0';
        }

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

/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("Read archive")
{
        uint64_t  i = 0;
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char payload[1024];

        CHECK(tar.pos == 0u);
        CHECK(tar.remaining_data == 0u);

        CHECK(mliTar_open(&tar, "tests/resources/minimal.tar", "r"));
        CHECK(mliTar_read_header(&tar, &tarh));
        fprintf(stderr, "tarh.name: '%s'\n", tarh.name);
        fprintf(stderr, "tarh.linkname: '%s'\n", tarh.linkname);
        fprintf(stderr, "tarh.size: '%ld'\n", tarh.size);
        for (i = 0; i < 1024; i++) {payload[i] = '\0';}
        CHECK(mliTar_read_data(&tar, payload, tarh.size));
        fprintf(stderr, "payload:\n%s", payload);

        CHECK(mliTar_read_header(&tar, &tarh));
        fprintf(stderr, "tarh.name: '%s'\n", tarh.name);
        fprintf(stderr, "tarh.linkname: '%s'\n", tarh.linkname);
        fprintf(stderr, "tarh.size: '%ld'\n", tarh.size);
        for (i = 0; i < 1024; i++) {payload[i] = '\0';}
        CHECK(mliTar_read_data(&tar, payload, tarh.size));
        fprintf(stderr, "payload:\n%s", payload);

        CHECK(mliTar_read_header(&tar, &tarh));
        fprintf(stderr, "tarh.name: '%s'\n", tarh.name);
        fprintf(stderr, "tarh.linkname: '%s'\n", tarh.linkname);
        fprintf(stderr, "tarh.size: '%ld'\n", tarh.size);
        for (i = 0; i < 1024; i++) {payload[i] = '\0';}
        CHECK(mliTar_read_data(&tar, payload, tarh.size));
        fprintf(stderr, "payload:\n%s", payload);

        CHECK(mliTar_read_header(&tar, &tarh));
        fprintf(stderr, "tarh.name: '%s'\n", tarh.name);
        fprintf(stderr, "tarh.linkname: '%s'\n", tarh.linkname);
        fprintf(stderr, "tarh.size: '%ld'\n", tarh.size);
        for (i = 0; i < 1024; i++) {payload[i] = '\0';}
        CHECK(mliTar_read_data(&tar, payload, tarh.size));
        fprintf(stderr, "payload:\n%s", payload);

        CHECK(mliTar_close(&tar));
}


CASE("Write and read tape-archive")
{
        uint64_t  i = 0;
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char payload[1024];

        sprintf(payload, "%s", "Hello world!");

        CHECK(mliTar_open(&tar, "tests/resources/123.tar", "w"));
        CHECK(mliTar_write_dir_header(&tar, "resources"));
        CHECK(mliTar_write_file_header(
                &tar, "resources/hans.txt",
                strlen(payload))
        );
        CHECK(mliTar_write_data(&tar, payload, strlen(payload)));
        CHECK(mliTar_finalize(&tar));
        CHECK(mliTar_close(&tar));

        for (i = 0; i < 1024; i++) {payload[i] = '\0';}

        CHECK(mliTar_open(&tar, "tests/resources/123.tar", "r"));

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("resources", tarh.name));
        CHECK(tarh.type == MLITAR_TDIR);

        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("resources/hans.txt", tarh.name));
        CHECK(mliTar_read_data(&tar, payload, tarh.size));
        CHECK(0 == strcmp("Hello world!", payload));

        CHECK(mliTar_close(&tar));
}

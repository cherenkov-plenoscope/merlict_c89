/* Copyright 2019 Sebastian Achim Mueller                                     */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliTar.h"

CASE("Write and read tape-archive")
{
        FILE *f = NULL;
        struct mliTar tar = mliTar_init();
        struct mliTarHeader tarh = mliTarHeader_init();
        char payload[1024] = {'\0'};

        sprintf(payload, "%s", "Hello world!");

        f = fopen("mli_core/tests/resources/mliTar/123.tar.tmp", "wb");
        CHECK(mliTar_write_begin(&tar, f));
        CHECK(mliTarHeader_set_directory(&tarh, "resources"));
        CHECK(mliTar_write_header(&tar, &tarh));
        CHECK(mliTarHeader_set_normal_file(
                &tarh, "resources/hans.txt", strlen(payload)));
        CHECK(mliTar_write_header(&tar, &tarh));
        CHECK(mliTar_write_data(&tar, payload, strlen(payload)));
        CHECK(mliTar_write_finalize(&tar));
        fclose(f);

        memset(payload, '\0', sizeof(payload));

        f = fopen("mli_core/tests/resources/mliTar/123.tar.tmp", "rb");
        tar = mliTar_init();
        CHECK(mliTar_read_begin(&tar, f));
        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("resources", tarh.name));
        CHECK(tarh.type == MLI_TAR_DIRECTORY);
        CHECK(mliTar_read_header(&tar, &tarh));
        CHECK(0 == strcmp("resources/hans.txt", tarh.name));
        CHECK(mliTar_read_data(&tar, payload, tarh.size));
        CHECK(0 == strcmp("Hello world!", payload));
        CHECK(mliTar_read_finalize(&tar));
        fclose(f);
}

CASE("ustar2001_size_base_256_init")
{
        char field[12];
        uint64_t val = 0lu;
        CHECK(mliTar_uint64_to_field12_2001star_base256(val, field));
        CHECK(field[0] == -128);
        CHECK(field[1] == 0);
        CHECK(field[2] == 0);
        CHECK(field[3] == 0);
        CHECK(field[4] == 0);
        CHECK(field[5] == 0);
        CHECK(field[6] == 0);
        CHECK(field[7] == 0);
        CHECK(field[8] == 0);
        CHECK(field[9] == 0);
        CHECK(field[10] == 0);
        CHECK(field[11] == 0);
}

CASE("ustar2001_size_base_256")
{
        char field[12];
        uint8_t uf[12];
        uint64_t val = 10lu * 1000lu * 1000lu * 1000lu;
        uint64_t val_back = 0;

        memset(field, '\0', 12);
        CHECK(mliTar_uint64_to_field12_2001star_base256(val, field));
        memcpy(uf, field, 12);

        CHECK(uf[0] == 0x80);
        CHECK(uf[1] == 0x00);
        CHECK(uf[2] == 0x00);
        CHECK(uf[3] == 0x00);
        CHECK(uf[4] == 0x00);
        CHECK(uf[5] == 0x00);
        CHECK(uf[6] == 0x00);
        CHECK(uf[7] == 0x02);
        CHECK(uf[8] == 0x54);
        CHECK(uf[9] == 0x0b);
        CHECK(uf[10] == 0xe4);
        CHECK(uf[11] == 0x00);

        CHECK(mliTar_field12_to_uint64_2001star_base256(field, &val_back));

        CHECK(val_back == val);
}

CASE("ustar2001_size_base_256_loop")
{
        char f[12];
        uint64_t i;
        uint64_t v = 0lu;
        uint64_t vb = 0lu;

        for (i = 0; i < 100; i++) {
                v = i * i * i * 211 + i * i * 73 + i * 7;
                CHECK(mliTar_uint64_to_field12_2001star_base256(v, f));
                CHECK(mliTar_field12_to_uint64_2001star_base256(f, &vb));
                CHECK(vb == v);
        }
}

CASE("ustar2001_size_base_256_exceeds_uint64")
{
        char field[12];
        uint8_t uf[12];
        uint64_t v = 0lu;

        uf[0] = 128;
        uf[1] = 0;
        uf[2] = 0;
        uf[3] = 1; /* <--- */
        uf[4] = 0;
        uf[5] = 0;
        uf[6] = 0;
        uf[7] = 0;
        uf[8] = 0;
        uf[9] = 0;
        uf[10] = 0;
        uf[11] = 0;

        memcpy(field, uf, 12);
        CHECK(!mliTar_field12_to_uint64_2001star_base256(field, &v));
}

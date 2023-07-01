/* Copyright 2018-2023 Sebastian Achim Mueller */

#include "mli_testing.h"
#include "mliIo.h"

CASE("BytesIo_init")
{
        struct mliIo byt = mliIo_init();

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}


CASE("BytesIo_malloc")
{
        struct mliIo byt = mliIo_init();
        CHECK(mliIo_malloc(&byt));

        CHECK(byt.cstr != NULL);
        CHECK(byt.capacity == 2u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);

        mliIo_free(&byt);
}


CASE("BytesIo_putc")
{
        struct mliIo byt = mliIo_init();
        uint64_t i;
        CHECK(mliIo_malloc(&byt));

        for (i = 0; i < 20; i++) {
                CHECK(mliIo_putc(&byt, 'A'));

                CHECK(byt.cstr != NULL);
                CHECK(byt.pos == i + 1);
                CHECK(byt.pos == byt.size);
                CHECK(byt.capacity > byt.size);

                /* always terminated with '\0' */
                CHECK(byt.cstr[byt.pos] == '\0');
                CHECK(strlen((char *)byt.cstr) == byt.size);
        }
        mliIo_free(&byt);

        CHECK(byt.cstr == NULL);
        CHECK(byt.capacity == 0u);
        CHECK(byt.size == 0u);
        CHECK(byt.pos == 0u);
}


CASE("BytesIo_putc_rewind_getc")
{
        struct mliIo byt = mliIo_init();
        CHECK(mliIo_malloc(&byt));

        CHECK(mliIo_putc(&byt, 'A'));
        CHECK(mliIo_putc(&byt, 'B'));
        CHECK(mliIo_putc(&byt, 'C'));

        mliIo_rewind(&byt);
        CHECK(byt.pos == 0u);

        CHECK(mliIo_getc(&byt) == 'A');
        CHECK(mliIo_getc(&byt) == 'B');
        CHECK(mliIo_getc(&byt) == 'C');
        CHECK(mliIo_getc(&byt) == EOF);

        mliIo_free(&byt);
}

CASE("BytesIo_write_rewind_read")
{
        struct mliIo byt = mliIo_init();
        int32_t evth[273];
        int32_t back[273];
        uint64_t i;
        int64_t rc;
        for (i = 0; i < 273; i++) {
                evth[i] = i;
        }

        CHECK(mliIo_malloc(&byt));

        rc = mliIo_write(&byt, evth, sizeof(float), 273);
        CHECK(rc == 273);

        CHECK(byt.cstr != NULL);
        CHECK(byt.size <= byt.capacity);
        CHECK(byt.size == 273 * 4);
        CHECK(byt.pos == byt.size);

        mliIo_rewind(&byt);
        CHECK(byt.pos == 0u);

        rc = mliIo_read(&byt, back, sizeof(float), 273);
        CHECK(rc == 273);

        for (i = 0; i < 273; i++) {
                CHECK(evth[i] == back[i]);
        }

        rc = mliIo_read(&byt, back, sizeof(float), 1);
        CHECK(rc == EOF);

        mliIo_free(&byt);
}

CASE("BytesIo_printf")
{
        struct mliIo byt = mliIo_init();

        mliIo_printf(&byt, "Hans %03d Lok %.3fh!", 8, 3.141);
        mliIo_rewind(&byt);

        CHECK(0 == strcmp((char *)byt.cstr, "Hans 008 Lok 3.141h!"));

        mliIo_free(&byt);
}

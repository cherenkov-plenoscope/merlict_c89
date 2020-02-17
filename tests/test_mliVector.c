/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("malloc, and free") {
    uint64_t i;
    mliVector vec = mliVector_init();
    CHECK(mliVector_malloc(&vec, 0u, sizeof(struct mliColor)));
    CHECK(vec.size == 0u);

    for (i = 0; i < 100; i++) {
        struct mliColor color = mliColor_set(i*1., i*2., i*3.);
        CHECK(vec.size == i);
        CHECK(mliVector_push_back(&vec, &color));
        CHECK(vec.size == i + 1);}

    for (i = 0; i < vec.size; i++) {
        struct mliColor color = *(struct mliColor*)mliVector_at(&vec, i);
        CHECK_MARGIN(color.r, i*1., 1e-9);
        CHECK_MARGIN(color.g, i*2., 1e-9);
        CHECK_MARGIN(color.b, i*3., 1e-9);
        CHECK(vec.size == 100);}

    mliVector_free(&vec);
    CHECK(vec.size == 0);
    CHECK(vec.capacity == 0);
}

CASE("with pointers to mliColor") {
    uint64_t i;
    mliVector vec = mliVector_init();
    CHECK(mliVector_malloc(&vec, 0u, sizeof(struct mliColor*)));
    CHECK(vec.size == 0u);

    for (i = 0; i < 10; i++) {
        struct mliColor *ptr_color_in = (struct mliColor*)(i*i);
        CHECK(vec.size == i);
        CHECK(mliVector_push_back(&vec, &ptr_color_in));
        CHECK(vec.size == i + 1);}

    for (i = 0; i < vec.size; i++) {
        struct mliColor *ptr_color_out =
            *((struct mliColor**)mliVector_at(&vec, i));
        CHECK(ptr_color_out == (struct mliColor*)(i*i));
        CHECK(vec.size == 10);}

    mliVector_free(&vec);
    CHECK(vec.size == 0);
    CHECK(vec.capacity == 0);
}

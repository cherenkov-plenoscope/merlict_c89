/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_triangle_surface_normal")
{
        struct mliVec n = mli_triangle_surface_normal(
                mliVec_set(-1, 0, 0), mliVec_set(1, 0, 0), mliVec_set(0, 1, 0));

        CHECK(mliVec_equal_margin(n, mliVec_set(0, 0, 1), 1e-9));
}

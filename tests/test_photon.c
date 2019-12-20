/* Copyright 2019 Sebastian Achim Mueller                                     */
{
    mliPhoton ph = mliPhoton_set(
        mliRay_set(mliVec_set(1, 2, 3), mliVec_set(0, 0, 1)),
        350e-9);
    CHECK(ph.wavelength == 350e-9);
    CHECK(mliVec_equal_margin(ph.ray.support, mliVec_set(1, 2, 3), 1e-9));
    CHECK(mliVec_equal_margin(ph.ray.direction, mliVec_set(0, 0, 1), 1e-9));
}

/* Copyright 2019-2020 Sebastian Achim Mueller                                */


CASE("parallel_towards_z_from_xy_disc") {
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliDynPhoton photons = mliDynPhoton_init();
        const double disc_radius = 1.23;
        const double wavelength = 433e-9;
        const uint64_t num_photons = 1000;
        uint64_t i;
        CHECK(mliDynPhoton_malloc(&photons, 0));

        CHECK(mli_photon_source_parallel_towards_z_from_xy_disc(
                &photons,
                wavelength,
                disc_radius,
                num_photons,
                &prng));

        for (i = 0; i < photons.dyn.size; i++) {
                CHECK(photons.arr[i].ray.support.z == 0.0);
                CHECK(
                        mli_hypot(
                                photons.arr[i].ray.support.x,
                                photons.arr[i].ray.support.y) <= disc_radius);
                CHECK(photons.arr[i].ray.direction.x == 0.0);
                CHECK(photons.arr[i].ray.direction.y == 0.0);
                CHECK(photons.arr[i].ray.direction.z == 1.0);
                CHECK(photons.arr[i].wavelength == wavelength);
                CHECK(photons.arr[i].simulation_truth_id == i);
        }

        mliDynPhoton_free(&photons);
}

/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("parallel_towards_z_from_xy_disc")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliDynPhoton photons = mliDynPhoton_init();
        const double disc_radius = 1.23;
        const double wavelength = 433e-9;
        const uint64_t num_photons = 1000;
        uint64_t i;
        CHECK(mliDynPhoton_malloc(&photons, 0));

        CHECK(mli_photon_source_parallel_towards_z_from_xy_disc(
                &photons, wavelength, disc_radius, num_photons, &prng));

        for (i = 0; i < photons.dyn.size; i++) {
                CHECK(photons.arr[i].ray.support.z == 0.0);
                CHECK(mli_hypot(
                              photons.arr[i].ray.support.x,
                              photons.arr[i].ray.support.y) <= disc_radius);
                CHECK(photons.arr[i].ray.direction.x == 0.0);
                CHECK(photons.arr[i].ray.direction.y == 0.0);
                CHECK(photons.arr[i].ray.direction.z == 1.0);
                CHECK(photons.arr[i].wavelength == wavelength);
                CHECK(photons.arr[i].id == (int64_t)i);
        }

        mliDynPhoton_free(&photons);
}

CASE("point_like_towards_z")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliDynPhoton photons = mliDynPhoton_init();
        const double opening_angle = mli_deg2rad(1.0);
        const double wavelength = 433e-9;
        const uint64_t num_photons = 1000;
        uint64_t i;
        CHECK(mliDynPhoton_malloc(&photons, 0));

        CHECK(point_like_towards_z_opening_angle_num_photons(
                &photons, wavelength, opening_angle, num_photons, &prng));

        for (i = 0; i < photons.dyn.size; i++) {
                CHECK(photons.arr[i].ray.support.z == 0.0);
                CHECK(photons.arr[i].ray.support.x == 0.0);
                CHECK(photons.arr[i].ray.support.y == 0.0);
                CHECK_MARGIN(
                        mliVec_norm(photons.arr[i].ray.direction), 1.0, 1e-6);
                CHECK(mliVec_angle_between(
                              photons.arr[i].ray.direction,
                              mliVec_set(0., 0., 1.)) <= opening_angle);
                CHECK(photons.arr[i].wavelength == wavelength);
                CHECK(photons.arr[i].id == (int64_t)i);
        }

        mliDynPhoton_free(&photons);
}

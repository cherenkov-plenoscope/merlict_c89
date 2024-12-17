/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("parallel_towards_z_from_xy_disc")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        struct mli_PhotonVector photons = mli_PhotonVector_init();
        const double disc_radius = 1.23;
        const double wavelength = 433e-9;
        const uint64_t num_photons = 1000;
        uint64_t i;
        CHECK(mli_PhotonVector_malloc(&photons, 0));

        CHECK(mli_photon_source_parallel_towards_z_from_xy_disc(
                &photons, wavelength, disc_radius, num_photons, &prng));

        for (i = 0; i < photons.size; i++) {
                CHECK(photons.array[i].ray.support.z == 0.0);
                CHECK(mli_math_hypot(
                              photons.array[i].ray.support.x,
                              photons.array[i].ray.support.y) <= disc_radius);
                CHECK(photons.array[i].ray.direction.x == 0.0);
                CHECK(photons.array[i].ray.direction.y == 0.0);
                CHECK(photons.array[i].ray.direction.z == 1.0);
                CHECK(photons.array[i].wavelength == wavelength);
                CHECK(photons.array[i].id == (int64_t)i);
        }

        mli_PhotonVector_free(&photons);
}

CASE("point_like_towards_z")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        struct mli_PhotonVector photons = mli_PhotonVector_init();
        const double opening_angle = mli_math_deg2rad(1.0);
        const double wavelength = 433e-9;
        const uint64_t num_photons = 1000;
        uint64_t i;
        CHECK(mli_PhotonVector_malloc(&photons, 0));

        CHECK(mli_photon_source_point_like_opening_cone_towards_z(
                &photons, wavelength, opening_angle, num_photons, &prng));

        for (i = 0; i < photons.size; i++) {
                CHECK(photons.array[i].ray.support.z == 0.0);
                CHECK(photons.array[i].ray.support.x == 0.0);
                CHECK(photons.array[i].ray.support.y == 0.0);
                CHECK_MARGIN(
                        mli_Vec_norm(photons.array[i].ray.direction),
                        1.0,
                        1e-6);
                CHECK(mli_Vec_angle_between(
                              photons.array[i].ray.direction,
                              mli_Vec_init(0., 0., 1.)) <= opening_angle);
                CHECK(photons.array[i].wavelength == wavelength);
                CHECK(photons.array[i].id == (int64_t)i);
        }

        mli_PhotonVector_free(&photons);
}

/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Surface_json")
{
        struct mli_Surface srf = mli_Surface_init();
        struct mli_String str = mli_String_init();
        CHECK(mli_String_from_cstr(
                &str,
                "{"
                "\"material\": \"Phong\","
                "\"specular_reflection\": [[0, 10], [1, 11], [2, 12]],"
                "\"diffuse_reflection\": [[5, 50], [6, 51], [7, 52]]"
                "}"));
        CHECK(mli_Surface_malloc_from_json_string(&srf, &str));

        CHECK(srf.material == MLI_SURFACE_PHONG);

        CHECK(srf.specular_reflection.num_points == 3);
        CHECK(srf.specular_reflection.x[0] == 0);
        CHECK(srf.specular_reflection.x[1] == 1);
        CHECK(srf.specular_reflection.x[2] == 2);

        CHECK(srf.specular_reflection.y[0] == 10);
        CHECK(srf.specular_reflection.y[1] == 11);
        CHECK(srf.specular_reflection.y[2] == 12);

        CHECK(srf.diffuse_reflection.num_points == 3);
        CHECK(srf.diffuse_reflection.x[0] == 5);
        CHECK(srf.diffuse_reflection.x[1] == 6);
        CHECK(srf.diffuse_reflection.x[2] == 7);

        CHECK(srf.diffuse_reflection.y[0] == 50);
        CHECK(srf.diffuse_reflection.y[1] == 51);
        CHECK(srf.diffuse_reflection.y[2] == 52);

        mli_Surface_free(&srf);
        mli_String_free(&str);
}

/* Copyright 2019-2024 Sebastian Achim Mueller                                */

CASE("boundarylayer_surface_phong")
{
        struct mli_BoundaryLayer_Surface_Phong phong;
        struct mli_String json_string = mli_String_init();
        struct mli_String key = mli_String_init();
        struct mli_Map spectra_names = mli_Map_init();

        CHECK(mli_Map_malloc(&spectra_names));
        CHECK(mli_String_from_cstr(&key, "Leet"));
        CHECK(mli_Map_insert(&spectra_names, &key, 1337));

        CHECK(mli_String_from_cstr(
                &json_string,
                "{"
                "\"reflection_spectrum\": \"Leet\","
                "\"diffuse_weight\": 0.6,"
                "\"specular_weight\": 0.3,"
                "\"shininess\": 32.0"
                "}"));

        CHECK(mli_BoundaryLayer_Surface_Phong_from_json_string(
                &phong, &spectra_names, &json_string));

        CHECK(phong.reflection_spectrum == 1337u);
        CHECK_MARGIN(phong.diffuse_weight, 0.6, 1e-9);
        CHECK_MARGIN(phong.specular_weight, 0.3, 1e-9);
        CHECK_MARGIN(phong.shininess, 32.0, 1e-9);

        mli_String_free(&key);
        mli_String_free(&json_string);
        mli_Map_free(&spectra_names);
}

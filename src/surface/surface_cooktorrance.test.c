/* Copyright 2019-2024 Sebastian Achim Mueller                                */

CASE("surface_CookTorrance")
{
        struct mli_Surface_CookTorrance cook;
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
                "\"roughness\": 0.2"
                "}"));

        CHECK(mli_Surface_CookTorrance_from_json_string(
                &cook, &spectra_names, &json_string));

        CHECK(cook.reflection_spectrum == 1337u);
        CHECK_MARGIN(cook.diffuse_weight, 0.6, 1e-9);
        CHECK_MARGIN(cook.specular_weight, 0.3, 1e-9);
        CHECK_MARGIN(cook.roughness, 0.2, 1e-9);

        mli_String_free(&key);
        mli_String_free(&json_string);
        mli_Map_free(&spectra_names);
}

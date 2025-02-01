/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("boundarylayer_surface")
{
        struct mli_Surface a = mli_Surface_init();
        CHECK(a.type == MLI_SURFACE_TYPE_NONE);
}

CASE("boundarylayer_surface_model_names")
{
        struct mli_String s = mli_String_init();
        uint64_t type = MLI_SURFACE_TYPE_NONE;

        CHECK(!mli_Surface_type_to_string(MLI_SURFACE_TYPE_NONE, &s));

        CHECK(mli_Surface_type_to_string(MLI_SURFACE_TYPE_TRANSPARENT, &s));
        CHECK(mli_String_equal_cstr(&s, "transparent"));
        CHECK(mli_Surface_type_from_string(&s, &type));
        CHECK(type == MLI_SURFACE_TYPE_TRANSPARENT);

        CHECK(mli_Surface_type_to_string(MLI_SURFACE_TYPE_MIRROR, &s));
        CHECK(mli_String_equal_cstr(&s, "mirror"));
        CHECK(mli_Surface_type_from_string(&s, &type));
        CHECK(type == MLI_SURFACE_TYPE_MIRROR);

        CHECK(mli_Surface_type_to_string(MLI_SURFACE_TYPE_PHONG, &s));
        CHECK(mli_String_equal_cstr(&s, "phong"));
        CHECK(mli_Surface_type_from_string(&s, &type));
        CHECK(type == MLI_SURFACE_TYPE_PHONG);

        CHECK(mli_Surface_type_to_string(MLI_SURFACE_TYPE_COOK_TORRANCE, &s));
        CHECK(mli_String_equal_cstr(&s, "cook-torrance"));
        CHECK(mli_Surface_type_from_string(&s, &type));
        CHECK(type == MLI_SURFACE_TYPE_COOK_TORRANCE);

        mli_String_free(&s);
}

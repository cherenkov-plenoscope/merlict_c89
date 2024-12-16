/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("Fresnel: orthogonal_incident")
{
        double n_from = 1.0;
        double n_going_to = 1.33;
        struct mli_Vec incident = {0.0, 0.0, -1.0};
        struct mli_Vec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;

        incident = mli_Vec_normalized(incident);
        normal = mli_Vec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);

        CHECK(0.0 < mliFresnel_reflection_propability(fresnel));
        CHECK(0.05 > mliFresnel_reflection_propability(fresnel));
        CHECK(mli_Vec_equal_margin(
                mliFresnel_refraction_direction(fresnel), incident, 1e-9));
}

CASE("Fresnel: flat_incident")
{
        double n_from = 1.0;
        double n_going_to = 1.33;
        struct mli_Vec incident = {100.0, 0.0, -1.0};
        struct mli_Vec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;
        double incident_to_normal;
        double outgoing_to_normal;
        double rec_outgoing_to_normal;

        incident = mli_Vec_normalized(incident);
        normal = mli_Vec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);

        incident_to_normal = mli_Vec_angle_between(incident, normal);
        outgoing_to_normal =
                asin(sin(incident_to_normal) * n_from / n_going_to);

        rec_outgoing_to_normal = mli_Vec_angle_between(
                mliFresnel_refraction_direction(fresnel),
                mli_Vec_multiply(normal, -1.0));

        CHECK_MARGIN(rec_outgoing_to_normal, outgoing_to_normal, 1e-9);
}

CASE("Fresnel: orthogonal_incident_same_index")
{
        double n_from = 1.0;
        double n_going_to = n_from;
        struct mli_Vec incident = {0.0, 0.0, -1.0};
        struct mli_Vec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;

        incident = mli_Vec_normalized(incident);
        normal = mli_Vec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);
        CHECK(0.0 == mliFresnel_reflection_propability(fresnel));
        CHECK(mli_Vec_equal_margin(
                mliFresnel_refraction_direction(fresnel), incident, 1e-9));
}

CASE("Fresnel: flat_incident_same_index")
{
        double n_from = 1.0;
        double n_going_to = n_from;
        struct mli_Vec incident = {100.0, 0.0, -1.0};
        struct mli_Vec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;

        incident = mli_Vec_normalized(incident);
        normal = mli_Vec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);
        CHECK(mli_Vec_equal_margin(
                mliFresnel_refraction_direction(fresnel), incident, 1e-9));
}

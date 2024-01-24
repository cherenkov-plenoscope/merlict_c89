/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliFresnel.h"

CASE("Fresnel: orthogonal_incident")
{
        double n_from = 1.0;
        double n_going_to = 1.33;
        struct mliVec incident = {0.0, 0.0, -1.0};
        struct mliVec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;

        incident = mliVec_normalized(incident);
        normal = mliVec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);

        CHECK(0.0 < mliFresnel_reflection_propability(fresnel));
        CHECK(0.05 > mliFresnel_reflection_propability(fresnel));
        CHECK(mliVec_equal_margin(
                mliFresnel_refraction_direction(fresnel), incident, 1e-9));
}

CASE("Fresnel: flat_incident")
{
        double n_from = 1.0;
        double n_going_to = 1.33;
        struct mliVec incident = {100.0, 0.0, -1.0};
        struct mliVec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;
        double incident_to_normal;
        double outgoing_to_normal;
        double rec_outgoing_to_normal;

        incident = mliVec_normalized(incident);
        normal = mliVec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);

        incident_to_normal = mliVec_angle_between(incident, normal);
        outgoing_to_normal =
                asin(sin(incident_to_normal) * n_from / n_going_to);

        rec_outgoing_to_normal = mliVec_angle_between(
                mliFresnel_refraction_direction(fresnel),
                mliVec_multiply(normal, -1.0));

        CHECK_MARGIN(rec_outgoing_to_normal, outgoing_to_normal, 1e-9);
}

CASE("Fresnel: orthogonal_incident_same_index")
{
        double n_from = 1.0;
        double n_going_to = n_from;
        struct mliVec incident = {0.0, 0.0, -1.0};
        struct mliVec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;

        incident = mliVec_normalized(incident);
        normal = mliVec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);
        CHECK(0.0 == mliFresnel_reflection_propability(fresnel));
        CHECK(mliVec_equal_margin(
                mliFresnel_refraction_direction(fresnel), incident, 1e-9));
}

CASE("Fresnel: flat_incident_same_index")
{
        double n_from = 1.0;
        double n_going_to = n_from;
        struct mliVec incident = {100.0, 0.0, -1.0};
        struct mliVec normal = {0.0, 0.0, 1.0};
        struct mliFresnel fresnel;

        incident = mliVec_normalized(incident);
        normal = mliVec_normalized(normal);
        fresnel = mliFresnel_init(incident, normal, n_from, n_going_to);
        CHECK(mliVec_equal_margin(
                mliFresnel_refraction_direction(fresnel), incident, 1e-9));
}

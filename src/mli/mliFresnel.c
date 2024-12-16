/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFresnel.h"
#include <math.h>

struct mliFresnel mliFresnel_init(
        const struct mli_Vec incident,
        const struct mli_Vec normal,
        const double n_from,
        const double n_to)
{
        struct mliFresnel fresnel;
        fresnel.incident = incident;
        fresnel.normal = normal;
        fresnel.n_from = n_from;
        fresnel.n_to = n_to;

        fresnel._cosI = -1.0 * mli_Vec_dot(normal, incident);
        fresnel._n_from_over_n_to = n_from / n_to;
        fresnel._sinT2 =
                (fresnel._n_from_over_n_to * fresnel._n_from_over_n_to) *
                (1.0 - (fresnel._cosI * fresnel._cosI));
        fresnel._cosT = sqrt(1.0 - fresnel._sinT2);
        return fresnel;
}

double mliFresnel_reflection_propability(const struct mliFresnel fresnel)
{
        if (fresnel._sinT2 > 1.0) {
                /* total internal reflection */
                return 1.0;
        } else {
                const struct mliFresnel f = fresnel;
                const double nFromCosI = f.n_from * f._cosI;
                const double nFromCosT = f.n_from * f._cosT;
                const double nToCosI = f.n_to * f._cosI;
                const double nToCosT = f.n_to * f._cosT;
                const double rOrth =
                        (nFromCosI - nToCosT) / (nFromCosI + nToCosT);
                const double rPara =
                        (nToCosI - nFromCosT) / (nToCosI + nFromCosT);
                return (rOrth * rOrth + rPara * rPara) / 2.0;
        }
}

struct mli_Vec mliFresnel_reflection_direction(const struct mliFresnel fresnel)
{
        return mli_Vec_add(
                fresnel.incident,
                mli_Vec_multiply(fresnel.normal, fresnel._cosI * 2.0));
}

struct mli_Vec mliFresnel_refraction_direction(const struct mliFresnel fresnel)
{
        return mli_Vec_add(
                mli_Vec_multiply(fresnel.incident, fresnel._n_from_over_n_to),
                mli_Vec_multiply(
                        fresnel.normal,
                        fresnel._n_from_over_n_to * fresnel._cosI -
                                fresnel._cosT));
}

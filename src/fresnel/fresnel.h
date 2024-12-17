/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FRESNEL_H_
#define MLI_FRESNEL_H_

#include "../vec/vec.h"

struct mli_Fresnel {
        struct mli_Vec incident;
        struct mli_Vec normal;
        double n_from;
        double n_to;

        double _cosI;
        double _n_from_over_n_to;
        double _sinT2;
        double _cosT;
};

struct mli_Vec mli_Fresnel_refraction_direction(
        const struct mli_Fresnel fresnel);
struct mli_Vec mli_Fresnel_reflection_direction(
        const struct mli_Fresnel fresnel);
double mli_Fresnel_reflection_propability(const struct mli_Fresnel fresnel);
struct mli_Fresnel mli_Fresnel_init(
        const struct mli_Vec incident,
        const struct mli_Vec normal,
        const double n_from,
        const double n_to);
#endif

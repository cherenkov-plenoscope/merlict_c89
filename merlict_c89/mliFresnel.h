/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIFRESNEL_H_
#define MLIFRESNEL_H_

struct mliFresnel {
        struct mliVec incident;
        struct mliVec normal;
        double n_from;
        double n_to;

        double _cosI;
        double _n_from_over_n_to;
        double _sinT2;
        double _cosT;
};

struct mliVec mliFresnel_refraction_direction(const struct mliFresnel fresnel);
struct mliVec mliFresnel_reflection_direction(const struct mliFresnel fresnel);
double mliFresnel_reflection_propability(const struct mliFresnel fresnel);
struct mliFresnel mliFresnel_init(
        const struct mliVec incident,
        const struct mliVec normal,
        const double n_from,
        const double n_to);
#endif

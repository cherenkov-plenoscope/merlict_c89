/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIFRESNEL_H_
#define MERLICT_MLIFRESNEL_H_

#include <math.h>

typedef struct {
    mliVec incident;
    mliVec normal;
    double n_from;
    double n_to;

    double _cosI;
    double _n_from_over_n_to;
    double _sinT2;
    double _cosT;
} mliFresnel;

mliFresnel mliFresnel_init(
    const mliVec incident,
    const mliVec normal,
    const double n_from,
    const double n_to) {
    mliFresnel fresnel;
    fresnel.incident = incident;
    fresnel.normal = normal;
    fresnel.n_from = n_from;
    fresnel.n_to = n_to;

    fresnel._cosI = -1.0*mliVec_dot(normal, incident);
    fresnel._n_from_over_n_to = n_from/n_to;
    fresnel._sinT2 = (fresnel._n_from_over_n_to*fresnel._n_from_over_n_to)*
        (1.0 - (fresnel._cosI*fresnel._cosI));
    fresnel._cosT = sqrt(1.0 - fresnel._sinT2);
    return fresnel;}

double mliFresnel_reflection_propability(const mliFresnel fresnel) {
    if (fresnel._sinT2 > 1.0) {
        /* total internal reflection */
        return 1.0;
    } else {
        const mliFresnel f = fresnel;
        const double nFromCosI = f.n_from*f._cosI;
        const double nFromCosT = f.n_from*f._cosT;
        const double nToCosI = f.n_to*f._cosI;
        const double nToCosT = f.n_to*f._cosT;
        const double rOrth = (nFromCosI - nToCosT)/(nFromCosI + nToCosT);
        const double rPara = (nToCosI - nFromCosT)/(nToCosI + nFromCosT);
        return (rOrth*rOrth + rPara*rPara) / 2.0;}}

mliVec mliFresnel_reflection_direction(const mliFresnel fresnel) {
    return mliVec_add(
        fresnel.incident,
        mliVec_multiply(
            fresnel.normal,
            fresnel._cosI*2.0));}

mliVec mliFresnel_refraction_direction(const mliFresnel fresnel) {
    return mliVec_add(
        mliVec_multiply(
            fresnel.incident,
            fresnel._n_from_over_n_to),
        mliVec_multiply(
            fresnel.normal,
            fresnel._n_from_over_n_to*fresnel._cosI - fresnel._cosT));}

#endif

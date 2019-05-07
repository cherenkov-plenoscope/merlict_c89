/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIMATH_H_
#define MERLICT_MLIMATH_H_

#include <math.h>
#include <stdint.h>

#define mli_PI 3.14159265358979323846

double mli_rad2deg(const double angle_in_rad) {
    return 180.*angle_in_rad/mli_PI;}

double mli_deg2rad(const double angle_in_deg) {
    return angle_in_deg*(1./180.)*mli_PI;}

int mli_min_int(const int x, const int y) {
    return (x < y) ? x : y;
}

double hypot(const double a, const double b) {
    return sqrt(a*a + b*b);}

double z_sphere(double x, double curvature_radius) {
    double half_curvature_radius = .5*curvature_radius;
    return half_curvature_radius - sqrt(
        half_curvature_radius*half_curvature_radius - x*x);}


void mli_uint32_ncpy(
    const uint32_t *from,
    uint32_t* to,
    const uint64_t num) {
    uint64_t i;
    for (i = 0; i < num; i++) {
        to[i] = from[i];}}

#define MLI_MIN2(a,b) (((a) < (b)) ? (a) : (b))
#define MLI_MAX2(a,b) (((a) > (b)) ? (a) : (b))

#define MLI_MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MLI_MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))

#endif

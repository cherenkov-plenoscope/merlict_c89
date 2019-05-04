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

#define MLI_MIN2(a,b) (((a) < (b)) ? (a) : (b))
#define MLI_MAX2(a,b) (((a) > (b)) ? (a) : (b))

#define MLI_MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MLI_MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))

#endif

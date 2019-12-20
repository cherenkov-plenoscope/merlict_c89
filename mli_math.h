/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MATH_H_
#define MERLICT_MATH_H_

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define MLI_PI 3.14159265358979323846

#define MLI_2_OVER_SQRT3 1.1547005383792517
#define MLI_SQRT3_OVER_2 0.8660254037844386

#define MLI_EPSILON 1e-4

double mli_rad2deg(const double angle_in_rad) {
    return 180.*angle_in_rad/MLI_PI;}

double mli_deg2rad(const double angle_in_deg) {
    return angle_in_deg*(1./180.)*MLI_PI;}

double mli_hypot(const double a, const double b) {
    return sqrt(a*a + b*b);}

double mli_z_sphere(double x, double curvature_radius) {
    double half_curvature_radius = .5*curvature_radius;
    return half_curvature_radius - sqrt(
        half_curvature_radius*half_curvature_radius - x*x);}

double mli_squared(const double a) {
    return a*a;}

void mli_uint32_ncpy(const uint32_t *src, uint32_t* dst, const uint64_t num) {
    uint64_t i; for (i = 0; i < num; i++) {dst[i] = src[i];}}

#define MLI_IS_NAN(a) ((a) != (a))

#define MLI_MIN2(a,b) (((a) < (b)) ? (a) : (b))
#define MLI_MAX2(a,b) (((a) > (b)) ? (a) : (b))

#define MLI_MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MLI_MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))

#define MLI_ZEROS(points, num_points) \
    do { \
        uint64_t i; \
        for (i = 0; i < num_points; i++) { \
            points[i] = 0; \
        } \
    } while (0)

void mli_zeros_double(double *points, const uint64_t num_points) {
    MLI_ZEROS(points, num_points);}

void mli_zeros_uint64_t(uint64_t *points, const uint64_t num_points) {
    MLI_ZEROS(points, num_points);}

#define MLI_UPPER_COMPARE(points, num_points, point_arg) \
    /*
    parameters
    ----------
        points          Sorted array in ascending order.
        num_points      Number floats in points. Its length.
        point_arg       The point to find the upper-bound for.
    */ \
    do { \
        uint64_t first, last, middle; \
        first = 0u; \
        last = num_points - 1u; \
        middle = (last - first)/2; \
        if (num_points == 0) { \
            return 0; \
        } \
        if (point_arg >= points[num_points - 1u]) { \
            return num_points; \
        } \
        while (first < last) { \
            if (points[middle] > point_arg) { \
                last = middle; \
            } else { \
                first = middle + 1u; \
            } \
            middle = first + (last - first)/2; \
        } \
        return last; \
    } while (0)

uint64_t mli_upper_compare_double(
    const double *points,
    const uint64_t num_points,
    const double point_arg) {
    MLI_UPPER_COMPARE(points, num_points, point_arg);}

void mli_histogram(
    const double *bin_edges,
    const uint64_t num_bin_edges,
    uint64_t *underflow_bin,
    uint64_t *bins,
    uint64_t *overflow_bin,
    const double point) {
    uint64_t idx_upper = mli_upper_compare_double(
        bin_edges,
        num_bin_edges,
        point);
    if (idx_upper == 0) {
        (*underflow_bin) += 1u;
    } else if (idx_upper == num_bin_edges) {
        (*overflow_bin) += 1u;
    } else {
        bins[idx_upper - 1] += 1u;
    }
}

void mli_linspace(
    const double start,
    const double stop,
    double *points,
    const uint64_t num_points) {
    uint64_t i;
    const double range = stop - start;
    const double step = range/(double)(num_points - 1u);
    for (i = 0; i < num_points; i++) {
        points[i] = (double)i * step;
    }
}


#endif

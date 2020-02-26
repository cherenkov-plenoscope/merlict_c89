/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MATH_H_
#define MERLICT_MATH_H_

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define MLI_PI 3.14159265358979323846
#define MLI_2PI 6.28318530717958623199

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

#define MLI_NAN 0./0.
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

/*
 *  parameters
 *  ----------
 *      points          Sorted array in ascending order.
 *      num_points      Number of points.
 *      point_arg       The point to find the upper-bound for.
 */
#define MLI_UPPER_COMPARE(points, num_points, point_arg, return_idx) \
    do { \
        uint64_t first, last, middle; \
        first = 0u; \
        last = num_points - 1u; \
        middle = (last - first)/2; \
        if (num_points == 0) { \
            return_idx = 0; \
        } else { \
            if (point_arg >= points[num_points - 1u]) { \
                return_idx = num_points; \
            } else { \
                while (first < last) { \
                    if (points[middle] > point_arg) { \
                        last = middle; \
                    } else { \
                        first = middle + 1u; \
                    } \
                    middle = first + (last - first)/2; \
                } \
                return_idx = last; \
            } \
        }\
    } while (0)

uint64_t mli_upper_compare_double(
    const double *points,
    const uint64_t num_points,
    const double point_arg) {
    uint64_t upper_index = 0;
    MLI_UPPER_COMPARE(points, num_points, point_arg, upper_index);
    return upper_index;
}

void mli_histogram(
    const double *bin_edges,
    const uint64_t num_bin_edges,
    uint64_t *underflow_bin,
    uint64_t *bins,
    uint64_t *overflow_bin,
    const double point)
{
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

double mli_mean(const double vals[], const uint64_t size)
{
    uint64_t i;
    double sum = 0;
    for (i = 0; i < size; i++) {
        sum = sum + vals[i];
    }
    return sum/(double)size;
}

double mli_std(const double vals[], const uint64_t size, const double vals_mean)
{
    uint64_t i;
    double s = 0.;
    for (i = 0; i < size; i++) {
        s = s + (vals[i] - vals_mean)*(vals[i] - vals_mean);
    }
    return sqrt(s/(double)size);
}

#define mli_roundf(num) (num - floor(num) > 0.5) ? ceil(num) : floor(num)

#define mli_near_int(x) ((x)>0?(int64_t)((x)+0.5):(int64_t)((x)-0.5))

#endif

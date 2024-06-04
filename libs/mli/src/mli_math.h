/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATH_H_
#define MLI_MATH_H_

#include <math.h>
#include <stdint.h>

#define MLI_PI 3.14159265358979323846
#define MLI_2PI 6.28318530717958623199
#define MLI_2_OVER_SQRT3 1.1547005383792517
#define MLI_SQRT3_OVER_2 0.8660254037844386
#define MLI_EPSILON 1e-9
#define MLI_NAN 0. / 0.
#define MLI_IS_NAN(a) ((a) != (a))
#define MLI_MIN2(a, b) (((a) < (b)) ? (a) : (b))
#define MLI_MAX2(a, b) (((a) > (b)) ? (a) : (b))
#define MLI_ROUND(num) (num - floor(num) > 0.5) ? ceil(num) : floor(num)
#define MLI_NEAR_INT(x) ((x) > 0 ? (int64_t)((x) + 0.5) : (int64_t)((x)-0.5))
#define MLI_SIGN(x) ((x) == 0 ? 0 : ((x) > 0 ? 1 : -1))

#define MLI_MIN3(a, b, c)                                                      \
        ((((a) < (b)) && ((a) < (c))) ? (a) : (((b) < (c)) ? (b) : (c)))

#define MLI_MAX3(a, b, c)                                                      \
        ((((a) > (b)) && ((a) > (c))) ? (a) : (((b) > (c)) ? (b) : (c)))

#define MLI_ARRAY_SET(arr, val, num)                                           \
        do {                                                                   \
                uint64_t i;                                                    \
                for (i = 0; i < num; i++) {                                    \
                        arr[i] = val;                                          \
                }                                                              \
        } while (0)

#define MLI_UPPER_COMPARE(points, num_points, point_arg, return_idx)           \
        do {                                                                   \
                uint64_t first, last, middle;                                  \
                first = 0u;                                                    \
                last = num_points - 1u;                                        \
                middle = (last - first) / 2;                                   \
                if (num_points == 0) {                                         \
                        return_idx = 0;                                        \
                } else {                                                       \
                        if (point_arg >= points[num_points - 1u]) {            \
                                return_idx = num_points;                       \
                        } else {                                               \
                                while (first < last) {                         \
                                        if (points[middle] > point_arg) {      \
                                                last = middle;                 \
                                        } else {                               \
                                                first = middle + 1u;           \
                                        }                                      \
                                        middle = first + (last - first) / 2;   \
                                }                                              \
                                return_idx = last;                             \
                        }                                                      \
                }                                                              \
        } while (0)

#define MLI_NCPY(src, dst, num)                                                \
        do {                                                                   \
                uint64_t i;                                                    \
                for (i = 0; i < num; i++) {                                    \
                        dst[i] = src[i];                                       \
                }                                                              \
        } while (0)

double mli_std(
        const double vals[],
        const uint64_t size,
        const double vals_mean);
double mli_mean(const double vals[], const uint64_t size);
void mli_linspace(
        const double start,
        const double stop,
        double *points,
        const uint64_t num_points);
void mli_histogram(
        const double *bin_edges,
        const uint64_t num_bin_edges,
        uint64_t *underflow_bin,
        uint64_t *bins,
        uint64_t *overflow_bin,
        const double point);
uint64_t mli_upper_compare_double(
        const double *points,
        const uint64_t num_points,
        const double point_arg);
double mli_square(const double a);
double mli_hypot(const double a, const double b);
double mli_deg2rad(const double angle_in_deg);
double mli_rad2deg(const double angle_in_rad);
double mli_bin_center_in_linear_space(
        const double start,
        const double stop,
        const uint64_t num_bins,
        const uint64_t bin);
double mli_linear_interpolate_1d(
        const double weight,
        const double start,
        const double end);
double mli_linear_interpolate_2d(
        const double xarg,
        const double x0,
        const double y0,
        const double x1,
        const double y1);
double mli_relative_ratio(const double a, const double b);

double mli_interpret_int64_as_double(int64_t i);
int64_t mli_interpret_double_as_int64(double d);
#endif

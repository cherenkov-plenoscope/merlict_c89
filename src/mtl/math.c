/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "math.h"
#include <string.h>

double mtl_math_rad2deg(const double angle_in_rad)
{
        return 180. * angle_in_rad / MTL_MATH_PI;
}

double mtl_math_deg2rad(const double angle_in_deg)
{
        return angle_in_deg * (1. / 180.) * MTL_MATH_PI;
}

double mtl_math_hypot(const double a, const double b)
{
        return sqrt(a * a + b * b);
}

double mtl_math_square(const double a) { return a * a; }

/*
 *  parameters
 *  ----------
 *      points          Sorted array in ascending order.
 *      num_points      Number of points.
 *      point_arg       The point to find the upper-bound for.
 */
uint64_t MTL_MATH_UPPER_COMPARE_double(
        const double *points,
        const uint64_t num_points,
        const double point_arg)
{
        uint64_t upper_index = 0;
        MTL_MATH_UPPER_COMPARE(points, num_points, point_arg, upper_index);
        return upper_index;
}

void mtl_math_histogram(
        const double *bin_edges,
        const uint64_t num_bin_edges,
        uint64_t *underflow_bin,
        uint64_t *bins,
        uint64_t *overflow_bin,
        const double point)
{
        uint64_t idx_upper =
                MTL_MATH_UPPER_COMPARE_double(bin_edges, num_bin_edges, point);
        if (idx_upper == 0) {
                (*underflow_bin) += 1u;
        } else if (idx_upper == num_bin_edges) {
                (*overflow_bin) += 1u;
        } else {
                bins[idx_upper - 1] += 1u;
        }
}

void mtl_math_linspace(
        const double start,
        const double stop,
        double *points,
        const uint64_t num_points)
{
        uint64_t i;
        const double range = stop - start;
        const double step = range / (double)(num_points - 1u);
        for (i = 0; i < num_points; i++) {
                points[i] = (double)i * step + start;
        }
}

double mtl_math_mean(const double vals[], const uint64_t size)
{
        uint64_t i;
        double sum = 0;
        for (i = 0; i < size; i++) {
                sum = sum + vals[i];
        }
        return sum / (double)size;
}

double mtl_math_std(
        const double vals[],
        const uint64_t size,
        const double vals_mean)
{
        uint64_t i;
        double s = 0.;
        for (i = 0; i < size; i++) {
                s = s + (vals[i] - vals_mean) * (vals[i] - vals_mean);
        }
        return sqrt(s / (double)size);
}

double mtl_math_bin_center_in_linear_space(
        const double start,
        const double stop,
        const uint64_t num_bins,
        const uint64_t bin)
{
        const double width = stop - start;
        const double bin_width = width / (double)num_bins;
        return start + bin * bin_width + 0.5 * bin_width;
}

double mtl_math_linear_interpolate_1d(
        const double weight,
        const double start,
        const double end)
{
        return start + weight * (end - start);
}

double mtl_math_linear_interpolate_2d(
        const double xarg,
        const double x0,
        const double y0,
        const double x1,
        const double y1)
{
        /*
         *      |
         *  y1 -|            o
         *      |
         *  y0 -|    o
         *      |       xarg
         *      +----|---|---|----
         *          x0       x1
         *
         *  f(x) = m*x + b
         *  m = (y1 - y0)/(x1 - x0)
         *  y0 = m*x0 + b
         *  b = y0 - m*x0
         */
        const double m = (y1 - y0) / (x1 - x0);
        const double b = y0 - m * x0;
        return m * xarg + b;
}

double mtl_math_relative_ratio(const double a, const double b)
{
        return fabs(a - b) / (0.5 * (a + b));
}

double mtl_math_interpret_int64_as_double(int64_t i)
{
        double f;
        memcpy(&f, &i, sizeof(double));
        return f;
}

int64_t mtl_math_interpret_double_as_int64(double d)
{
        int64_t i;
        memcpy(&i, &d, sizeof(int64_t));
        return i;
}

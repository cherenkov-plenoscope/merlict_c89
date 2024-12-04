/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliRay.h"
#include "../../mtl/src/math_quadratic_equation.h"

struct mliRay mliRay_set(
        const struct mliVec support,
        const struct mliVec direction)
{
        struct mliRay ray;
        ray.support = support;
        ray.direction = mliVec_multiply(direction, 1. / mliVec_norm(direction));
        return ray;
}

struct mliVec mliRay_at(const struct mliRay *ray, const double t)
{
        struct mliVec out;
        out.x = ray->support.x + t * ray->direction.x;
        out.y = ray->support.y + t * ray->direction.y;
        out.z = ray->support.z + t * ray->direction.z;
        return out;
}

int mliRay_sphere_intersection(
        const struct mliVec support,
        const struct mliVec direction,
        const double radius,
        double *minus_solution,
        double *plus_solution)
{
        const double sup_times_dir = mliVec_dot(support, direction);
        const double dir_times_dir = mliVec_dot(direction, direction);
        const double sup_times_sup = mliVec_dot(support, support);
        const double radius_square = radius * radius;

        const double p = 2.0 * (sup_times_dir / dir_times_dir);
        const double q = sup_times_sup / dir_times_dir - radius_square;

        return mtl_math_quadratic_equation(p, q, minus_solution, plus_solution);
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_spherical_cap.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mli_spherical_cap_equation(
        const struct mliRay ray,
        const double radius,
        double *plus_solution,
        double *minus_solution)
{
        /*
         *  sphere intersection with ray
         *  z(x,y) = R - sqrt(R^2 -x^2 -y^2) |- R, *(-1)
         *  R - z  = sqrt(R^2 -x^2 -y^2)     |pow2
         *  (R-z)^2= R^2 - x^2 - y^2         |+x^2 +y^2
         *  R^2 -2Rz +z^2 +y^2 +x^2 = R^2    |-R^2
         *  x^2 + y^2 + z^2 -2Rz = 0         | x=sx+v*dx, y=sy+v*dy, z=sz+v*dz
         *  v^2*(d*d) + v*2(s*d -R*dz) +s*s -2*R*sz = 0
         *  v^2 + v 2(s*d - R*dz)/(d*d) + (s*s - 2*R*sz)/(d*d) = 0
         */
        const struct mliVec sup = ray.support;
        const struct mliVec dir = ray.direction;
        const double dir_times_dir = mliVec_dot(dir, dir);
        const double sup_times_sup = mliVec_dot(sup, sup);
        const double sup_times_dir = mliVec_dot(sup, dir);
        const double R_times_dir_z = radius * dir.z;
        const double R_times_sup_z = radius * sup.z;
        const double P = 2. * (sup_times_dir - R_times_dir_z) / dir_times_dir;
        const double Q = (sup_times_sup - 2. * R_times_sup_z) / dir_times_dir;
        return mli_quadratiq_equation(P, Q, plus_solution, minus_solution);
}

struct mliVec mli_spherical_cap_surface_normal(
        const struct mliVec intersection_point,
        const double radius)
{
        struct mliVec radial = mliVec_set(0., 0., radius);
        struct mliVec normal = mliVec_substract(radial, intersection_point);
        normal = mliVec_multiply(normal, 1. / mliVec_norm(normal));
        return normal;
}

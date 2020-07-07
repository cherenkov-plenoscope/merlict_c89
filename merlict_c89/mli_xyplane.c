/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_xyplane.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mli_xyplane_equation(const struct mliRay ray, double *solution)
{
        /*
         *  calculate intersection of  ray: g := sup+v*dir
         *                       and plane: P := a*ex + b*ey
         *  g = P
         *  (a,0,0)^T + (0,b,0)^T = (sup_x, sup_y, sup_z)^T +
         *                          v*(dir_x, dir_y, dir_z)
         *  (a,b,0)^T             = (sup_x + v*dir_x,
         *                           sup_y + v*dir_y,
         *                           sup_z + v*dir_z)^T
         *  z-component only
         *  0 = sup_z + v*dir_z  <=>
         *  v = -sup_z/dir_z
         */
        if (ray.direction.z != 0.) {
                (*solution) = -ray.support.z / ray.direction.z;
                return 1;
        } else {
                return 0;
        }
}

struct mliVec mli_xyplane_surface_normal(void)
{
        return mliVec_set(0., 0., 1.);
}

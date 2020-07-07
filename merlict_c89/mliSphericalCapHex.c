/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSphericalCapHex.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliSphericalCapHex_is_equal(
        const struct mliSphericalCapHex a,
        const struct mliSphericalCapHex b)
{
        if (a.curvature_radius != b.curvature_radius)
                return 0;
        if (a.inner_hex_radius != b.inner_hex_radius)
                return 0;
        return 1;
}

double mliSphericalCapHex_bounding_radius(const struct mliSphericalCapHex cap)
{
        const double outer_hex_radius = cap.inner_hex_radius * MLI_2_OVER_SQRT3;
        return mli_hypot(
                outer_hex_radius,
                mli_z_sphere(
                        cap.inner_hex_radius * MLI_2_OVER_SQRT3,
                        cap.curvature_radius));
}

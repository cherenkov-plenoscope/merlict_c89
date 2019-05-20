/* Copyright 2019 Sebastian A. Mueller */
#ifndef MERLICT_MLI_DUAL_SPHERICAL_PRISM_H_
#define MERLICT_MLI_DUAL_SPHERICAL_PRISM_H_

#include <math.h>

int mli_is_inside_dual_circle_prism(
    const double x,
    const double y,
    const double height,
    const double width) {
    /*
     *                  ______|_______                      ___
     *            _____/      |       \_____                 |
     *         __/            |             \__              |
     *       _/               |                 \_           |
     * -----|_----------------|------------------_|-- y    height
     *        \__             |              ___/            |
     *           \_____       |        _____/                |
     *                 \______|_______/                     _|_
     *                        |x
     *      |_______________________width_________|
     */
    const double circle_distance = (width*width - height*height)/(2.*height);
    const double circle_radius = circle_distance/2. + height/2.;

    const double circ1_x = +circle_distance/2.;
    const double circ1_y = 0.;

    const double circ2_x = -circle_distance/2.;
    const double circ2_y = 0.;

    const double v_to_c1_x = circ1_x - x;
    const double v_to_c1_y = circ1_y - y;

    const double v_to_c2_x = circ2_x - x;
    const double v_to_c2_y = circ2_y - y;

    const double v_to_c1_norm_sq = v_to_c1_x*v_to_c1_x + v_to_c1_y*v_to_c1_y;
    const double v_to_c2_norm_sq = v_to_c2_x*v_to_c2_x + v_to_c2_y*v_to_c2_y;

    const double circle_radius_sq = circle_radius*circle_radius;

    return v_to_c1_norm_sq <= circle_radius_sq &&
           v_to_c2_norm_sq <= circle_radius_sq;
}

#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIBICIRCLEPLANE_H_
#define MERLICT_C89_MLIBICIRCLEPLANE_H_

struct mliBiCirclePlane {
        double x_height;
        double y_width;
};

int mliBiCirclePlane_is_valid(const struct mliBiCirclePlane a);
int mliBiCirclePlane_is_equal(
        const struct mliBiCirclePlane a,
        const struct mliBiCirclePlane b);
#endif

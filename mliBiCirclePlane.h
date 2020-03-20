/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIBICIRCLEPLANE_H_
#define MERLICT_MLIBICIRCLEPLANE_H_

struct mliBiCirclePlane {
        double x_height;
        double y_width;
};

int mliBiCirclePlane_is_equal(
        const struct mliBiCirclePlane a,
        const struct mliBiCirclePlane b)
{
        if (a.x_height != b.x_height) {
                return 0;
        }
        if (a.y_width != b.y_width) {
                return 0;
        }
        return 1;
}

int mliBiCirclePlane_is_valid(const struct mliBiCirclePlane a)
{
        if (a.x_height <= 0.) {
                return 0;
        }
        if (a.y_width <= 0.) {
                return 0;
        }
        return 1;
}

#endif

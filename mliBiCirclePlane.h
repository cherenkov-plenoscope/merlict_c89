/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIBICIRCLEPLANE_H_
#define MERLICT_MLIBICIRCLEPLANE_H_


typedef struct {
    double x_height;
    double y_width;
} mliBiCirclePlane;

int mliBiCirclePlane_is_equal(
    const mliBiCirclePlane a,
    const mliBiCirclePlane b) {
    if (a.x_height != b.x_height) {
        return 0;
    }
    if (a.y_width != b.y_width) {
        return 0;
    }
    return 1;}

#endif

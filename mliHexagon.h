/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHEXAGON_H_
#define MERLICT_MLIHEXAGON_H_


typedef struct {
    double inner_radius;
} mliHexagon;

int mliHexagon_is_equal(const mliHexagon a, const mliHexagon b) {
    return a.inner_radius == b.inner_radius;}

#endif

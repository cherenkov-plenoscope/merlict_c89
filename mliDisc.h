/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_DISC_H_
#define MERLICT_DISC_H_


typedef struct {
    double radius;
} mliDisc;

int mliDisc_is_equal(const mliDisc a, const mliDisc b) {
    return a.radius == b.radius;}

#endif

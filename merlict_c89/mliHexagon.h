/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIHEXAGON_H_
#define MERLICT_C89_MLIHEXAGON_H_

struct mliHexagon {
        double inner_radius;
};

int mliHexagon_is_equal(const struct mliHexagon a, const struct mliHexagon b);
#endif

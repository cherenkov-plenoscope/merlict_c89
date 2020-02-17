/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHEXAGON_H_
#define MERLICT_MLIHEXAGON_H_


struct mliHexagon {
    double inner_radius;
};

int mliHexagon_is_equal(const struct mliHexagon a, const struct mliHexagon b)
{
    return a.inner_radius == b.inner_radius;
}

#endif

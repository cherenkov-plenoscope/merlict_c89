/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISPHERE_H_
#define MERLICT_C89_MLISPHERE_H_

struct mliSphere {
        double radius;
};

int mliSphere_is_equal(const struct mliSphere a, const struct mliSphere b);
#endif

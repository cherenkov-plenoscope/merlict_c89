/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDISC_H_
#define MERLICT_C89_MLIDISC_H_

struct mliDisc {
        double radius;
};

int mliDisc_is_valid(const struct mliDisc a);
int mliDisc_is_equal(const struct mliDisc a, const struct mliDisc b);
#endif

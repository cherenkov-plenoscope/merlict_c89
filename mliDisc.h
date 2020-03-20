/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_DISC_H_
#define MERLICT_DISC_H_

struct mliDisc {
        double radius;
};

int mliDisc_is_equal(const struct mliDisc a, const struct mliDisc b)
{
        return a.radius == b.radius;
}

int mliDisc_is_valid(const struct mliDisc a) { return a.radius >= 0.; }

#endif

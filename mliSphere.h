/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_SPHERE_H_
#define MERLICT_SPHERE_H_

struct mliSphere {
        double radius;
};

int mliSphere_is_equal(const struct mliSphere a, const struct mliSphere b)
{
        return a.radius == b.radius;
}

#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIVEC_H_
#define MLIVEC_H_

#include <stdint.h>

struct mliVec {
        double x;
        double y;
        double z;
};

void mliVec_print(const struct mliVec v);
uint32_t mliVec_octant(const struct mliVec a);
int mliVec_equal(const struct mliVec a, const struct mliVec b);
int mliVec_equal_margin(
        const struct mliVec a,
        const struct mliVec b,
        const double distance_margin);
struct mliVec mliVec_mirror(const struct mliVec in, const struct mliVec normal);
double mliVec_norm_between(const struct mliVec a, const struct mliVec b);
double mliVec_angle_between(const struct mliVec a, const struct mliVec b);
struct mliVec mliVec_normalized(struct mliVec a);
double mliVec_norm(const struct mliVec a);
struct mliVec mliVec_multiply(const struct mliVec v, const double a);
double mliVec_dot(const struct mliVec a, const struct mliVec b);
struct mliVec mliVec_cross(const struct mliVec a, const struct mliVec b);
struct mliVec mliVec_substract(const struct mliVec a, const struct mliVec b);
struct mliVec mliVec_add(const struct mliVec a, const struct mliVec b);
struct mliVec mliVec_init(const double x, const double y, const double z);
int mliVec_sign3_bitmask(const struct mliVec a, const double epsilon);
struct mliVec mliVec_mean(const struct mliVec *vecs, const uint64_t num_vecs);
void mliVec_set(struct mliVec *a, const uint64_t dim, const double val);
double mliVec_get(const struct mliVec *a, const uint64_t dim);
#endif

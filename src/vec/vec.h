/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VEC_H_
#define MLI_VEC_H_

#include <stdint.h>
#include "../bool/bool.h"

struct mli_Vec {
        double x;
        double y;
        double z;
};

void mli_Vec_print(const struct mli_Vec v);
uint32_t mli_Vec_octant(const struct mli_Vec a);
mli_bool mli_Vec_equal(const struct mli_Vec a, const struct mli_Vec b);
mli_bool mli_Vec_equal_margin(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const double distance_margin);
struct mli_Vec mli_Vec_mirror(
        const struct mli_Vec in,
        const struct mli_Vec normal);
double mli_Vec_norm_between(const struct mli_Vec a, const struct mli_Vec b);
double mli_Vec_angle_between(const struct mli_Vec a, const struct mli_Vec b);
struct mli_Vec mli_Vec_normalized(struct mli_Vec a);
double mli_Vec_norm(const struct mli_Vec a);
struct mli_Vec mli_Vec_multiply(const struct mli_Vec v, const double a);
double mli_Vec_dot(const struct mli_Vec a, const struct mli_Vec b);
struct mli_Vec mli_Vec_cross(const struct mli_Vec a, const struct mli_Vec b);
struct mli_Vec mli_Vec_substract(
        const struct mli_Vec a,
        const struct mli_Vec b);
struct mli_Vec mli_Vec_add(const struct mli_Vec a, const struct mli_Vec b);
struct mli_Vec mli_Vec_init(const double x, const double y, const double z);
int64_t mli_Vec_sign3_bitmask(const struct mli_Vec a, const double epsilon);
struct mli_Vec mli_Vec_mean(
        const struct mli_Vec *vecs,
        const uint64_t num_vecs);
void mli_Vec_set(struct mli_Vec *a, const uint64_t dim, const double val);
double mli_Vec_get(const struct mli_Vec *a, const uint64_t dim);
#endif

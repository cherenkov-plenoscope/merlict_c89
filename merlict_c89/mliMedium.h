/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMEDIUM_H_
#define MERLICT_C89_MLIMEDIUM_H_

#include <stdint.h>

struct mliMedium {
        struct mliFunc refraction;
        struct mliFunc absorbtion;
};
struct mliMedium mliMedium_init(void);
int mliMedium_malloc(
        struct mliMedium *medium,
        const uint32_t num_points_refraction,
        const uint32_t num_points_absorbtion);
void mliMedium_free(struct mliMedium *medium);

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b);

int mliMedium_fwrite(const struct mliMedium *med, FILE *f);
int mliMedium_malloc_fread(struct mliMedium *med, FILE *f);

#endif

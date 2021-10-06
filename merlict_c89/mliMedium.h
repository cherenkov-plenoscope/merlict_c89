/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMEDIUM_H_
#define MERLICT_C89_MLIMEDIUM_H_

#include <stdint.h>
#include "mliJson.h"

struct mliMedium {
        struct mliFunc refraction;
        struct mliFunc absorbtion;
};
struct mliMedium mliMedium_init(void);
void mliMedium_free(struct mliMedium *medium);

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b);

int mliMedium_fwrite(const struct mliMedium *med, FILE *f);
int mliMedium_malloc_fread(struct mliMedium *med, FILE *f);

int mliMedium_malloc_from_json_str(struct mliMedium *med, const char *json_str);
int mliMedium_malloc_from_json_token(
        struct mliMedium *med,
        const struct mliJson *json,
        const uint64_t token);
#endif

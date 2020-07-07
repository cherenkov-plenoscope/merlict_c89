/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFACE_H_
#define MERLICT_C89_MLIFACE_H_

#include <stdint.h>

struct mliFace {
        uint32_t a;
        uint32_t b;
        uint32_t c;
};

void mliFace_ncpy(
        const struct mliFace *from,
        struct mliFace *to,
        const uint64_t num);
int mliFace_is_equal(const struct mliFace a, const struct mliFace b);
struct mliFace mliFace_set(
        const uint32_t a,
        const uint32_t b,
        const uint32_t c);
#endif

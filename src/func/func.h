/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FUNC_H_
#define MLI_FUNC_H_

#include <stdint.h>
#include <stdio.h>
#include "../bool/bool.h"
#include "../chk/chk.h"

struct mli_Func {
        uint64_t num_points;
        double *x;
        double *y;
};

mli_bool mli_Func_equal(const struct mli_Func a, const struct mli_Func b);

chk_rc mli_Func_evaluate(
        const struct mli_Func *f,
        const double xarg,
        double *out);
mli_bool mli_Func_in_range(const struct mli_Func *f, const double xarg);
mli_bool mli_Func_x_is_strictly_increasing(const struct mli_Func *f);
chk_rc mli_Func_malloc(struct mli_Func *f, const uint64_t num_points);
void mli_Func_free(struct mli_Func *f);
struct mli_Func mli_Func_init(void);
mli_bool mli_Func_is_valid(const struct mli_Func *func);

mli_bool mli_Func_malloc_constant(
        struct mli_Func *self,
        const double start,
        const double stop,
        const double value);
#endif

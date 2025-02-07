/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "func.h"
#include <stdlib.h>
#include "../magicid/magicid.h"
#include "../chk/chk.h"
#include "../math/math.h"
#include "../bool/bool.h"

struct mli_Func mli_Func_init(void)
{
        struct mli_Func f;
        f.num_points = 0u;
        f.x = NULL;
        f.y = NULL;
        return f;
}

void mli_Func_free(struct mli_Func *f)
{
        free(f->x);
        free(f->y);
        (*f) = mli_Func_init();
}

chk_rc mli_Func_malloc(struct mli_Func *f, const uint64_t num_points)
{
        mli_Func_free(f);
        f->num_points = num_points;
        chk_malloc(f->x, double, f->num_points);
        chk_malloc(f->y, double, f->num_points);
        return CHK_SUCCESS;
chk_error:
        mli_Func_free(f);
        return CHK_FAIL;
}

mli_bool mli_Func_x_is_strictly_increasing(const struct mli_Func *f)
{
        uint64_t i;
        for (i = 1; i < f->num_points; i++) {
                if (f->x[i] <= f->x[i - 1]) {
                        return MLI_FALSE;
                }
        }
        return MLI_TRUE;
}

chk_rc mli_Func_evaluate(
        const struct mli_Func *f,
        const double xarg,
        double *out)
{
        double y1, y0, x1, x0;
        uint64_t idx = mli_math_upper_compare_double(f->x, f->num_points, xarg);
        if (idx == 0) {
                chk_bad("mli_Func argument below lower bound.");
        } else if (idx == f->num_points) {
                chk_bad("mli_Func argument above upper bound.");
        } else {
                y1 = f->y[idx];
                y0 = f->y[idx - 1u];
                x1 = f->x[idx];
                x0 = f->x[idx - 1u];
                (*out) = mli_math_linear_interpolate_2d(xarg, x0, y0, x1, y1);
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_Func_in_range(const struct mli_Func *f, const double xarg)
{
        if (f->num_points < 2) {
                return MLI_FALSE;
        }
        if (xarg >= f->x[0]) {
                if (xarg < f->x[f->num_points - 1]) {
                        return MLI_TRUE;
                }
        }
        return MLI_FALSE;
}

mli_bool mli_Func_equal(const struct mli_Func a, const struct mli_Func b)
{
        uint64_t i;
        if (a.num_points != b.num_points)
                return MLI_FALSE;
        for (i = 0; i < a.num_points; i++) {
                if (a.x[i] != b.x[i])
                        return MLI_FALSE;
                if (a.y[i] != b.y[i])
                        return MLI_FALSE;
        }
        return MLI_TRUE;
}

mli_bool mli_Func_is_valid(const struct mli_Func *func)
{
        uint64_t i;
        chk_msg(func->num_points >= 2,
                "Expected function to have at least two points. "
                "Evaluation is not possible when there is no valid range "
                "between two points.");

        for (i = 0; i < func->num_points; i++) {
                chk_msg(!MLI_MATH_IS_NAN(func->x[i]),
                        "Expected x-argument to be a real number, "
                        "but it is 'nan'.");
                chk_msg(!MLI_MATH_IS_NAN(func->y[i]),
                        "Expected y-value to be a real number, "
                        "but it is 'nan'.");
        }

        chk_msg(mli_Func_x_is_strictly_increasing(func),
                "Expected x-arguments to be strictly increasing, "
                "but they do not.");

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_Func_malloc_constant(
        struct mli_Func *self,
        const double start,
        const double stop,
        const double value)
{
        chk(mli_Func_malloc(self, 2));
        self->x[0] = start;
        self->y[0] = value;
        self->x[1] = stop;
        self->y[1] = value;

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

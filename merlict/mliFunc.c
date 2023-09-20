/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFunc.h"
#include <stdlib.h>
#include "mliMagicId.h"
#include "chk_debug.h"

struct mliFunc mliFunc_init(void)
{
        struct mliFunc f;
        f.num_points = 0u;
        f.x = NULL;
        f.y = NULL;
        return f;
}

void mliFunc_free(struct mliFunc *f)
{
        free(f->x);
        free(f->y);
        (*f) = mliFunc_init();
}

int mliFunc_malloc(struct mliFunc *f, const uint32_t num_points)
{
        mliFunc_free(f);
        f->num_points = num_points;
        chk_malloc(f->x, double, f->num_points);
        chk_malloc(f->y, double, f->num_points);
        return 1;
error:
        mliFunc_free(f);
        return 0;
}

int mliFunc_x_is_strictly_increasing(const struct mliFunc *f)
{
        uint32_t i;
        for (i = 1; i < f->num_points; i++) {
                if (f->x[i] <= f->x[i - 1]) {
                        return 0;
                }
        }
        return 1;
}

int mliFunc_evaluate(const struct mliFunc *f, const double xarg, double *out)
{
        double y1, y0, x1, x0;
        uint32_t idx = mli_upper_compare_double(f->x, f->num_points, xarg);
        if (idx == 0) {
                chk_bad("mliFunc argument below lower bound.");
        } else if (idx == f->num_points) {
                chk_bad("mliFunc argument above upper bound.");
        } else {
                y1 = f->y[idx];
                y0 = f->y[idx - 1u];
                x1 = f->x[idx];
                x0 = f->x[idx - 1u];
                (*out) = mli_linear_interpolate_2d(xarg, x0, y0, x1, y1);
        }
        return 1;
error:
        return 0;
}

int mliFunc_fold_numeric(
        const struct mliFunc *a,
        const struct mliFunc *b,
        double *fold)
{
        uint64_t i;
        const uint64_t NUM_STEPS = 1024 * 8;
        const double xmin = a->x[0];
        const double xmax = a->x[a->num_points - 1];
        const double step_size = (xmax - xmin) / (double)NUM_STEPS;
        chk_msg(a->num_points >= 2u, "Expect a->num_points >= 2.");
        chk_msg(b->num_points >= 2u, "Expect b->num_points >= 2.");
        chk_msg(a->x[0] == b->x[0], "Expect a->x[0] == b->x[0].");
        chk_msg(a->x[a->num_points - 1] == b->x[b->num_points - 1],
                "Expect a->x[:-1] == b->x[:-1].");
        (*fold) = 0.0;
        for (i = 0; i < NUM_STEPS; i++) {
                double ra = MLI_NAN;
                double rb = MLI_NAN;
                double x = xmin + (double)i * step_size;
                chk(mliFunc_evaluate(a, x, &ra));
                chk(mliFunc_evaluate(b, x, &rb));
                (*fold) += (ra * rb) * step_size;
        }
        return 1;
error:
        return 0;
}

int mliFunc_equal(const struct mliFunc a, const struct mliFunc b)
{
        uint64_t i;
        if (a.num_points != b.num_points)
                return 0;
        for (i = 0; i < a.num_points; i++) {
                if (a.x[i] != b.x[i])
                        return 0;
                if (a.y[i] != b.y[i])
                        return 0;
        }
        return 1;
}

int mliFunc_is_valid(const struct mliFunc *func)
{
        uint64_t i;
        chk_msg(func->num_points >= 2,
                "Expected function to have at least two points. "
                "Evaluation is not possible when there is no valid range "
                "between two points.");

        for (i = 0; i < func->num_points; i++) {
                chk_msg(!MLI_IS_NAN(func->x[i]),
                        "Expected x-argument to be a real number, "
                        "but it is 'nan'.");
                chk_msg(!MLI_IS_NAN(func->y[i]),
                        "Expected y-value to be a real number, "
                        "but it is 'nan'.");
        }

        chk_msg(mliFunc_x_is_strictly_increasing(func),
                "Expected x-arguments to be strictly increasing, "
                "but they do not.");

        return 1;
error:
        return 0;
}

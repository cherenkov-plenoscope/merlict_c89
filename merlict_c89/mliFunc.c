/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFunc.h"

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
        mli_malloc(f->x, double, f->num_points);
        mli_malloc(f->y, double, f->num_points);
        return 1;
error:
        mliFunc_free(f);
        return 0;
}

int mliFunc_x_is_causal(struct mliFunc *f)
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
                mli_sentinel("mliFunc argument below lower bound.");
        } else if (idx == f->num_points) {
                mli_sentinel("mliFunc argument larger upper bound.");
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
        mli_check(a->num_points >= 2u, "Expect a->num_points >= 2.");
        mli_check(b->num_points >= 2u, "Expect b->num_points >= 2.");
        mli_check(a->x[0] == b->x[0], "Expect a->x[0] == b->x[0].");
        mli_check(
                a->x[a->num_points - 1] == b->x[b->num_points - 1],
                "Expect a->x[:-1] == b->x[:-1].");
        (*fold) = 0.0;
        for (i = 0; i < NUM_STEPS; i++) {
                double ra = MLI_NAN;
                double rb = MLI_NAN;
                double x = xmin + (double)i * step_size;
                mli_c(mliFunc_evaluate(a, x, &ra));
                mli_c(mliFunc_evaluate(b, x, &rb));
                (*fold) += (ra * rb) * step_size;
        }
        return 1;
error:
        return 0;
}

int mliFunc_fwrite(const struct mliFunc *func, FILE *f)
{
        mli_fwrite(&func->num_points, sizeof(uint32_t), 1u, f);
        mli_fwrite(func->x, sizeof(double), func->num_points, f);
        mli_fwrite(func->y, sizeof(double), func->num_points, f);
        return 1;
error:
        return 0;
}

int mliFunc_malloc_from_file(struct mliFunc *func, FILE *f)
{
        uint32_t num_points;
        mli_fread(&num_points, sizeof(uint32_t), 1u, f);
        mli_c(mliFunc_malloc(func, num_points));
        mli_fread(func->x, sizeof(double), func->num_points, f);
        mli_fread(func->y, sizeof(double), func->num_points, f);
        mli_check(
                mliFunc_x_is_causal(func),
                "Expected function x-arguments to be ascending, but they are "
                "not.");
        return 1;
error:
        mliFunc_free(func);
        return 0;
}

int mliFunc_is_equal(const struct mliFunc a, const struct mliFunc b)
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

int mliFunc_cpy(struct mliFunc *destination, const struct mliFunc *source)
{
        uint64_t p;
        mli_check(
                destination->num_points == source->num_points,
                "Expected source and destination mliFunc to have same "
                "num_points.");
        for (p = 0; p < destination->num_points; p++) {
                destination->x[p] = source->x[p];
                destination->y[p] = source->y[p];
        }
        return 1;
error:
        return 0;
}

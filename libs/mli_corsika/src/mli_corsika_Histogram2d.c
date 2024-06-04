/* Copyright 2017 Sebastian A. Mueller */

#include "mli_corsika_Histogram2d.h"
#include <string.h>
#include "../../mli/src/chk.h"
#include "../../mli/src/mli_math.h"

struct key {
        int32_t x;
        int32_t y;
};

union i4i4_to_i8 {
        struct key i4i4;
        int64_t i8;
};

struct mliCorsikaHistogram2d mliCorsikaHistogram2d_init(void)
{
        struct mliCorsikaHistogram2d hist;
        hist.dict = mliAvlDict_init();
        return hist;
}

void mliCorsikaHistogram2d_free(struct mliCorsikaHistogram2d *hist)
{
        mliAvlDict_free(&hist->dict);
}

int mliCorsikaHistogram2d_malloc(
        struct mliCorsikaHistogram2d *hist,
        const uint64_t capacity)
{
        return mliAvlDict_malloc(&hist->dict, capacity);
}

int mliCorsikaHistogram2d_assign(
        struct mliCorsikaHistogram2d *hist,
        const int32_t x,
        const int32_t y,
        const double weight)
{
        int has;
        union i4i4_to_i8 key;
        int64_t ival = 0;
        key.i4i4.x = x;
        key.i4i4.y = y;

        has = mliAvlDict_get(&hist->dict, key.i8, &ival);
        if (has) {
                double dval = mli_interpret_int64_as_double(ival);
                dval += weight;
                ival = mli_interpret_double_as_int64(dval);

        } else {
                ival = mli_interpret_double_as_int64(weight);
        }
        return mliAvlDict_set(&hist->dict, key.i8, ival);
}

int mliCorsikaHistogram2d_dumps__(
        const struct mliAvlNode *node,
        struct mliIo *f)
{
        int64_t count = 0;
        union i4i4_to_i8 key;
        double dval = 0.0;

        if (node == NULL) {
                return 1;
        }

        key.i8 = node->key;
        dval = mli_interpret_int64_as_double(node->value);

        count = mliIo_write(
                f, (const void *)(&key.i4i4.x), sizeof(uint32_t), 1);
        chk_msg(count == 1, "Failed to write x.");

        count = mliIo_write(
                f, (const void *)(&key.i4i4.y), sizeof(uint32_t), 1);
        chk_msg(count == 1, "Failed to write y.");

        count = mliIo_write(f, (const void *)(&dval), sizeof(double), 1);
        chk_msg(count == 1, "Failed to write weight.");

        if (node->avl.left != NULL) {
                struct mliAvlNode *left = (struct mliAvlNode *)(node->avl.left);
                chk_msg(mliCorsikaHistogram2d_dumps__(left, f), "Failed left");
        }
        if (node->avl.right != NULL) {
                struct mliAvlNode *right =
                        (struct mliAvlNode *)(node->avl.right);
                chk_msg(mliCorsikaHistogram2d_dumps__(right, f), "Failed right");
        }

        return 1;
chk_error:
        return 0;
}

int mliCorsikaHistogram2d_dumps(
        const struct mliCorsikaHistogram2d *hist,
        struct mliIo *f)
{
        int64_t count = 0;

        count = mliIo_write(
                f, (const void *)(&hist->dict.len), sizeof(uint64_t), 1);
        chk_msg(count == 1, "Failed to write dict->len.");

        chk_msg(
                mliCorsikaHistogram2d_dumps__(
                        (const struct mliAvlNode *)hist->dict.tree.root,
                        f
                ),
                "Failed to write dict."
        );

        return 1;
chk_error:
        return 0;
}

void mliCorsikaHistogram2d_reset(struct mliCorsikaHistogram2d *hist)
{
        mliAvlDict_reset(&hist->dict);
}

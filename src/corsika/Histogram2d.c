/* Copyright 2017 Sebastian A. Mueller */

#include "Histogram2d.h"
#include <string.h>
#include "../chk/chk.h"
#include "../math/math.h"

MLI_VECTOR_IMPLEMENTATION(
        mliDynCorsikaHistogram2dBin,
        struct mli_corsika_Histogram2dBin)

struct key {
        int32_t x;
        int32_t y;
};

union i4i4_to_i8 {
        struct key i4i4;
        int64_t i8;
};

struct mli_corsika_Histogram2d mli_corsika_Histogram2d_init(void)
{
        struct mli_corsika_Histogram2d hist;
        hist.dict = mli_AvlDict_init();
        return hist;
}

void mli_corsika_Histogram2d_free(struct mli_corsika_Histogram2d *hist)
{
        mli_AvlDict_free(&hist->dict);
}

int mli_corsika_Histogram2d_malloc(
        struct mli_corsika_Histogram2d *hist,
        const uint64_t capacity)
{
        return mli_AvlDict_malloc(&hist->dict, capacity);
}

int mli_corsika_Histogram2d_assign(
        struct mli_corsika_Histogram2d *hist,
        const int32_t x,
        const int32_t y,
        const double weight)
{
        int has;
        union i4i4_to_i8 key;
        int64_t ival = 0;
        key.i4i4.x = x;
        key.i4i4.y = y;

        has = mli_AvlDict_get(&hist->dict, key.i8, &ival);
        if (has) {
                double dval = mli_math_interpret_int64_as_double(ival);
                dval += weight;
                ival = mli_math_interpret_double_as_int64(dval);

        } else {
                ival = mli_math_interpret_double_as_int64(weight);
        }
        return mli_AvlDict_set(&hist->dict, key.i8, ival);
}

uint64_t mli_corsika_Histogram2d_len(const struct mli_corsika_Histogram2d *hist)
{
        return hist->dict.len;
}

int mli_corsika_Histogram2d_flatten__(
        const struct mli_AvlNode *node,
        struct mliDynCorsikaHistogram2dBin *f)
{
        if (node == NULL) {
                return 1;
        } else {
                union i4i4_to_i8 key;
                struct mli_corsika_Histogram2dBin bin;
                key.i8 = node->key;

                bin.x = key.i4i4.x;
                bin.y = key.i4i4.y;
                bin.value = mli_math_interpret_int64_as_double(node->value);

                chk_msg(mliDynCorsikaHistogram2dBin_push_back(f, bin),
                        "Failed to push back bin-node.");

                if (node->avl.left != NULL) {
                        struct mli_AvlNode *left =
                                (struct mli_AvlNode *)(node->avl.left);
                        chk_msg(mli_corsika_Histogram2d_flatten__(left, f),
                                "Failed left");
                }
                if (node->avl.right != NULL) {
                        struct mli_AvlNode *right =
                                (struct mli_AvlNode *)(node->avl.right);
                        chk_msg(mli_corsika_Histogram2d_flatten__(right, f),
                                "Failed right");
                }
                return 1;
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_corsika_Histogram2d_flatten(
        const struct mli_corsika_Histogram2d *hist,
        struct mliDynCorsikaHistogram2dBin *f)
{
        chk_msg(mli_corsika_Histogram2d_flatten__(
                        (const struct mli_AvlNode *)hist->dict.tree.root, f),
                "Failed to write dict.");

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

void mli_corsika_Histogram2d_reset(struct mli_corsika_Histogram2d *hist)
{
        mli_AvlDict_reset(&hist->dict);
}

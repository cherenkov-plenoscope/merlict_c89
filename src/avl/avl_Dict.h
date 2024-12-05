/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_AVL_DICT_H_
#define MLI_AVL_DICT_H_

#include <stdint.h>
#include <stdlib.h>
#include "avl_Tree.h"

struct mli_AvlDict {
        struct mli_AvlTree tree;
        struct mli_AvlNode *nodes;
        uint64_t capacity;
        uint64_t back;
        uint64_t len;
};

struct mli_AvlDict mli_AvlDict_init(void);
void mli_AvlDict_free(struct mli_AvlDict *dict);
int mli_AvlDict_malloc(struct mli_AvlDict *dict, const uint64_t capacity);

int mli_AvlDict_set(
        struct mli_AvlDict *dict,
        const int64_t key,
        const int64_t value);
int mli_AvlDict_pop(struct mli_AvlDict *dict, const int64_t key);
int mli_AvlDict_has(struct mli_AvlDict *dict, const int64_t key);
int mli_AvlDict_get(struct mli_AvlDict *dict, const int64_t key, int64_t *value);
void mli_AvlDict_reset(struct mli_AvlDict *dict);

#endif

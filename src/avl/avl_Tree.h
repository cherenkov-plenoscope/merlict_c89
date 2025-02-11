/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_AVL_TREE_H_
#define MLI_AVL_TREE_H_

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

enum mli_avltree_depth_changes {
        MLI_AVL_DEPTH_GREW_BY_ONE = 1,
        MLI_AVL_DEPTH_DID_NOT_CHANGE = 0,
        MLI_AVL_DEPTH_SHRUNK_BY_ONE = -1
};

struct mli_Avl {
        struct mli_Avl *left;
        struct mli_Avl *right;
        int64_t balance;
};

struct mli_AvlTree {
        struct mli_Avl *root;
        int64_t (*compare)(const void *a, const void *b);
};

int mli_AvlTree_insert(struct mli_AvlTree *t, struct mli_Avl *a);
int mli_AvlTree_remove(struct mli_AvlTree *t, struct mli_Avl *a);
int mli_AvlTree_removeroot(struct mli_AvlTree *t);
struct mli_Avl *mli_AvlTree_find(
        struct mli_AvlTree *t,
        const struct mli_Avl *probe);

struct mli_AvlNode {
        struct mli_Avl avl;
        int64_t key;
        int64_t value;
};

struct mli_AvlNode mli_AvlNode_init(void);
int64_t mli_AvlNode_compare(const void *a, const void *b);
void mli_AvlNode_print(struct mli_Avl *a, int m);

#endif

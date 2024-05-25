/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIAVLTREE_H_
#define MLIAVLTREE_H_

#include <stdlib.h>
#include <ctype.h>


struct mliAvl {
        struct mliAvl* left;
        struct mliAvl* right;
        int64_t balance;
};

struct mliAvlTree {
        struct mliAvl* root;
        int64_t(*compare)(const void* a, const void* b);
};

int mliAvlTree_insert(struct mliAvlTree* t, struct mliAvl* a);
int mliAvlTree_remove(struct mliAvlTree* t, struct mliAvl* a);
int mliAvlTree_removeroot(struct mliAvlTree* t);
struct mliAvl* mliAvlTree_find(struct mliAvlTree* t, const struct mliAvl* probe);

struct mliAvlNode{
        struct mliAvl avl;
        int64_t key;
        int64_t value;
};

struct mliAvlNode mliAvlNode_init(void);
int64_t mliAvlNode_compare(const void* a, const void* b);
void mliAvlNode_print(struct mliAvl* a, int m);

#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIAVLDICT_H_
#define MLIAVLDICT_H_

#include <stdint.h>
#include <stdlib.h>
#include "mliAvlTree.h"


struct mliAvlDict {
        struct mliAvlTree tree;
        struct mliAvlNode* nodes;
        uint64_t capacity;
        uint64_t back;
        uint64_t len;
};


struct mliAvlDict mliAvlDict_init(void);
void mliAvlDict_free(struct mliAvlDict *dict);
int mliAvlDict_malloc(struct mliAvlDict *dict, const uint64_t capacity);

int mliAvlDict_set(struct mliAvlDict* dict, const int64_t key, const int64_t value);
int mliAvlDict_pop(struct mliAvlDict* dict, const int64_t key);
int mliAvlDict_has(struct mliAvlDict* dict, const int64_t key);
int mliAvlDict_get(struct mliAvlDict* dict, const int64_t key, int64_t* value);
void mliAvlDict_reset(struct mliAvlDict* dict);

#endif

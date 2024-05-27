/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliAvlTree.h"

CASE("mliAvlTree_basics")
{
        struct mliAvlTree ints;
        struct mliAvlNode myint[20];

        struct mliAvlNode probe;
        struct mliAvlNode* out = NULL;

        int i;
        for (i = 0; i < 20; i++) {
                myint[i].key = (i*9) % 20;
        }
        ints.compare = mliAvlNode_compare;
        ints.root=0;
        for (i = 0; i < 20; i++) {
                /*printf("-------------\n");*/
                mliAvlTree_insert(&ints, (struct mliAvl*)&myint[i]);
                /*mliAvlNode_print(ints.root, 0);*/
        }

        probe.key = 15;
        out = (struct mliAvlNode*)mliAvlTree_find(&ints, (const struct mliAvl*)(&probe));
        CHECK(out != NULL);
        CHECK(out->key == 15);

        probe.key = -1337;
        out = (struct mliAvlNode*)mliAvlTree_find(&ints, (const struct mliAvl*)(&probe));
        CHECK(out == NULL);

        for (i = 0; i < 20; i++) {
                /*printf("++++++++++++++\n");*/
                mliAvlTree_remove(&ints, (struct mliAvl*)&myint[i]);
                /*mliAvlNode_print(ints.root, 0);*/
        }
}

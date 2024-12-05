/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_AvlTree_basics")
{
        struct mli_AvlTree ints;
        struct mli_AvlNode myint[20];

        struct mli_AvlNode probe;
        struct mli_AvlNode *out = NULL;

        int i;
        for (i = 0; i < 20; i++) {
                myint[i].key = (i * 9) % 20;
        }
        ints.compare = mli_AvlNode_compare;
        ints.root = 0;
        for (i = 0; i < 20; i++) {
                /*printf("-------------\n");*/
                mli_AvlTree_insert(&ints, (struct mli_Avl *)&myint[i]);
                /*mli_AvlNode_print(ints.root, 0);*/
        }

        probe.key = 15;
        out = (struct mli_AvlNode *)mli_AvlTree_find(
                &ints, (const struct mli_Avl *)(&probe));
        CHECK(out != NULL);
        CHECK(out->key == 15);

        probe.key = -1337;
        out = (struct mli_AvlNode *)mli_AvlTree_find(
                &ints, (const struct mli_Avl *)(&probe));
        CHECK(out == NULL);

        for (i = 0; i < 20; i++) {
                /*printf("++++++++++++++\n");*/
                mli_AvlTree_remove(&ints, (struct mli_Avl *)&myint[i]);
                /*mli_AvlNode_print(ints.root, 0);*/
        }
}

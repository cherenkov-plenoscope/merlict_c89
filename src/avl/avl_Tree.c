#include "avl_Tree.h"
#include <stdio.h>

/* Adopted from:
 *
 * ANSI C Library for maintainance of AVL Balanced Trees
 *
 * ref.:
 *  G. M. Adelson-Velskij & E. M. Landis
 *  Doklady Akad. Nauk SSSR 146 (1962), 263-266
 *
 * see also:
 *  D. E. Knuth: The Art of Computer Programming Vol.3 (Sorting and Searching)
 *
 * (C) 2000 Daniel Nagy, Budapest University of Technology and Economics
 * Released under GNU General Public License (GPL) version 2
 *
 */

/* Swing to the left
 * Warning: no balance maintainance
 */
void mli_Avl_swing_left(struct mli_Avl **root)
{
        /* no balance maintainance */
        struct mli_Avl *a = *root;
        struct mli_Avl *b = a->right;
        *root = b;
        a->right = b->left;
        b->left = a;
}

/* Swing to the right
 * Warning: no balance maintainance
 */
void mli_Avl_swing_right(struct mli_Avl **root)
{
        /* no balance maintainance */
        struct mli_Avl *a = *root;
        struct mli_Avl *b = a->left;
        *root = b;
        a->left = b->right;
        b->right = a;
}

/* Balance maintainance after especially nasty swings
 */
void mli_Avl_rebalance(struct mli_Avl *root)
{
        switch (root->balance) {
        case -1:
                root->left->balance = 0;
                root->right->balance = 1;
                break;
        case 1:
                root->left->balance = -1;
                root->right->balance = 0;
                break;
        case 0:
                root->left->balance = 0;
                root->right->balance = 0;
        }
        root->balance = 0;
}

/* Insert element a into the AVL tree t
 * returns 1 if the depth of the tree has grown
 * Warning: do not insert elements already present
 */
int mli_AvlTree_insert(struct mli_AvlTree *t, struct mli_Avl *a)
{
        /* initialize */
        a->left = 0;
        a->right = 0;
        a->balance = 0;

        /* insert into an empty tree */
        if (!t->root) {
                t->root = a;
                return 1;
        }

        if (t->compare(t->root, a) > 0) {
                /* insert into the left subtree */
                if (t->root->left) {
                        struct mli_AvlTree left_subtree;
                        left_subtree.root = t->root->left;
                        left_subtree.compare = t->compare;
                        if (mli_AvlTree_insert(&left_subtree, a)) {
                                switch (t->root->balance--) {
                                case 1:
                                        return 0;
                                case 0:
                                        return 1;
                                }
                                if (t->root->left->balance < 0) {
                                        mli_Avl_swing_right(&(t->root));
                                        t->root->balance = 0;
                                        t->root->right->balance = 0;
                                } else {
                                        mli_Avl_swing_left(&(t->root->left));
                                        mli_Avl_swing_right(&(t->root));
                                        mli_Avl_rebalance(t->root);
                                }
                        } else {
                                t->root->left = left_subtree.root;
                        }
                        return 0;
                } else {
                        t->root->left = a;
                        if (t->root->balance--) {
                                return 0;
                        }
                        return 1;
                }
        } else {
                /* insert into the right subtree */
                if (t->root->right) {
                        struct mli_AvlTree right_subtree;
                        right_subtree.root = t->root->right;
                        right_subtree.compare = t->compare;
                        if (mli_AvlTree_insert(&right_subtree, a)) {
                                switch (t->root->balance++) {
                                case -1:
                                        return 0;
                                case 0:
                                        return 1;
                                }
                                if (t->root->right->balance > 0) {
                                        mli_Avl_swing_left(&(t->root));
                                        t->root->balance = 0;
                                        t->root->left->balance = 0;
                                } else {
                                        mli_Avl_swing_right(&(t->root->right));
                                        mli_Avl_swing_left(&(t->root));
                                        mli_Avl_rebalance(t->root);
                                }
                        } else {
                                t->root->right = right_subtree.root;
                        }
                        return 0;
                } else {
                        t->root->right = a;
                        if (t->root->balance++) {
                                return 0;
                        }
                        return 1;
                }
        }
        return -1;
}

/* Remove an element a from the AVL tree t
 * returns -1 if the depth of the tree has shrunk
 * Warning: if the element is not present in the tree,
 *          returns 0 as if it had been removed succesfully.
 */
int mli_AvlTree_remove(struct mli_AvlTree *t, struct mli_Avl *a)
{
        int b;
        if (t->root == a) {
                return mli_AvlTree_removeroot(t);
        }
        b = t->compare(t->root, a);

        if (b >= 0) {
                /* remove from the left subtree */
                int ch;
                if (t->root->left) {
                        struct mli_AvlTree left_subtree;
                        left_subtree.root = t->root->left;
                        left_subtree.compare = t->compare;
                        ch = mli_AvlTree_remove(&left_subtree, a);
                        t->root->left = left_subtree.root;
                        if (ch) {
                                switch (t->root->balance++) {
                                case -1:
                                        return -1;
                                case 0:
                                        return 0;
                                }
                                switch (t->root->right->balance) {
                                case 0:
                                        mli_Avl_swing_left(&(t->root));
                                        t->root->balance = -1;
                                        t->root->left->balance = 1;
                                        return 0;
                                case 1:
                                        mli_Avl_swing_left(&(t->root));
                                        t->root->balance = 0;
                                        t->root->left->balance = 0;
                                        return -1;
                                }
                                mli_Avl_swing_right(&(t->root->right));
                                mli_Avl_swing_left(&(t->root));
                                mli_Avl_rebalance(t->root);
                                return -1;
                        }
                }
        }
        if (b <= 0) {
                /* remove from the right subtree */
                int ch;
                if (t->root->right) {
                        struct mli_AvlTree right_subtree;
                        right_subtree.root = t->root->right;
                        right_subtree.compare = t->compare;
                        ch = mli_AvlTree_remove(&right_subtree, a);
                        t->root->right = right_subtree.root;
                        if (ch) {
                                switch (t->root->balance--) {
                                case 1:
                                        return -1;
                                case 0:
                                        return 0;
                                }
                                switch (t->root->left->balance) {
                                case 0:
                                        mli_Avl_swing_right(&(t->root));
                                        t->root->balance = 1;
                                        t->root->right->balance = -1;
                                        return 0;
                                case -1:
                                        mli_Avl_swing_right(&(t->root));
                                        t->root->balance = 0;
                                        t->root->right->balance = 0;
                                        return -1;
                                }
                                mli_Avl_swing_left(&(t->root->left));
                                mli_Avl_swing_right(&(t->root));
                                mli_Avl_rebalance(t->root);
                                return -1;
                        }
                }
        }
        return 0;
}

/* Remove the root of the AVL tree t
 * Warning: dumps core if t is empty
 */
int mli_AvlTree_removeroot(struct mli_AvlTree *t)
{
        int ch;
        struct mli_Avl *a;
        if (!t->root->left) {
                if (!t->root->right) {
                        t->root = 0;
                        return -1;
                }
                t->root = t->root->right;
                return -1;
        }
        if (!t->root->right) {
                t->root = t->root->left;
                return -1;
        }
        if (t->root->balance < 0) {

                /* remove from the left subtree */
                a = t->root->left;
                while (a->right)
                        a = a->right;
        } else {
                /* remove from the right subtree */
                a = t->root->right;
                while (a->left) {
                        a = a->left;
                }
        }
        ch = mli_AvlTree_remove(t, a);
        a->left = t->root->left;
        a->right = t->root->right;
        a->balance = t->root->balance;
        t->root = a;
        if (a->balance == 0)
                return ch;
        return 0;
}

struct mli_Avl *mli_AvlTree_find(
        struct mli_AvlTree *t,
        const struct mli_Avl *probe)
{
        int64_t match;

        if (t->root == NULL) {
                return NULL;
        }

        match = t->compare(probe, t->root);

        if (match == 0) {
                return t->root;
        } else if (match < 0) {
                if (t->root->left != NULL) {
                        struct mli_AvlTree left_subtree;
                        left_subtree.root = t->root->left;
                        left_subtree.compare = t->compare;
                        return mli_AvlTree_find(&left_subtree, probe);
                } else {
                        return NULL;
                }
        } else {
                if (t->root->right != NULL) {
                        struct mli_AvlTree right_subtree;
                        right_subtree.root = t->root->right;
                        right_subtree.compare = t->compare;
                        return mli_AvlTree_find(&right_subtree, probe);
                } else {
                        return NULL;
                }
        }
        return NULL;
}

struct mli_AvlNode mli_AvlNode_init(void)
{
        struct mli_AvlNode n;
        n.avl.left = NULL;
        n.avl.right = NULL;
        n.avl.balance = 0;
        n.key = 0;
        n.value = 0;
        return n;
}

int64_t mli_AvlNode_compare(const void *a, const void *b)
{
        return ((struct mli_AvlNode *)a)->key - ((struct mli_AvlNode *)b)->key;
}

void mli_AvlNode_print(struct mli_Avl *a, int m)
{
        int n = m;
        if (a == NULL) {
                return;
        };
        if (a->right) {
                mli_AvlNode_print(a->right, m + 1);
        }
        while (n--) {
                printf("   ");
        }
        printf("%ld (%ld)\n", ((struct mli_AvlNode *)a)->key, a->balance);
        if (a->left) {
                mli_AvlNode_print(a->left, m + 1);
        }
}

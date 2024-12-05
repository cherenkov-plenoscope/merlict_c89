#include "mliAvlTree.h"
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
void mliAvl_swing_left(struct mliAvl **root)
{
        /* no balance maintainance */
        struct mliAvl *a = *root;
        struct mliAvl *b = a->right;
        *root = b;
        a->right = b->left;
        b->left = a;
}

/* Swing to the right
 * Warning: no balance maintainance
 */
void mliAvl_swing_right(struct mliAvl **root)
{
        /* no balance maintainance */
        struct mliAvl *a = *root;
        struct mliAvl *b = a->left;
        *root = b;
        a->left = b->right;
        b->right = a;
}

/* Balance maintainance after especially nasty swings
 */
void mliAvl_rebalance(struct mliAvl *root)
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
int mliAvlTree_insert(struct mliAvlTree *t, struct mliAvl *a)
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
                        struct mliAvlTree left_subtree;
                        left_subtree.root = t->root->left;
                        left_subtree.compare = t->compare;
                        if (mliAvlTree_insert(&left_subtree, a)) {
                                switch (t->root->balance--) {
                                case 1:
                                        return 0;
                                case 0:
                                        return 1;
                                }
                                if (t->root->left->balance < 0) {
                                        mliAvl_swing_right(&(t->root));
                                        t->root->balance = 0;
                                        t->root->right->balance = 0;
                                } else {
                                        mliAvl_swing_left(&(t->root->left));
                                        mliAvl_swing_right(&(t->root));
                                        mliAvl_rebalance(t->root);
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
                        struct mliAvlTree right_subtree;
                        right_subtree.root = t->root->right;
                        right_subtree.compare = t->compare;
                        if (mliAvlTree_insert(&right_subtree, a)) {
                                switch (t->root->balance++) {
                                case -1:
                                        return 0;
                                case 0:
                                        return 1;
                                }
                                if (t->root->right->balance > 0) {
                                        mliAvl_swing_left(&(t->root));
                                        t->root->balance = 0;
                                        t->root->left->balance = 0;
                                } else {
                                        mliAvl_swing_right(&(t->root->right));
                                        mliAvl_swing_left(&(t->root));
                                        mliAvl_rebalance(t->root);
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
int mliAvlTree_remove(struct mliAvlTree *t, struct mliAvl *a)
{
        int b;
        if (t->root == a) {
                return mliAvlTree_removeroot(t);
        }
        b = t->compare(t->root, a);

        if (b >= 0) {
                /* remove from the left subtree */
                int ch;
                if (t->root->left) {
                        struct mliAvlTree left_subtree;
                        left_subtree.root = t->root->left;
                        left_subtree.compare = t->compare;
                        ch = mliAvlTree_remove(&left_subtree, a);
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
                                        mliAvl_swing_left(&(t->root));
                                        t->root->balance = -1;
                                        t->root->left->balance = 1;
                                        return 0;
                                case 1:
                                        mliAvl_swing_left(&(t->root));
                                        t->root->balance = 0;
                                        t->root->left->balance = 0;
                                        return -1;
                                }
                                mliAvl_swing_right(&(t->root->right));
                                mliAvl_swing_left(&(t->root));
                                mliAvl_rebalance(t->root);
                                return -1;
                        }
                }
        }
        if (b <= 0) {
                /* remove from the right subtree */
                int ch;
                if (t->root->right) {
                        struct mliAvlTree right_subtree;
                        right_subtree.root = t->root->right;
                        right_subtree.compare = t->compare;
                        ch = mliAvlTree_remove(&right_subtree, a);
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
                                        mliAvl_swing_right(&(t->root));
                                        t->root->balance = 1;
                                        t->root->right->balance = -1;
                                        return 0;
                                case -1:
                                        mliAvl_swing_right(&(t->root));
                                        t->root->balance = 0;
                                        t->root->right->balance = 0;
                                        return -1;
                                }
                                mliAvl_swing_left(&(t->root->left));
                                mliAvl_swing_right(&(t->root));
                                mliAvl_rebalance(t->root);
                                return -1;
                        }
                }
        }
        return 0;
}

/* Remove the root of the AVL tree t
 * Warning: dumps core if t is empty
 */
int mliAvlTree_removeroot(struct mliAvlTree *t)
{
        int ch;
        struct mliAvl *a;
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
        ch = mliAvlTree_remove(t, a);
        a->left = t->root->left;
        a->right = t->root->right;
        a->balance = t->root->balance;
        t->root = a;
        if (a->balance == 0)
                return ch;
        return 0;
}

struct mliAvl *mliAvlTree_find(struct mliAvlTree *t, const struct mliAvl *probe)
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
                        struct mliAvlTree left_subtree;
                        left_subtree.root = t->root->left;
                        left_subtree.compare = t->compare;
                        return mliAvlTree_find(&left_subtree, probe);
                } else {
                        return NULL;
                }
        } else {
                if (t->root->right != NULL) {
                        struct mliAvlTree right_subtree;
                        right_subtree.root = t->root->right;
                        right_subtree.compare = t->compare;
                        return mliAvlTree_find(&right_subtree, probe);
                } else {
                        return NULL;
                }
        }
        return NULL;
}

struct mliAvlNode mliAvlNode_init(void)
{
        struct mliAvlNode n;
        n.avl.left = NULL;
        n.avl.right = NULL;
        n.avl.balance = 0;
        n.key = 0;
        n.value = 0;
        return n;
}

int64_t mliAvlNode_compare(const void *a, const void *b)
{
        return ((struct mliAvlNode *)a)->key - ((struct mliAvlNode *)b)->key;
}

void mliAvlNode_print(struct mliAvl *a, int m)
{
        int n = m;
        if (a == NULL) {
                return;
        };
        if (a->right) {
                mliAvlNode_print(a->right, m + 1);
        }
        while (n--) {
                printf("   ");
        }
        printf("%ld (%ld)\n", ((struct mliAvlNode *)a)->key, a->balance);
        if (a->left) {
                mliAvlNode_print(a->left, m + 1);
        }
}

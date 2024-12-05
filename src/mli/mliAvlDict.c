#include "mliAvlDict.h"
#include "../chk/chk.h"
#include "../mtl/math.h"

struct mliAvlDict mliAvlDict_init(void)
{
        struct mliAvlDict dict;
        dict.tree.root = NULL;
        dict.tree.compare = mliAvlNode_compare;

        dict.nodes = NULL;
        dict.capacity = 0u;
        dict.back = 0u;
        dict.len = 0u;
        return dict;
}

void mliAvlDict_free(struct mliAvlDict *dict)
{
        free(dict->nodes);
        (*dict) = mliAvlDict_init();
}

int mliAvlDict_malloc(struct mliAvlDict *dict, const uint64_t capacity)
{
        mliAvlDict_free(dict);
        dict->capacity = capacity;
        chk_malloc(dict->nodes, struct mliAvlNode, dict->capacity);
        return 1;
chk_error:
        mliAvlDict_free(dict);
        return 0;
}

struct mliAvlNode *mliAvlDict_find(struct mliAvlDict *dict, const int64_t key)
{
        struct mliAvlNode *out = NULL;
        struct mliAvlNode probe;
        probe.key = key;
        out = (struct mliAvlNode *)mliAvlTree_find(
                &dict->tree, (const struct mliAvl *)(&probe));

        return out;
}

int mliAvlDict_update__(const struct mliAvlNode *node, struct mliAvlDict *out)
{
        if (node == NULL) {
                return 1;
        }
        chk_msg(mliAvlDict_set(out, node->key, node->value),
                "Failed to insert key/value into destination dict while "
                "updating.");

        if (node->avl.left != NULL) {
                struct mliAvlNode *left = (struct mliAvlNode *)(node->avl.left);
                chk_msg(mliAvlDict_update__(left, out), "1");
        }
        if (node->avl.right != NULL) {
                struct mliAvlNode *right =
                        (struct mliAvlNode *)(node->avl.right);
                chk_msg(mliAvlDict_update__(right, out), "2");
        }

        return 1;
chk_error:
        return 0;
}

void mliAvlDict_swap(struct mliAvlDict *a, struct mliAvlDict *b)
{
        struct mliAvlDict swap = (*a);
        (*a) = (*b);
        (*b) = swap;
}

int mliAvlDict_grow(struct mliAvlDict *dict)
{
        uint64_t new_capacity = (dict->capacity * 2);
        struct mliAvlDict tmp = mliAvlDict_init();

        chk_msg(mliAvlDict_malloc(&tmp, new_capacity),
                "Failed to malloc bigger tmp dict in order to grow.");

        /* copy nodes */
        chk_msg(mliAvlDict_update__(
                        (const struct mliAvlNode *)dict->tree.root, &tmp),
                "Failed to copy nodes over to bigger tmp dict while growing.");

        mliAvlDict_swap(dict, &tmp);
        mliAvlDict_free(&tmp);
        return 1;
chk_error:
        return 0;
}

int mliAvlDict_insert(
        struct mliAvlDict *dict,
        const int64_t key,
        const int64_t value)
{
        int insert_rc;
        struct mliAvlNode *back_node;

        if (dict->back == dict->capacity) {
                chk_msg(mliAvlDict_grow(dict),
                        "AvlTree insertion did not work.");
        }

        dict->nodes[dict->back].key = key;
        dict->nodes[dict->back].value = value;
        back_node = &dict->nodes[dict->back];

        insert_rc =
                mliAvlTree_insert(&dict->tree, (struct mliAvl *)(back_node));
        chk_msg(insert_rc >= 0, "AvlTree insertion did not work.");

        dict->back += 1;
        dict->len += 1;

        return 1;
chk_error:
        return 0;
}

int mliAvlDict_set(
        struct mliAvlDict *dict,
        const int64_t key,
        const int64_t value)
{
        struct mliAvlNode *nn = mliAvlDict_find(dict, key);
        if (nn != NULL) {
                /* key is already in dict. Set it right here. */
                nn->value = value;
        } else {
                /* key is not yet in dict. Insert to grow memory. */
                chk_msg(mliAvlDict_insert(dict, key, value),
                        "Can't insert key/value into mliAvlDict.");
        }
        return 1;
chk_error:
        return 0;
}

int mliAvlDict_shrink(struct mliAvlDict *dict)
{
        uint64_t new_capacity = (dict->len * 3) / 2;
        struct mliAvlDict tmp = mliAvlDict_init();

        chk_msg(mliAvlDict_malloc(&tmp, new_capacity),
                "Failed to malloc smaller tmp dict in order to shrink.");

        /* copy nodes */
        chk_msg(mliAvlDict_update__(
                        (const struct mliAvlNode *)dict->tree.root, &tmp),
                "Failed to copy nodes over to smaller tmp dict while "
                "shrinking.");

        mliAvlDict_swap(dict, &tmp);
        mliAvlDict_free(&tmp);
        return 1;
chk_error:
        return 0;
}

int mliAvlDict_pop(struct mliAvlDict *dict, const int64_t key)
{
        int rc_remove;
        struct mliAvlNode *nn = mliAvlDict_find(dict, key);
        chk_msg(nn != NULL, "key does not exist, and thus can not be removed");

        /* the key exists and can be removed */
        rc_remove = mliAvlTree_remove(&dict->tree, (struct mliAvl *)nn);
        chk_msg(rc_remove <= 0, "AvlTree remove did not work as expected.");

        chk_msg(dict->len > 0, "Expected len > 0.");
        dict->len -= 1;

        if (dict->len < (dict->capacity / 2)) {
                /* shrink */
                chk_msg(mliAvlDict_shrink(dict), "Failed to shrink capacity.");
        }

        return 1;
chk_error:
        return 0;
}

int mliAvlDict_has(struct mliAvlDict *dict, const int64_t key)
{
        struct mliAvlNode *nn = mliAvlDict_find(dict, key);
        if (nn == NULL) {
                return 0;
        } else {
                return 1;
        }
}

int mliAvlDict_get(struct mliAvlDict *dict, const int64_t key, int64_t *value)
{
        struct mliAvlNode *nn = mliAvlDict_find(dict, key);
        if (nn == NULL) {
                return 0;
        } else {
                (*value) = nn->value;
                return 1;
        }
}

void mliAvlDict_reset(struct mliAvlDict *dict)
{
        dict->tree.root = NULL;
        dict->back = 0;
        dict->len = 0;
}

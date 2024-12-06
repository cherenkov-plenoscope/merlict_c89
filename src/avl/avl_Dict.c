#include "avl_Dict.h"
#include "../chk/chk.h"
#include "../math/math.h"

struct mli_AvlDict mli_AvlDict_init(void)
{
        struct mli_AvlDict dict;
        dict.tree.root = NULL;
        dict.tree.compare = mli_AvlNode_compare;

        dict.nodes = NULL;
        dict.capacity = 0u;
        dict.back = 0u;
        dict.len = 0u;
        return dict;
}

void mli_AvlDict_free(struct mli_AvlDict *dict)
{
        free(dict->nodes);
        (*dict) = mli_AvlDict_init();
}

int mli_AvlDict_malloc(struct mli_AvlDict *dict, const uint64_t capacity)
{
        mli_AvlDict_free(dict);
        dict->capacity = capacity;
        chk_malloc(dict->nodes, struct mli_AvlNode, dict->capacity);
        return 1;
chk_error:
        mli_AvlDict_free(dict);
        return 0;
}

struct mli_AvlNode *mli_AvlDict_find(
        struct mli_AvlDict *dict,
        const int64_t key)
{
        struct mli_AvlNode *out = NULL;
        struct mli_AvlNode probe;
        probe.key = key;
        out = (struct mli_AvlNode *)mli_AvlTree_find(
                &dict->tree, (const struct mli_Avl *)(&probe));

        return out;
}

int mli_AvlDict_update__(
        const struct mli_AvlNode *node,
        struct mli_AvlDict *out)
{
        if (node == NULL) {
                return 1;
        }
        chk_msg(mli_AvlDict_set(out, node->key, node->value),
                "Failed to insert key/value into destination dict while "
                "updating.");

        if (node->avl.left != NULL) {
                struct mli_AvlNode *left =
                        (struct mli_AvlNode *)(node->avl.left);
                chk_msg(mli_AvlDict_update__(left, out), "1");
        }
        if (node->avl.right != NULL) {
                struct mli_AvlNode *right =
                        (struct mli_AvlNode *)(node->avl.right);
                chk_msg(mli_AvlDict_update__(right, out), "2");
        }

        return 1;
chk_error:
        return 0;
}

void mli_AvlDict_swap(struct mli_AvlDict *a, struct mli_AvlDict *b)
{
        struct mli_AvlDict swap = (*a);
        (*a) = (*b);
        (*b) = swap;
}

int mli_AvlDict_grow(struct mli_AvlDict *dict)
{
        uint64_t new_capacity = (dict->capacity * 2);
        struct mli_AvlDict tmp = mli_AvlDict_init();

        chk_msg(mli_AvlDict_malloc(&tmp, new_capacity),
                "Failed to malloc bigger tmp dict in order to grow.");

        /* copy nodes */
        chk_msg(mli_AvlDict_update__(
                        (const struct mli_AvlNode *)dict->tree.root, &tmp),
                "Failed to copy nodes over to bigger tmp dict while growing.");

        mli_AvlDict_swap(dict, &tmp);
        mli_AvlDict_free(&tmp);
        return 1;
chk_error:
        return 0;
}

int mli_AvlDict_insert(
        struct mli_AvlDict *dict,
        const int64_t key,
        const int64_t value)
{
        int insert_rc;
        struct mli_AvlNode *back_node;

        if (dict->back == dict->capacity) {
                chk_msg(mli_AvlDict_grow(dict),
                        "AvlTree insertion did not work.");
        }

        dict->nodes[dict->back].key = key;
        dict->nodes[dict->back].value = value;
        back_node = &dict->nodes[dict->back];

        insert_rc =
                mli_AvlTree_insert(&dict->tree, (struct mli_Avl *)(back_node));
        chk_msg(insert_rc >= 0, "AvlTree insertion did not work.");

        dict->back += 1;
        dict->len += 1;

        return 1;
chk_error:
        return 0;
}

int mli_AvlDict_set(
        struct mli_AvlDict *dict,
        const int64_t key,
        const int64_t value)
{
        struct mli_AvlNode *nn = mli_AvlDict_find(dict, key);
        if (nn != NULL) {
                /* key is already in dict. Set it right here. */
                nn->value = value;
        } else {
                /* key is not yet in dict. Insert to grow memory. */
                chk_msg(mli_AvlDict_insert(dict, key, value),
                        "Can't insert key/value into mli_AvlDict.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_AvlDict_shrink(struct mli_AvlDict *dict)
{
        uint64_t new_capacity = (dict->len * 3) / 2;
        struct mli_AvlDict tmp = mli_AvlDict_init();

        chk_msg(mli_AvlDict_malloc(&tmp, new_capacity),
                "Failed to malloc smaller tmp dict in order to shrink.");

        /* copy nodes */
        chk_msg(mli_AvlDict_update__(
                        (const struct mli_AvlNode *)dict->tree.root, &tmp),
                "Failed to copy nodes over to smaller tmp dict while "
                "shrinking.");

        mli_AvlDict_swap(dict, &tmp);
        mli_AvlDict_free(&tmp);
        return 1;
chk_error:
        return 0;
}

int mli_AvlDict_pop(struct mli_AvlDict *dict, const int64_t key)
{
        int rc_remove;
        struct mli_AvlNode *nn = mli_AvlDict_find(dict, key);
        chk_msg(nn != NULL, "key does not exist, and thus can not be removed");

        /* the key exists and can be removed */
        rc_remove = mli_AvlTree_remove(&dict->tree, (struct mli_Avl *)nn);
        chk_msg(rc_remove <= 0, "AvlTree remove did not work as expected.");

        chk_msg(dict->len > 0, "Expected len > 0.");
        dict->len -= 1;

        if (dict->len < (dict->capacity / 2)) {
                /* shrink */
                chk_msg(mli_AvlDict_shrink(dict), "Failed to shrink capacity.");
        }

        return 1;
chk_error:
        return 0;
}

int mli_AvlDict_has(struct mli_AvlDict *dict, const int64_t key)
{
        struct mli_AvlNode *nn = mli_AvlDict_find(dict, key);
        if (nn == NULL) {
                return 0;
        } else {
                return 1;
        }
}

int mli_AvlDict_get(struct mli_AvlDict *dict, const int64_t key, int64_t *value)
{
        struct mli_AvlNode *nn = mli_AvlDict_find(dict, key);
        if (nn == NULL) {
                return 0;
        } else {
                (*value) = nn->value;
                return 1;
        }
}

void mli_AvlDict_reset(struct mli_AvlDict *dict)
{
        dict->tree.root = NULL;
        dict->back = 0;
        dict->len = 0;
}

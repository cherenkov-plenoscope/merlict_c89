/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_MAP_H_
#define MERLICT_MLI_MAP_H_

#include <stdint.h>
#include "mli_debug.h"
#include "mli_variable_length_integer.h"

#define MLI_MAP_TYPE_LEAF 1
#define MLI_MAP_TYPE_NODE 2


struct mliMapNode {
        struct mliMapNode *upper;
        struct mliMapNode *lower;
        uint64_t type;
        uint64_t value;
        struct mliDynUint8 key;
};

struct mliMap {
        uint64_t size;
        struct mliMapNode *root;
};

struct mliMap mliMap_init()
{
        struct mliMap map;
        map.size = 0u;
        map.root = NULL;
        return map;
}

void _mliMap_free_nodes(struct mliMapNode *node)
{
        if (node == NULL)
                return;
        _mliMap_free_nodes(node->lower);
        _mliMap_free_nodes(node->upper);
        if (node->type == MLI_MAP_TYPE_LEAF) {
                mliDynUint8_free(&node->key);
        }
        free(node);
}

void mliMap_free(struct mliMap *map)
{
        _mliMap_free_nodes(map->root);
        map->root = NULL;
}

int _mliMap_insert(struct mliMapNode **node, struct mliMapNode *leaf)
{
        if ((*node) == NULL) {
                *node = leaf;
        } else if ((*node)->type == MLI_MAP_TYPE_NODE) {
                if (mliDynUint8_greater_than(&leaf->key, &(*node)->key)) {
                        _mliMap_insert(&(*node)->upper, leaf);
                } else {
                        _mliMap_insert(&(*node)->lower, leaf);
                }
        } else if ((*node)->type == MLI_MAP_TYPE_LEAF) {
                struct mliDynUint8 tmp_sum = mliDynUint8_init();
                struct mliMapNode *new_node;

                mli_check(!mliDynUint8_equal(&leaf->key, &(*node)->key),
                        "Key already in use");

                mli_c(mliDynUint8_add(
                        &leaf->key,
                        &((*node)->key),
                        &tmp_sum));
                new_node = (struct mliMapNode *)malloc(
                        sizeof(struct mliMapNode));
                mli_check(new_node, "Failed to malloc new node");
                new_node->type = MLI_MAP_TYPE_NODE;
                new_node->value = 0;
                new_node->key = mliDynUint8_init();
                mli_c(mliDynUint8_divide_two(&tmp_sum, &new_node->key));

                if (mliDynUint8_greater_than(&leaf->key, &new_node->key)) {
                        new_node->upper = leaf;
                        new_node->lower = (*node);
                } else {
                        new_node->upper = (*node);
                        new_node->lower = leaf;
                }
                (*node) = new_node;
                mliDynUint8_free(&tmp_sum);
        } else {
                assert(0);
        }
        return 1;
    error:
        return 0;
}

int mliMap_malloc_insert(
        struct mliMap *map,
        const char *key_str,
        const uint64_t value)
{
        struct mliMapNode *leaf = (struct mliMapNode *)malloc(
                sizeof(struct mliMapNode));
        mli_check(leaf != NULL, "Failed to malloc leaf");
        leaf->type = MLI_MAP_TYPE_LEAF;
        leaf->value = value;
        leaf->key = mliDynUint8_init();
        mli_c(mliDynUint8_malloc_char(&leaf->key, key_str));
        leaf->lower = NULL;
        leaf->upper = NULL;
        mli_check(_mliMap_insert(&map->root, leaf), "Failed to insert");
        map->size += 1u;
        return 1;
    error:
        return 0;
}

struct mliMapNode *_mliMap_has(
        struct mliMapNode *node,
        const struct mliDynUint8 *key)
{
        if (node == NULL) {
                return NULL;
        } else if (node->type == MLI_MAP_TYPE_NODE) {
                if (mliDynUint8_greater_than(key, &node->key)) {
                        return _mliMap_has(node->upper, key);
                } else {
                        return _mliMap_has(node->lower, key);
                }
        } else if (node->type == MLI_MAP_TYPE_LEAF) {
                if (mliDynUint8_equal(key, &node->key)) {
                        return node;
                } else {
                        return NULL;
                }
        } else {
                assert(0);
        }
}

int mliMap_get(const struct mliMap *map, const char *key_str, uint64_t *out)
{
        struct mliMapNode *corresponding_node = NULL;
        struct mliDynUint8 tmp_key = mliDynUint8_init();
        tmp_key.dyn.size = strlen(key_str) + 1;
        tmp_key.arr = (uint8_t *)key_str;
        corresponding_node = _mliMap_has(
                map->root,
                &tmp_key);
        if (corresponding_node == NULL) {
                return 0;
        } else {
                (*out) = corresponding_node->value;
                return 1;
        }
}

int mliMap_has(const struct mliMap *map, const char *key_str)
{
        uint64_t _unused = 0;
        return mliMap_get(map, key_str, &_unused);
}

void _mliMap_print(struct mliMapNode *node, const uint64_t indent)
{
        uint64_t i;
        for (i = 0; i < indent; i++) {
                fprintf(stderr, " ");
        }
        if (node == NULL) {
                fprintf(stderr, "NULL\n");
        } else if (node->type == MLI_MAP_TYPE_NODE) {
                fprintf(stderr, "NODE\n");
                _mliMap_print(node->lower, indent+4);
                _mliMap_print(node->upper, indent+4);
        } else if (node->type == MLI_MAP_TYPE_LEAF) {
                fprintf(
                        stderr, "LEAF(%s)\n",
                        node->key.arr);
        } else {
            assert(0);
        }
}

void mliMap_print(struct mliMap *map)
{
        _mliMap_print(map->root, 0);
}

#endif

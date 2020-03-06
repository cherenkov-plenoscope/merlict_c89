/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_MAP_H_
#define MERLICT_MLI_MAP_H_

#include <stdint.h>
#include "mli_debug.h"
#include "mliDynArray_template.h"

#define MLI_MAP_TYPE_LEAF 1
#define MLI_MAP_TYPE_NODE 2

uint64_t mli_dan_bernstein_hash_string(const char *str)
{
        uint64_t hash = 5381;
        uint64_t c;
        while ((c = *str++))
                hash = ((hash << 5) + hash) + c;
        return hash;
}

struct mliMapNode {
        struct mliMapNode *upper;
        struct mliMapNode *lower;
        uint64_t type;
        uint64_t value;
        uint64_t key_hash;
        char *key_str;
};

struct mliMap {
        struct mliMapNode *root;
};

struct mliMap mliMap_init()
{
        struct mliMap map;
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
                free(node->key_str);
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
                if (leaf->key_hash >= (*node)->key_hash) {
                        _mliMap_insert(&(*node)->upper, leaf);
                } else {
                        _mliMap_insert(&(*node)->lower, leaf);
                }
        } else if ((*node)->type == MLI_MAP_TYPE_LEAF) {
                struct mliMapNode *new_node;
                uint64_t mid_key = (leaf->key_hash + (*node)->key_hash)/2;
                mli_check(
                        leaf->key_hash != (*node)->key_hash,
                        "Key already in use");
                new_node = (struct mliMapNode *)malloc(
                        sizeof(struct mliMapNode));
                mli_check(new_node, "Failed to malloc new node");
                new_node->type = MLI_MAP_TYPE_NODE;
                new_node->value = 0;
                new_node->key_hash = mid_key;
                new_node->key_str = NULL;
                if (leaf->key_hash >= mid_key) {
                        new_node->upper = leaf;
                        new_node->lower = (*node);
                } else {
                        new_node->upper = (*node);
                        new_node->lower = leaf;
                }
                (*node) = new_node;
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
        leaf->key_hash = mli_dan_bernstein_hash_string(key_str);

        leaf->key_str = (char *)malloc(strlen(key_str) + 1);
        strncpy(leaf->key_str, key_str, strlen(key_str));
        leaf->key_str[strlen(key_str)] = '\0';

        mli_check(leaf->key_str, "Failed to insert");
        leaf->lower = NULL;
        leaf->upper = NULL;
        mli_check(_mliMap_insert(&map->root, leaf), "Failed to insert");
        return 1;
    error:
        return 0;
}

struct mliMapNode *_mliMap_has(
        struct mliMapNode *node,
        const uint64_t key_hash)
{
        if (node == NULL) {
                return NULL;
        } else if (node->type == MLI_MAP_TYPE_NODE) {
                if (key_hash >= node->key_hash) {
                        return _mliMap_has(node->upper, key_hash);
                } else {
                        return _mliMap_has(node->lower, key_hash);
                }
        } else if (node->type == MLI_MAP_TYPE_LEAF) {
                if (key_hash == node->key_hash) {
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
        const uint64_t key_hash = mli_dan_bernstein_hash_string(key_str);
        struct mliMapNode *corresponding_node = _mliMap_has(
                map->root,
                key_hash);
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
                fprintf(stderr, "NODE(%lu)\n", node->key_hash);
                _mliMap_print(node->lower, indent+4);
                _mliMap_print(node->upper, indent+4);
        } else if (node->type == MLI_MAP_TYPE_LEAF) {
                fprintf(
                        stderr, "LEAF(%lu, '%s')\n",
                        node->key_hash,
                        node->key_str);
        } else {
            assert(0);
        }
}

void mliMap_print(struct mliMap *map)
{
        _mliMap_print(map->root, 0);
}

#endif

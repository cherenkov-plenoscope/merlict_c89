/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_MAP_H_
#define MERLICT_MLI_MAP_H_

#include <stdint.h>
#include "mli_debug.h"

#define MLI_MAP_KEY_CAPACITY 128

struct mliMap2 {
        size_t capacity;
        size_t size;
        char **keys;
        uint32_t *values;
};

struct mliMap2 mliMap2_init(void)
{
        struct mliMap2 map;
        map.capacity = 0u;
        map.size = 0u;
        map.keys = NULL;
        map.values = NULL;
        return map;
}

void mliMap2_free(struct mliMap2 *map)
{
        if (map->keys != NULL) {
                size_t i;
                for (i = 0; i < map->capacity; i++) {
                        free(map->keys[i]);
                }
        }
        free(map->keys);
        free(map->values);
        (*map) = mliMap2_init();
}

void _mliMap2_set_zero(struct mliMap2 *map)
{
        size_t i, j;
        for (i = 0u; i < map->capacity; i++) {
                for (j = 0u; j < MLI_MAP_KEY_CAPACITY; j++) {
                        map->keys[i][j] = '\0';
                }
                map->values[i] = 0u;
        }
}

int mliMap2_malloc(struct mliMap2 *map, const size_t capacity)
{
        size_t i;
        mliMap2_free(map);
        map->capacity = capacity;
        mli_malloc(map->keys, char *, map->capacity);
        for (i = 0u; i < map->capacity; i++) {
                mli_malloc(map->keys[i], char, MLI_MAP_KEY_CAPACITY);
        }
        mli_malloc(map->values, uint32_t, map->capacity);
        _mliMap2_set_zero(map);
        return 1;
error:
        return 0;
}

int32_t _mliMap2_key_idx(const struct mliMap2 *map, const char *key)
{
        int32_t i;
        for (i = 0; i < (int32_t)map->size; i++) {
                if (strcmp(map->keys[i], key) == 0)
                        return i;
        }
        return -1;
}

int32_t _mliMap2_value_idx(const struct mliMap2 *map, const uint32_t value)
{
        int32_t i;
        for (i = 0; i < (int32_t)map->size; i++) {
                if (map->values[i] == value)
                        return i;
        }
        return -1;
}

int mliMap2_insert(struct mliMap2 *map, const char *key, const uint32_t value)
{
        mli_check(
                map->size < map->capacity, "Can not insert, reached capacity.");
        mli_check(
                strlen(key) < MLI_MAP_KEY_CAPACITY,
                "Can not insert, key is too long.");
        mli_check(
                _mliMap2_key_idx(map, key) < 0,
                "Can not insert, key is already in use.");
        mli_check(
                _mliMap2_value_idx(map, value) < 0,
                "Can not insert, value is already in use.");
        strcpy(map->keys[map->size], key);
        map->values[map->size] = value;
        map->size++;
        return 1;
error:
        return 0;
}

int mliMap2_get_value(
        const struct mliMap2 *map,
        const char *key,
        uint32_t *value)
{
        int32_t idx = _mliMap2_key_idx(map, key);
        if (idx >= 0) {
                (*value) = map->values[idx];
                return 1;
        } else {
                return 0;
        }
}

int mliMap2_get_key(const struct mliMap2 *map, const uint32_t value, char *key)
{
        int32_t idx = _mliMap2_value_idx(map, value);
        if (idx >= 0) {
                strcpy(key, map->keys[idx]);
                return 1;
        } else {
                return 0;
        }
}

int mliMap2_has(const struct mliMap2 *map, const char *key)
{
        return _mliMap2_key_idx(map, key) >= 0 ? 1 : 0;
}

int mliMap2_has_value(const struct mliMap2 *map, const uint32_t value)
{
        return _mliMap2_value_idx(map, value) >= 0 ? 1 : 0;
}

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

struct mliMap mliMap_init(void)
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

                mli_check(
                        !mliDynUint8_equal(&leaf->key, &(*node)->key),
                        "Key already in use");

                mli_c(mliDynUint8_add(&leaf->key, &((*node)->key), &tmp_sum));
                new_node =
                        (struct mliMapNode *)malloc(sizeof(struct mliMapNode));
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
        struct mliMapNode *leaf =
                (struct mliMapNode *)malloc(sizeof(struct mliMapNode));
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
        corresponding_node = _mliMap_has(map->root, &tmp_key);
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
                _mliMap_print(node->lower, indent + 4);
                _mliMap_print(node->upper, indent + 4);
        } else if (node->type == MLI_MAP_TYPE_LEAF) {
                fprintf(stderr, "LEAF(%s)\n", node->key.arr);
        } else {
                assert(0);
        }
}

void mliMap_print(struct mliMap *map) { _mliMap_print(map->root, 0); }

#endif

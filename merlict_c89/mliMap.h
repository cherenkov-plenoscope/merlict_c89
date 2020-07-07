/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMAP_H_
#define MERLICT_C89_MLIMAP_H_

#include <stdint.h>

#include "mli_debug.h"
#include "mli_variable_length_integer.h"

#define MLI_MAP_KEY_CAPACITY 128
#define MLI_MAP_TYPE_LEAF 1
#define MLI_MAP_TYPE_NODE 2

struct mliMap2 {
        uint64_t capacity;
        uint64_t size;
        char **keys;
        uint32_t *values;
};

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

void mliMap_print(struct mliMap *map);
void _mliMap_print(struct mliMapNode *node, const uint64_t indent);
int mliMap_has(const struct mliMap *map, const char *key_str);
int mliMap_get(const struct mliMap *map, const char *key_str, uint64_t *out);
struct mliMapNode *_mliMap_has(
        struct mliMapNode *node,
        const struct mliDynUint8 *key);
int mliMap_malloc_insert(
        struct mliMap *map,
        const char *key_str,
        const uint64_t value);
int _mliMap_insert(struct mliMapNode **node, struct mliMapNode *leaf);
void mliMap_free(struct mliMap *map);
void _mliMap_free_nodes(struct mliMapNode *node);
struct mliMap mliMap_init(void);
int mliMap2_has_value(const struct mliMap2 *map, const uint32_t value);
int mliMap2_has(const struct mliMap2 *map, const char *key);
int mliMap2_get_key(const struct mliMap2 *map, const uint32_t value, char *key);
int mliMap2_get_value(
        const struct mliMap2 *map,
        const char *key,
        uint32_t *value);
int mliMap2_insert(struct mliMap2 *map, const char *key, const uint32_t value);
int32_t _mliMap2_value_idx(const struct mliMap2 *map, const uint32_t value);
int32_t _mliMap2_key_idx(const struct mliMap2 *map, const char *key);
int mliMap2_malloc(struct mliMap2 *map, const uint64_t capacity);
void _mliMap2_set_zero(struct mliMap2 *map);
void mliMap2_free(struct mliMap2 *map);
struct mliMap2 mliMap2_init(void);
#endif

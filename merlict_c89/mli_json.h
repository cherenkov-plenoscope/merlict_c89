/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_JSON_H_
#define MERLICT_C89_MLI_JSON_H_

#include <stdint.h>
#include "jsmn.h"

struct mliJson {
        uint64_t num_chars;
        char *chars;
        uint64_t num_tokens;
        struct jsmntok_t *tokens;
};

int mliJson_debug_export(const struct mliJson *json, const char *path);
int mliJson_debug_fprint(FILE *f, const struct mliJson *json);
uint64_t mliJson_array_child_token(
        const struct mliJson *json,
        const uint64_t start_token_idx,
        const uint64_t child_idx);
int mliJson_find_key(
        const struct mliJson *json,
        const uint64_t start_token_idx,
        const char *key,
        uint64_t *return_idx);
int mliJson_as_float64(
        const struct mliJson *json,
        const uint64_t token_idx,
        double *return_float64);
int mliJson_as_int64(
        const struct mliJson *json,
        const uint64_t token_idx,
        int64_t *return_int64);
int mliJson_as_string(
        const struct mliJson *json,
        const uint64_t token_idx,
        char *return_string,
        const uint64_t return_string_size);
int mliJson_malloc_from_file(struct mliJson *json, const char *path);
int mliJson_malloc_from_string(struct mliJson *json, const char *json_str);
int mliJson_malloc(struct mliJson *json, const uint64_t json_strlen);
void _mliJson_set_zero(struct mliJson *json);
void mliJson_free(struct mliJson *json);
struct mliJson mliJson_init(void);
#endif

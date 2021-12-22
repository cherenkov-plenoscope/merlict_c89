/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_JSON_H_
#define MLI_JSON_H_

#include <stdint.h>
#include "mli_json_jsmn.h"
#include "mliStr.h"

struct mliJson {
        struct mliStr raw;
        uint64_t num_tokens;
        struct jsmntok_t *tokens;
};

int mliJson_debug_to_path(const struct mliJson *json, const char *path);
int mliJson_debug_fprint(FILE *f, const struct mliJson *json);
int mliJson_debug_token_fprint(
        FILE *f,
        const struct mliJson *json,
        const uint64_t token);
uint64_t mliJson_array_child_token(
        const struct mliJson *json,
        const uint64_t start_token_idx,
        const uint64_t child_idx);
int mliJson_find_key(
        const struct mliJson *json,
        const uint64_t start_token_idx,
        const char *key,
        uint64_t *return_idx);
int mliJson_find_expected_key(
        const struct mliJson *json,
        const uint64_t token,
        const char *key,
        uint64_t *ret_token);
int mliJson_as_float64(
        const struct mliJson *json,
        const uint64_t token_idx,
        double *return_float64);
int mliJson_double_by_key(
        const struct mliJson *json,
        const uint64_t token,
        double *val,
        const char *key);
int mliJson_as_int64(
        const struct mliJson *json,
        const uint64_t token_idx,
        int64_t *return_int64);
int mliJson_as_string(
        const struct mliJson *json,
        const uint64_t token_idx,
        char *return_string,
        const uint64_t return_string_size);
int mliJson_int64_by_key(
        const struct mliJson *json,
        const uint64_t token_idx,
        int64_t *return_int64,
        const char *key);
int mliJson_strcmp(
        const struct mliJson *json,
        const uint64_t token_idx,
        const char *str);
int mliJson_malloc_from_path(struct mliJson *json, const char *path);
int mliJson_malloc_from_string(struct mliJson *json, const char *json_str);
int mliJson_malloc_tokens__(struct mliJson *json);
void mliJson_free(struct mliJson *json);
struct mliJson mliJson_init(void);
#endif

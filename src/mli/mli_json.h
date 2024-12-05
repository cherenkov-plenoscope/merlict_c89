/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_JSON_H_
#define MLI_JSON_H_

#include <stdint.h>
#include <stdio.h>
#include "mli_json_jsmn.h"
#include "../io/io.h"
#include "../string/string.h"

struct mliJson {
        struct mli_String raw;
        uint64_t num_tokens;
        struct jsmntok_t *tokens;
};

int mliJson_debug_to_path(const struct mliJson *json, const char *path);
int mliJson_debug_fprint(FILE *f, const struct mliJson *json);
int mliJson_debug_token_fprint(
        FILE *f,
        const struct mliJson *json,
        const uint64_t token);
uint64_t mliJson_token_by_index(
        const struct mliJson *json,
        const uint64_t start_token_idx,
        const uint64_t child_idx);
int mliJson_token_by_key(
        const struct mliJson *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token);
int mliJson_token_by_key_eprint(
        const struct mliJson *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token);
int mliJson_double_by_token(
        const struct mliJson *json,
        const uint64_t token,
        double *val);
int mliJson_double_by_key(
        const struct mliJson *json,
        const uint64_t token,
        double *val,
        const char *key);
int mliJson_int64_by_token(
        const struct mliJson *json,
        const uint64_t token,
        int64_t *return_int64);
int mliJson_uint64_by_token(
        const struct mliJson *json,
        const uint64_t token,
        uint64_t *return_uint64);
int mliJson_cstr_by_token(
        const struct mliJson *json,
        const uint64_t token,
        char *return_string,
        const uint64_t return_string_size);
int mliJson_int64_by_key(
        const struct mliJson *json,
        const uint64_t token,
        int64_t *val,
        const char *key);
int mliJson_uint64_by_key(
        const struct mliJson *json,
        const uint64_t token,
        uint64_t *val,
        const char *key);
int mliJson_cstrcmp(
        const struct mliJson *json,
        const uint64_t token,
        const char *str);
int mliJson_malloc_from_path(struct mliJson *json, const char *path);
int mliJson_malloc_from_cstr(struct mliJson *json, const char *cstr);
int mliJson_malloc_tokens__(struct mliJson *json);
int mliJson_parse_tokens__(struct mliJson *json);
void mliJson_free(struct mliJson *json);
struct mliJson mliJson_init(void);
#endif

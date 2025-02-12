/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_JSON_H_
#define MLI_JSON_H_

#include <stdint.h>
#include <stdio.h>
#include "json_jsmn.h"
#include "../chk/chk.h"
#include "../io/io.h"
#include "../string/string.h"

struct mli_Json {
        struct mli_String raw;
        uint64_t num_tokens;
        struct jsmntok_t *tokens;
};

chk_rc mli_Json_from_string(
        struct mli_Json *self,
        const struct mli_String *str);
chk_rc mli_Json_from_io(struct mli_Json *self, struct mli_IO *io);
void mli_Json_free(struct mli_Json *self);
struct mli_Json mli_Json_init(void);

chk_rc mli_Json_debug_to_io(const struct mli_Json *self, struct mli_IO *io);
chk_rc mli_Json_debug_token_to_io(
        const struct mli_Json *self,
        const uint64_t token,
        struct mli_IO *io);
chk_rc mli_Json_debug_token_fprint(
        FILE *f,
        const struct mli_Json *self,
        const uint64_t token);

uint64_t mli_Json__token_by_index_unsafe(
        const struct mli_Json *json,
        const uint64_t start_token_idx,
        const uint64_t child_idx);
chk_rc mli_Json_token_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token);
chk_rc mli_Json_token_by_idx(
        const struct mli_Json *json,
        const uint64_t token,
        const uint64_t idx,
        uint64_t *idx_token);

chk_rc mli_Json_token_by_key_eprint(
        const struct mli_Json *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token);
chk_rc mli_Json_double_by_token(
        const struct mli_Json *json,
        const uint64_t token,
        double *val);
chk_rc mli_Json_double_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        double *val,
        const char *key);
chk_rc mli_Json_int64_by_token(
        const struct mli_Json *json,
        const uint64_t token,
        int64_t *return_int64);
chk_rc mli_Json_uint64_by_token(
        const struct mli_Json *json,
        const uint64_t token,
        uint64_t *return_uint64);
chk_rc mli_Json_cstr_by_token(
        const struct mli_Json *json,
        const uint64_t token,
        char *return_string,
        const uint64_t return_string_size);
chk_rc mli_Json_string_by_token(
        const struct mli_Json *json,
        const uint64_t token,
        struct mli_String *return_string);
chk_rc mli_Json_int64_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        int64_t *val,
        const char *key);
chk_rc mli_Json_uint64_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        uint64_t *val,
        const char *key);
mli_bool mli_Json_cstrcmp(
        const struct mli_Json *json,
        const uint64_t token,
        const char *str);

chk_rc mli_Json__malloc_tokens(struct mli_Json *json);
chk_rc mli_Json__parse_tokens(struct mli_Json *json);
#endif

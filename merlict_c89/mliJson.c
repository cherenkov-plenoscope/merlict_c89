/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliJson.h"
#include <stdlib.h>
#include "mli_debug.h"
#include "mli_string_to.h"

struct mliJson mliJson_init(void)
{
        struct mliJson j;
        j.c_str_capacity = 0u;
        j.c_str = NULL;
        j.num_tokens = 0u;
        j.tokens = NULL;
        return j;
}

void mliJson_free(struct mliJson *json)
{
        free(json->c_str);
        free(json->tokens);
        (*json) = mliJson_init();
}

void _mliJson_set_zero(struct mliJson *json)
{
        uint64_t i;
        for (i = 0; i < json->c_str_capacity; i++) {
                json->c_str[i] = '\0';
        }
        for (i = 0; i < json->num_tokens; i++) {
                json->tokens[i].type = JSMN_UNDEFINED;
                json->tokens[i].start = 0;
                json->tokens[i].end = 0;
                json->tokens[i].size = 0;
        }
}

int mliJson_malloc(struct mliJson *json, const uint64_t json_strlen)
{
        mliJson_free(json);
        json->c_str_capacity = json_strlen + 1u;     /* NULL termination. */
        json->num_tokens = json->c_str_capacity / 4; /* A rather safe guess. */
        mli_malloc(json->c_str, char, json->c_str_capacity);
        mli_malloc(json->tokens, struct jsmntok_t, json->num_tokens);
        _mliJson_set_zero(json);
        return 1;
error:
        mliJson_free(json);
        return 0;
}

int mliJson_malloc_from_string(struct mliJson *json, const char *json_str)
{
        int64_t num_tokens_parsed;
        uint64_t json_strlen = strlen(json_str);
        struct jsmn_parser parser;
        mli_check_mem(mliJson_malloc(json, json_strlen));
        strcpy(json->c_str, json_str);
        json->c_str[json_strlen] = '\0';
        jsmn_init(&parser);
        num_tokens_parsed = jsmn_parse(
                &parser,
                json->c_str,
                json->c_str_capacity,
                json->tokens,
                json->num_tokens);
        mli_check(
                num_tokens_parsed != JSMN_ERROR_NOMEM,
                "Not enough tokens were provided.");
        mli_check(
                num_tokens_parsed != JSMN_ERROR_INVAL,
                "Invalid character inside JSON string.");
        mli_check(
                num_tokens_parsed != JSMN_ERROR_PART,
                "The string is not a full JSON packet, more "
                "bytes expected.");
        mli_check(num_tokens_parsed >= 0, "Can not parse Json-string");
        json->num_tokens = num_tokens_parsed;
        return 1;
error:
        mliJson_free(json);
        return 0;
}

int mliJson_malloc_from_path(struct mliJson *json, const char *path)
{
        struct mliString str = mliString_init();
        mli_check(
                mliString_malloc_from_path(&str, path),
                "Failed to read file into string.");
        mli_check(
                mliJson_malloc_from_string(json, str.c_str),
                "Failed to parse json-string read from path.");
        mliString_free(&str);
        return 1;
error:
        mliJson_free(json);
        return 0;
}

int mliJson_as_string(
        const struct mliJson *json,
        const uint64_t token_idx,
        char *return_string,
        const uint64_t return_string_size)
{
        const struct jsmntok_t t = json->tokens[token_idx];
        const uint64_t actual_length = t.end - t.start;
        mli_check(
                actual_length < return_string_size,
                "Expected return_string_size to be sufficiently large for "
                "json-string, but it is not.")
                memcpy(return_string, json->c_str + t.start, actual_length);
        return_string[actual_length] = '\0';
        return 1;
error:
        return 0;
}

int mliJson_as_int64(
        const struct mliJson *json,
        const uint64_t token_idx,
        int64_t *return_int64)
{
        const struct jsmntok_t t = json->tokens[token_idx];
        const uint64_t token_length = t.end - t.start;
        mli_check(
                t.type == JSMN_PRIMITIVE,
                "Json int64 expected json-token-to be JSMN_PRIMITIVE.");
        mli_check(
                mli_nstring_to_int(return_int64, &json->c_str[t.start], 10, token_length),
                "Can not parse int.");
        return 1;
error:
        return 0;
}

int mliJson_as_float64(
        const struct mliJson *json,
        const uint64_t token_idx,
        double *return_float64)
{
        struct jsmntok_t t = json->tokens[token_idx];
        uint64_t buff_size = t.end - t.start + 1u;
        char *buff = NULL;
        mli_check(
                t.type == JSMN_PRIMITIVE,
                "Json float64 expected json-token-to be JSMN_PRIMITIVE.");
        mli_malloc(buff, char, buff_size);
        mliJson_as_string(json, token_idx, buff, buff_size);
        mli_check(
                mli_string_to_float(return_float64, buff),
                "Can not parse float.");
        free(buff);
        return 1;
error:
        free(buff);
        return 0;
}

int _mliJson_strcmp(
        const struct mliJson *json,
        const uint64_t token_idx,
        const char *str)
{
        uint64_t i;
        const struct jsmntok_t t = json->tokens[token_idx];
        const uint64_t token_length = t.end - t.start;
        const uint64_t str_length = strlen(str);

        if (token_length != str_length) {
                return 0;
        }
        for (i = 0; i < token_length; i++) {
               const char token_char = json->c_str[t.start + i];
               const char str_char = str[i];
               if (token_char != str_char) {
                        return 0;
               }
        }
        return 1;
}

int mliJson_find_key(
        const struct mliJson *json,
        const uint64_t start_token_idx,
        const char *key,
        uint64_t *return_idx)
{
        int64_t found = 0;
        int64_t child = 0;
        int64_t subchild_balance = 0;
        int64_t idx = start_token_idx + 1;

        while (child < json->tokens[start_token_idx].size) {
                if (_mliJson_strcmp(json, idx, key)) {
                        (*return_idx) = idx;
                        found += 1;
                }
                subchild_balance += json->tokens[idx].size;
                while (subchild_balance > 0) {
                        idx += 1;
                        subchild_balance += json->tokens[idx].size;
                        subchild_balance -= 1;
                }
                idx += 1;
                child += 1;
        }
        mli_check(found, "No such key in json-object.");
        return 1;
error:
        return 0;
}

uint64_t mliJson_array_child_token(
        const struct mliJson *json,
        const uint64_t start_token_idx,
        const uint64_t child_idx)
{
        uint64_t child = 0;
        int64_t subchild_balance = 0;
        uint64_t idx = start_token_idx + 1;

        while (child < child_idx) {
                subchild_balance += json->tokens[idx].size;
                while (subchild_balance > 0) {
                        idx += 1;
                        subchild_balance += json->tokens[idx].size;
                        subchild_balance -= 1;
                }
                idx += 1;
                child += 1;
        }
        return idx;
}

int mliJson_debug_token_fprint(
        FILE *f,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t i = 0u;
        uint32_t token_size = 0u;
        struct jsmntok_t t = json->tokens[token];
        token_size = t.end - t.start;
        mli_c(fprintf(f, "Token: %lu ", i));
        mli_c(fprintf(f, "sz: %d ", t.size));
        mli_c(fprintf(f, "type: %d ", t.type));
        mli_c(fprintf(f, "(%d -> %d, %d)\n", t.start, t.end, token_size));
        for (i = 0; i < token_size; i++) {
                mli_c(fputc(json->c_str[t.start + i], f));
        }
        mli_c(fprintf(f, "\n"));
        return 1;
error:
        return 0;
}

int mliJson_debug_fprint(FILE *f, const struct mliJson *json)
{
        uint64_t i;
        for (i = 0; i < json->num_tokens; i++) {
                mli_check(
                        mliJson_debug_token_fprint(f, json, i),
                        "Failed to write json-token debug-info to file.");
        }
        return 1;
error:
        return 0;
}

int mliJson_debug_to_path(const struct mliJson *json, const char *path)
{
        FILE *f;
        f = fopen(path, "wt");
        mli_check(f != NULL, "Failed to open file for Json debug output.");
        mli_check(mliJson_debug_fprint(f, json), "Failed to fprint debug.");
        fclose(f);
        return 1;
error:
        fclose(f);
        return 0;
}

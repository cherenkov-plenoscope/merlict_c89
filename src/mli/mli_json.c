/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_json.h"
#include <stdlib.h>
#include "../cstr/cstr.h"
#include "../cstr/cstr_numbers.h"
#include "mli_json_jsmn.h"
#include "../math/math.h"
#include "../chk/chk.h"

struct mliJson mliJson_init(void)
{
        struct mliJson j;
        j.raw = mli_String_init();
        j.num_tokens = 0u;
        j.tokens = NULL;
        return j;
}

void mliJson_free(struct mliJson *json)
{
        mli_String_free(&json->raw);
        free(json->tokens);
        (*json) = mliJson_init();
}

int mliJson_malloc_tokens__(struct mliJson *json)
{
        struct jsmntok_t default_token = {JSMN_UNDEFINED, 0, 0, 0};
        chk_msg(&json->raw.array != NULL, "Expected raw cstr to be malloced.");
        json->num_tokens = json->raw.size / 2;
        chk_malloc(json->tokens, struct jsmntok_t, json->num_tokens);
        MLI_MATH_ARRAY_SET(json->tokens, default_token, json->num_tokens);
        return 1;
chk_error:
        return 0;
}

int mliJson_parse_tokens__(struct mliJson *json)
{
        int64_t num_tokens_parsed;
        struct jsmn_parser parser;

        chk_msg(&json->tokens != NULL, "Expected tokens to be malloced.");
        jsmn_init(&parser);
        num_tokens_parsed = jsmn_parse(
                &parser,
                json->raw.array,
                json->raw.size,
                json->tokens,
                json->num_tokens);
        chk_msgf(
                num_tokens_parsed != JSMN_ERROR_NOMEM,
                ("Not enough tokens. Only got " PRIu64, json->num_tokens));
        chk_msg(num_tokens_parsed != JSMN_ERROR_INVAL,
                "Invalid character inside JSON string.");
        chk_msg(num_tokens_parsed != JSMN_ERROR_PART,
                "The string is not a full JSON packet, more "
                "bytes expected.");
        chk_msg(num_tokens_parsed >= 0, "Can't parse Json-string");
        json->num_tokens = num_tokens_parsed;
        return 1;
chk_error:
        return 0;
}

int mliJson_malloc_from_cstr(struct mliJson *json, const char *cstr)
{
        mliJson_free(json);
        chk_msg(mli_String_from_cstr(&json->raw, cstr), "Can't copy cstr.");
        chk_msg(mliJson_malloc_tokens__(json), "Can't malloc Json's tokens.");
        chk_msg(mliJson_parse_tokens__(json), "Can't parse Json into tokens.");
        return 1;
chk_error:
        mliJson_free(json);
        return 0;
}

int mliJson_malloc_from_path(struct mliJson *json, const char *path)
{
        struct mli_IO ff = mli_IO_init();
        mliJson_free(json);
        chk_msg(mli_IO_write_from_path(&ff, path),
                "Failed to read file into Json's Str.");
        chk_msg(mli_String_from_cstr(&json->raw, (char *)ff.cstr),
                "Failed to copy cstr.");
        mli_IO_free(&ff);
        chk_msg(mliJson_malloc_tokens__(json), "Can't malloc Json's tokens.");
        chk_msg(mliJson_parse_tokens__(json), "Can't parse Json into tokens.");
        return 1;
chk_error:
        mli_IO_free(&ff);
        mliJson_free(json);
        return 0;
}

int mliJson_cstr_by_token(
        const struct mliJson *json,
        const uint64_t token,
        char *return_string,
        const uint64_t return_string_size)
{
        const struct jsmntok_t t = json->tokens[token];
        const uint64_t actual_length = t.end - t.start;
        chk_msg(actual_length < return_string_size,
                "Expected return_string_size to be sufficiently large for "
                "json-string, but it is not.");
        memcpy(return_string, json->raw.array + t.start, actual_length);
        return_string[actual_length] = '\0';
        return 1;
chk_error:
        return 0;
}

int mliJson_int64_by_token(
        const struct mliJson *json,
        const uint64_t token,
        int64_t *return_int64)
{
        const struct jsmntok_t t = json->tokens[token];
        const uint64_t token_length = t.end - t.start;
        chk_msg(t.type == JSMN_PRIMITIVE,
                "Json int64 expected json-token-to be JSMN_PRIMITIVE.");
        chk_msg(mli_cstr_nto_int64(
                        return_int64,
                        (char *)&json->raw.array[t.start],
                        10,
                        token_length),
                "Can't parse int64.");
        return 1;
chk_error:
        return 0;
}

int mliJson_uint64_by_token(
        const struct mliJson *json,
        const uint64_t token,
        uint64_t *val)
{
        int64_t tmp;
        chk(mliJson_int64_by_token(json, token, &tmp));
        chk_msg(tmp >= 0, "Expected value to be unsigned.");
        (*val) = (uint64_t)tmp;
        return 1;
chk_error:
        return 0;
}

int mliJson_int64_by_key(
        const struct mliJson *json,
        const uint64_t token,
        int64_t *val,
        const char *key)
{
        uint64_t token_n;
        chk(mliJson_token_by_key_eprint(json, token, key, &token_n));
        chk_msgf(
                mliJson_int64_by_token(json, token_n + 1, val),
                ("Can't parse value of '%s' into int64.", key));
        return 1;
chk_error:
        return 0;
}

int mliJson_uint64_by_key(
        const struct mliJson *json,
        const uint64_t token,
        uint64_t *val,
        const char *key)
{
        int64_t tmp;
        chk(mliJson_int64_by_key(json, token, &tmp, key));
        chk_msg(tmp >= 0, "Expected value to be unsigned.");
        (*val) = (uint64_t)tmp;
        return 1;
chk_error:
        return 0;
}

int mliJson_double_by_token(
        const struct mliJson *json,
        const uint64_t token,
        double *val)
{
        const struct jsmntok_t t = json->tokens[token];
        const uint64_t token_length = t.end - t.start;
        chk_msg(t.type == JSMN_PRIMITIVE,
                "Json float64 expected json-token-to be JSMN_PRIMITIVE.");
        chk_msg(mli_cstr_nto_double(
                        val, (char *)&json->raw.array[t.start], token_length),
                "Can't parse double.");
        return 1;
chk_error:
        return 0;
}

int mliJson_double_by_key(
        const struct mliJson *json,
        const uint64_t token,
        double *val,
        const char *key)
{
        uint64_t token_n;
        chk(mliJson_token_by_key_eprint(json, token, key, &token_n));
        chk_msgf(
                mliJson_double_by_token(json, token_n + 1, val),
                ("Can't parse value of '%s' into double.", key));

        return 1;
chk_error:
        return 0;
}

int mliJson_cstrcmp(
        const struct mliJson *json,
        const uint64_t token,
        const char *str)
{
        uint64_t i;
        const struct jsmntok_t t = json->tokens[token];
        const uint64_t token_length = t.end - t.start;
        const uint64_t str_length = strlen(str);

        if (token_length != str_length) {
                return 0;
        }
        for (i = 0; i < token_length; i++) {
                const char token_char = (char)json->raw.array[t.start + i];
                const char str_char = str[i];
                if (token_char != str_char) {
                        return 0;
                }
        }
        return 1;
}

int mliJson_token_by_key(
        const struct mliJson *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token)
{
        int64_t found = 0;
        int64_t child = 0;
        int64_t subchild_balance = 0;
        int64_t idx = token + 1;

        while (child < json->tokens[token].size) {
                if (mliJson_cstrcmp(json, idx, key)) {
                        (*key_token) = idx;
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
        return found;
}

int mliJson_token_by_key_eprint(
        const struct mliJson *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token)
{
        chk_msgf(
                mliJson_token_by_key(json, token, key, key_token),
                ("Expected key '%s' in json.", key));
        return 1;
chk_error:
        return 0;
}

uint64_t mliJson_token_by_index(
        const struct mliJson *json,
        const uint64_t token,
        const uint64_t index)
{
        uint64_t child = 0;
        int64_t subchild_balance = 0;
        uint64_t idx = token + 1;

        while (child < index) {
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
        struct jsmntok_t t = json->tokens[token];
        uint32_t token_size = t.end - t.start;
        uint64_t line_number =
                1u + mli_String_countn(&json->raw, '\n', t.start);
        chk(fprintf(f, "line: %u, ", (uint32_t)line_number));
        chk(fprintf(f, "token: %u, ", (uint32_t)token));
        chk(fprintf(f, "type: %d, ", t.type));
        chk(fprintf(f, "children: %d, ", t.size));
        chk(fprintf(f, "chars: (%d -> %d, %d)\n", t.start, t.end, token_size));
        for (i = 0; i < token_size; i++) {
                chk(fputc((char)json->raw.array[t.start + i], f));
        }
        chk(fprintf(f, "\n"));
        return 1;
chk_error:
        return 0;
}

int mliJson_debug_fprint(FILE *f, const struct mliJson *json)
{
        uint64_t i;
        for (i = 0; i < json->num_tokens; i++) {
                chk_msg(mliJson_debug_token_fprint(f, json, i),
                        "Failed to write json-token debug-info to file.");
        }
        return 1;
chk_error:
        return 0;
}

int mliJson_debug_to_path(const struct mliJson *json, const char *path)
{
        FILE *f;
        f = fopen(path, "wt");
        chk_msg(f != NULL, "Failed to open file for Json debug output.");
        chk_msg(mliJson_debug_fprint(f, json), "Failed to fprint debug.");
        fclose(f);
        return 1;
chk_error:
        fclose(f);
        return 0;
}

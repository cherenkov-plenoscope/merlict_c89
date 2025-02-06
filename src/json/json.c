/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "json.h"
#include <stdlib.h>
#include "json_jsmn.h"
#include "../cstr/cstr.h"
#include "../cstr/cstr_numbers.h"
#include "../math/math.h"
#include "../bool/bool.h"
#include "../chk/chk.h"
#include "../io/io_text.h"

struct mli_Json mli_Json_init(void)
{
        struct mli_Json j;
        j.raw = mli_String_init();
        j.num_tokens = 0u;
        j.tokens = NULL;
        return j;
}

void mli_Json_free(struct mli_Json *json)
{
        mli_String_free(&json->raw);
        free(json->tokens);
        (*json) = mli_Json_init();
}

chk_rc mli_Json__malloc_tokens(struct mli_Json *json)
{
        struct jsmntok_t default_token = {JSMN_UNDEFINED, 0, 0, 0};
        chk_msg(&json->raw.array != NULL, "Expected raw cstr to be malloced.");
        json->num_tokens = json->raw.size / 2;
        chk_malloc(json->tokens, struct jsmntok_t, json->num_tokens);
        MLI_MATH_ARRAY_SET(json->tokens, default_token, json->num_tokens);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json__parse_tokens(struct mli_Json *json)
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
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_from_string(struct mli_Json *self, const struct mli_String *str)
{
        mli_Json_free(self);
        chk_msg(mli_String_copy(&self->raw, str),
                "Failed to copy string into json->raw.");
        chk_msg(mli_Json__malloc_tokens(self), "Can't malloc Json's tokens.");
        chk_msg(mli_Json__parse_tokens(self), "Can't parse Json into tokens.");
        return 1;
chk_error:
        mli_Json_free(self);
        return 0;
}

chk_rc mli_Json_from_io(struct mli_Json *self, struct mli_IO *io)
{
        mli_Json_free(self);
        chk_msg(mli_IO_text_read_string(io, &self->raw),
                "Failed to read file into String.");
        chk_msg(mli_Json__malloc_tokens(self), "Can't malloc Json's tokens.");
        chk_msg(mli_Json__parse_tokens(self), "Can't parse Json into tokens.");
        return 1;
chk_error:
        mli_Json_free(self);
        return 0;
}

chk_rc mli_Json_cstr_by_token(
        const struct mli_Json *json,
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
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_string_by_token(
        const struct mli_Json *json,
        const uint64_t token,
        struct mli_String *return_string)
{
        const struct jsmntok_t t = json->tokens[token];
        const uint64_t size = t.end - t.start;
        chk(mli_String_malloc(return_string, size));
        memcpy(return_string->array, json->raw.array + t.start, size);
        return_string->size = size;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_int64_by_token(
        const struct mli_Json *json,
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
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_uint64_by_token(
        const struct mli_Json *json,
        const uint64_t token,
        uint64_t *val)
{
        int64_t tmp;
        chk(mli_Json_int64_by_token(json, token, &tmp));
        chk_msg(tmp >= 0, "Expected value to be unsigned.");
        (*val) = (uint64_t)tmp;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_int64_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        int64_t *val,
        const char *key)
{
        uint64_t token_n;
        chk(mli_Json_token_by_key_eprint(json, token, key, &token_n));
        chk_msgf(
                mli_Json_int64_by_token(json, token_n + 1, val),
                ("Can't parse value of '%s' into int64.", key));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_uint64_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        uint64_t *val,
        const char *key)
{
        int64_t tmp;
        chk(mli_Json_int64_by_key(json, token, &tmp, key));
        chk_msg(tmp >= 0, "Expected value to be unsigned.");
        (*val) = (uint64_t)tmp;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_double_by_token(
        const struct mli_Json *json,
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
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_double_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        double *val,
        const char *key)
{
        uint64_t token_n;
        chk(mli_Json_token_by_key_eprint(json, token, key, &token_n));
        chk_msgf(
                mli_Json_double_by_token(json, token_n + 1, val),
                ("Can't parse value of '%s' into double.", key));

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_Json_cstrcmp(
        const struct mli_Json *json,
        const uint64_t token,
        const char *str)
{
        uint64_t i;
        const struct jsmntok_t t = json->tokens[token];
        const uint64_t token_length = t.end - t.start;
        const uint64_t str_length = strlen(str);

        if (token_length != str_length) {
                return MLI_FALSE;
        }
        for (i = 0; i < token_length; i++) {
                const char token_char = (char)json->raw.array[t.start + i];
                const char str_char = str[i];
                if (token_char != str_char) {
                        return MLI_FALSE;
                }
        }
        return MLI_TRUE;
}

chk_rc mli_Json_token_by_key(
        const struct mli_Json *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token)
{
        int64_t num_found = 0;
        int64_t child = 0;
        int64_t subchild_balance = 0;
        int64_t idx = token + 1;

        while (child < json->tokens[token].size) {
                if (mli_Json_cstrcmp(json, idx, key)) {
                        (*key_token) = idx;
                        num_found += 1;
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
        if (num_found > 0) {
                return CHK_SUCCESS;
        } else {
                return CHK_FAIL;
        }
}

chk_rc mli_Json_token_by_key_eprint(
        const struct mli_Json *json,
        const uint64_t token,
        const char *key,
        uint64_t *key_token)
{
        chk_msgf(
                mli_Json_token_by_key(json, token, key, key_token),
                ("Expected key '%s' in json.", key));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

uint64_t mli_Json__token_by_index_unsafe(
        const struct mli_Json *json,
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

chk_rc mli_Json_token_by_idx(
        const struct mli_Json *json,
        const uint64_t token,
        const uint64_t idx,
        uint64_t *idx_token)
{
        chk_msgf(
                (int64_t)idx < json->tokens[token].size,
                ("Array idx '%lu' is out of range for size '%lu'.",
                 token,
                 json->tokens[token].size));
        (*idx_token) = mli_Json__token_by_index_unsafe(json, token, idx);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_debug_token_fprint(
        FILE *f,
        const struct mli_Json *self,
        const uint64_t token)
{
        struct mli_IO io = mli_IO_init();
        mli_IO_adopt_file(&io, f);
        chk(mli_Json_debug_token_to_io(self, token, &io));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_debug_token_to_io(
        const struct mli_Json *self,
        const uint64_t token,
        struct mli_IO *io)
{
        uint64_t i = 0u;
        struct jsmntok_t t = self->tokens[token];
        uint32_t token_size = t.end - t.start;
        uint64_t line_number =
                1u + mli_String_countn(&self->raw, '\n', t.start);

        chk(mli_IO_text_write_cstr_format(
                io, "line: %u, ", (uint32_t)line_number));
        chk(mli_IO_text_write_cstr_format(io, "token: %u, ", (uint32_t)token));
        chk(mli_IO_text_write_cstr_format(io, "type: %d, ", t.type));
        chk(mli_IO_text_write_cstr_format(io, "children: %d, ", t.size));
        chk(mli_IO_text_write_cstr_format(
                io, "chars: (%d -> %d, %d)\n", t.start, t.end, token_size));

        for (i = 0; i < token_size; i++) {
                chk(mli_IO_text_putc(io, self->raw.array[t.start + i]));
        }
        chk(mli_IO_text_putc(io, '\n'));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Json_debug_to_io(const struct mli_Json *self, struct mli_IO *io)
{
        uint64_t i;
        for (i = 0; i < self->num_tokens; i++) {
                chk_msg(mli_Json_debug_token_to_io(self, i, io),
                        "Failed to write json-token debug-info to io.");
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

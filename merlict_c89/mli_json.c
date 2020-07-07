/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_json.h"

/* Copyright 2019 Sebastian Achim Mueller */

struct mliJson mliJson_init(void)
{
        struct mliJson j;
        j.num_chars = 0u;
        j.chars = NULL;
        j.num_tokens = 0u;
        j.tokens = NULL;
        return j;
}

void mliJson_free(struct mliJson *json)
{
        free(json->chars);
        free(json->tokens);
        (*json) = mliJson_init();
}

void _mliJson_set_zero(struct mliJson *json)
{
        uint64_t i;
        for (i = 0; i < json->num_chars; i++) {
                json->chars[i] = '\0';
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
        json->num_chars = json_strlen + 1u;     /* NULL termination. */
        json->num_tokens = json->num_chars / 4; /* A rather safe guess. */
        mli_malloc(json->chars, char, json->num_chars);
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
        strcpy(json->chars, json_str);
        json->chars[json_strlen] = '\0';
        jsmn_init(&parser);
        num_tokens_parsed = jsmn_parse(
                &parser,
                json->chars,
                json->num_chars,
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

int mliJson_malloc_from_file(struct mliJson *json, const char *path)
{
        char *json_str = NULL;
        uint64_t num_chars = 0u;
        uint64_t num_chars_and_null = 0u;
        FILE *f = fopen(path, "rt");
        mli_check(f != NULL, "Can not read json from path.");
        mli_check(fseek(f, 0, SEEK_END) == 0, "Can not seek to end of file.");
        num_chars = ftell(f);
        num_chars_and_null = num_chars + 1u;
        mli_check(fseek(f, 0, SEEK_SET) == 0, "Can not seek to begin of file");
        mli_malloc(json_str, char, num_chars_and_null);
        mli_fread(json_str, sizeof(char), num_chars, f);
        json_str[num_chars] = '\0';
        fclose(f);
        mli_check(
                mliJson_malloc_from_string(json, json_str),
                "Failed to parse json-string read from path.");
        free(json_str);
        return 1;
error:
        free(json_str);
        if (f != NULL)
                fclose(f);
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
                memcpy(return_string, json->chars + t.start, actual_length);
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
        struct jsmntok_t t = json->tokens[token_idx];
        uint64_t buff_size = t.end - t.start + 1u;
        char *buff = NULL;
        mli_check(
                t.type == JSMN_PRIMITIVE,
                "Json int64 expected json-token-to be JSMN_PRIMITIVE.");
        mli_malloc(buff, char, buff_size);
        mliJson_as_string(json, token_idx, buff, buff_size);
        mli_check(
                mli_string_to_int(return_int64, buff, 10),
                "Can not parse int.");
        free(buff);
        return 1;
error:
        free(buff);
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
        char *buff;
        mli_malloc(buff, char, json->num_chars);
        while (child < json->tokens[start_token_idx].size) {
                mliJson_as_string(json, idx, buff, json->num_chars);
                if (strcmp(buff, key) == 0 && strlen(buff) == strlen(key)) {
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
        free(buff);
        return 1;
error:
        free(buff);
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

int mliJson_fprint_debug(FILE *f, const struct mliJson *json)
{
        uint64_t i;
        char *buff;
        mli_malloc(buff, char, json->num_chars);
        for (i = 0; i < json->num_tokens; i++) {
                struct jsmntok_t t = json->tokens[i];
                fprintf(f, "Token: %lu ", i);
                fprintf(f, "sz: %d ", t.size);
                fprintf(f, "tp: %d ", t.type);
                fprintf(f, "(%d -> %d, %d)\n", t.start, t.end, t.end - t.start);
                mliJson_as_string(json, i, buff, json->num_chars);
                fprintf(f, "%s\n", buff);
        }
        free(buff);
        return 1;
error:
        free(buff);
        return 0;
}

int mliJson_write_debug(const struct mliJson *json, const char *path)
{
        FILE *f;
        f = fopen(path, "wt");
        mli_check(f != NULL, "Failed to open file for Json debug output.");
        mli_check(mliJson_fprint_debug(f, json), "Failed to fprint debug.");
        fclose(f);
        return 1;
error:
        fclose(f);
        return 0;
}

/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_JSON_H_
#define MERLICT_MLI_JSON_H_

#include <stdint.h>
#include <stdlib.h>
#include "jsmn.h"
#include "mli_debug.h"
#include "mli_string_to.h"

typedef struct {
    uint64_t num_chars;
    char* chars;
    uint64_t num_tokens;
    jsmntok_t *tokens;
} mliJson;


mliJson mliJson_init() {
    mliJson j;
    j.num_chars = 0u;
    j.chars = NULL;
    j.num_tokens = 0u;
    j.tokens = NULL;
    return j;}


void mliJson_free(mliJson *json) {
    free(json->chars);
    free(json->tokens);
    (*json) = mliJson_init();}


int mliJson_malloc(mliJson *json) {
    mli_malloc(json->chars, char, json->num_chars);
    mli_malloc(json->tokens, jsmntok_t, json->num_tokens);
    return 1;
error:
    mliJson_free(json);
    return 0;}


int mliJson_malloc_from_file(mliJson *json, const char *path) {
    FILE * f = fopen(path, "rt");
    int64_t num_tokens_parsed;
    jsmn_parser parser;
    mli_check(f != NULL, "Can not read json.");
    fseek(f, 0, SEEK_END);
    json->num_chars = ftell(f);
    json->num_tokens = json->num_chars/3;
    fseek(f, 0, SEEK_SET);
    mli_check_mem(mliJson_malloc(json));
    mli_fread(json->chars, sizeof(char), json->num_chars, f);
    jsmn_init(&parser);
    num_tokens_parsed = jsmn_parse(
        &parser,
        json->chars,
        json->num_chars,
        json->tokens,
        json->num_tokens);
    mli_check(num_tokens_parsed != JSMN_ERROR_NOMEM,
        "Not enough tokens were provided.")
    mli_check(num_tokens_parsed != JSMN_ERROR_INVAL,
        "Invalid character inside JSON string.")
    mli_check(num_tokens_parsed != JSMN_ERROR_PART,
        "The string is not a full JSON packet, more bytes expected.")

    mli_check(num_tokens_parsed >= 0, "Can not parse Json-string");
    json->num_tokens = num_tokens_parsed;
    fclose(f);
    return 1;
error:
    if (f != NULL) {
        fclose(f);}
    return 0;
}


void mliJson_string(
    const mliJson *json,
    const uint64_t token_idx,
    char *return_string) {
    jsmntok_t t = json->tokens[token_idx];
    memcpy(return_string, json->chars + t.start, t.end - t.start);
    return_string[t.end - t.start] = '\0';}


int mliJson_as_int64(
    const mliJson *json,
    const uint64_t token_idx,
    int64_t *return_int64) {
    jsmntok_t t = json->tokens[token_idx];
    uint64_t token_string_size = t.end - t.start;
    char *buff;
    mli_check(
        t.type == JSMN_PRIMITIVE,
        "Json int64 parsing expected json-token-to be JSMN_PRIMITIVE.");
    mli_malloc(buff, char, token_string_size);
    mliJson_string(json, token_idx, buff);
    mli_check(mli_string_to_int(return_int64, buff, 10), "Can not parse int.");
    free(buff);
    return 1;
error:
    free(buff);
    return 0;
}


int mliJson_as_float64(
    const mliJson *json,
    const uint64_t token_idx,
    double *return_float64) {
    jsmntok_t t = json->tokens[token_idx];
    uint64_t token_string_size = t.end - t.start;
    char *buff;
    mli_check(
        t.type == JSMN_PRIMITIVE,
        "Json float64 parsing expected json-token-to be JSMN_PRIMITIVE.");
    mli_malloc(buff, char, token_string_size);
    mliJson_string(json, token_idx, buff);
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
    const mliJson *json,
    const uint64_t start_token_idx,
    const char *key,
    uint64_t *return_idx) {
    int64_t found = 0;
    int64_t child = 0;
    int64_t subchild_balance = 0;
    int64_t idx = start_token_idx + 1;
    char *buff;
    mli_malloc(buff, char, json->num_chars);
    while (child < json->tokens[start_token_idx].size) {
        mliJson_string(json, idx, buff);
        /* fprintf(stderr, "child:%ld idx:%ld str:%s\n", child, idx, buff); */
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
    const mliJson *json,
    const uint64_t start_token_idx,
    const uint64_t child_idx) {
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


int mliJson_fprint_debug(FILE* f, const mliJson *json) {
    uint64_t i;
    char *buff;
    mli_malloc(buff, char, json->num_chars);
    for (i = 0; i < json->num_tokens; i++) {
        jsmntok_t t = json->tokens[i];
        fprintf(f, "Token: %lu ", i);
        fprintf(f, "sz: %d ", t.size);
        fprintf(f, "tp: %d ", t.type);
        fprintf(f, "(%d -> %d, %d)\n", t.start, t.end, t.end - t.start);
        mliJson_string(json, i, buff);
        fprintf(f, "%s\n", buff);
    }
    free(buff);
    return 1;
error:
    free(buff);
    return 0;
}

int mliJson_write_debug(const mliJson *json, const char* path) {
    FILE* f;
    f = fopen(path, "wt");
    mli_check(f != NULL, "Can not open file to write Json debug output.");
    mli_check(mliJson_fprint_debug(f, json), "Can not create debug string.");
    fclose(f);
    return 1;
error:
    fclose(f);
    return 0;
}

#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_JSMN_H_
#define MERLICT_C89_JSMN_H_

#include <stddef.h>

#define JSMN_H

enum jsmntype_t {
        JSMN_UNDEFINED = 0,
        JSMN_OBJECT = 1,
        JSMN_ARRAY = 2,
        JSMN_STRING = 3,
        JSMN_PRIMITIVE = 4
};

enum jsmnerr {
        /* Not enough tokens were provided */
        JSMN_ERROR_NOMEM = -1,
        /* Invalid character inside JSON string */
        JSMN_ERROR_INVAL = -2,
        /* The string is not a full JSON packet, more bytes expected */
        JSMN_ERROR_PART = -3
};

struct jsmntok_t {
        enum jsmntype_t type;
        int start;
        int end;
        int size;
};

struct jsmn_parser {
        unsigned int pos;     /* offset in the JSON string */
        unsigned int toknext; /* next token to allocate */
        int toksuper; /* superior token node, e.g. parent object or array */
};

int jsmn_parse(
        struct jsmn_parser *parser,
        const char *js,
        const size_t len,
        struct jsmntok_t *tokens,
        const unsigned int num_tokens);
int jsmn_parse(
        struct jsmn_parser *parser,
        const char *js,
        const size_t len,
        struct jsmntok_t *tokens,
        const unsigned int num_tokens);
void jsmn_init(struct jsmn_parser *parser);
void jsmn_init(struct jsmn_parser *parser);
#endif

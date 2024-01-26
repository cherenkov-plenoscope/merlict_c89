/*
 * MIT License
 *
 * Copyright (c) 2010 Serge Zaitsev
 *               2018-2020 Sebastian Achim Mueller
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * - removed JSMN_PARENT_LINKS
 * - always JSMN_STRICT
 */

#include "mli_json_jsmn.h"

void jsmn_init(struct jsmn_parser *parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each
 * describing
 * a single JSON object.
 */
int jsmn_parse(
        struct jsmn_parser *parser,
        const char *js,
        const size_t len,
        struct jsmntok_t *tokens,
        const unsigned int num_tokens);

/**
 * Allocates a fresh unused token from the token pool.
 */
static struct jsmntok_t *jsmn_alloc_token(
        struct jsmn_parser *parser,
        struct jsmntok_t *tokens,
        const size_t num_tokens)
{
        struct jsmntok_t *tok;
        if (parser->toknext >= num_tokens) {
                return NULL;
        }
        tok = &tokens[parser->toknext++];
        tok->start = tok->end = -1;
        tok->size = 0;
        return tok;
}

/**
 * Fills token type and boundaries.
 */
static void jsmn_fill_token(
        struct jsmntok_t *token,
        const enum jsmntype_t type,
        const int start,
        const int end)
{
        token->type = type;
        token->start = start;
        token->end = end;
        token->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static int jsmn_parse_primitive(
        struct jsmn_parser *parser,
        const char *js,
        const size_t len,
        struct jsmntok_t *tokens,
        const size_t num_tokens)
{
        struct jsmntok_t *token;
        int start;

        start = parser->pos;

        for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
                switch (js[parser->pos]) {
                case '\t':
                case '\r':
                case '\n':
                case ' ':
                case ',':
                case ']':
                case '}':
                        goto found;
                }
                if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
                        parser->pos = start;
                        return JSMN_ERROR_INVAL;
                }
        }

        /* In strict mode primitive must be followed by a comma/object/array */
        parser->pos = start;
        return JSMN_ERROR_PART;

found:
        if (tokens == NULL) {
                parser->pos--;
                return 0;
        }
        token = jsmn_alloc_token(parser, tokens, num_tokens);
        if (token == NULL) {
                parser->pos = start;
                return JSMN_ERROR_NOMEM;
        }
        jsmn_fill_token(token, JSMN_PRIMITIVE, start, parser->pos);
        parser->pos--;
        return 0;
}

/**
 * Fills next token with JSON string.
 */
static int jsmn_parse_string(
        struct jsmn_parser *parser,
        const char *js,
        const size_t len,
        struct jsmntok_t *tokens,
        const size_t num_tokens)
{
        struct jsmntok_t *token;

        int start = parser->pos;

        parser->pos++;

        /* Skip starting quote */
        for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
                char c = js[parser->pos];

                /* Quote: end of string */
                if (c == '\"') {
                        if (tokens == NULL) {
                                return 0;
                        }
                        token = jsmn_alloc_token(parser, tokens, num_tokens);
                        if (token == NULL) {
                                parser->pos = start;
                                return JSMN_ERROR_NOMEM;
                        }
                        jsmn_fill_token(
                                token, JSMN_STRING, start + 1, parser->pos);
                        return 0;
                }

                /* Backslash: Quoted symbol expected */
                if (c == '\\' && parser->pos + 1 < len) {
                        int i;
                        parser->pos++;
                        switch (js[parser->pos]) {
                        /* Allowed escaped symbols */
                        case '\"':
                        case '/':
                        case '\\':
                        case 'b':
                        case 'f':
                        case 'r':
                        case 'n':
                        case 't':
                                break;
                        /* Allows escaped symbol \uXXXX */
                        case 'u':
                                parser->pos++;
                                for (i = 0; i < 4 && parser->pos < len &&
                                            js[parser->pos] != '\0';
                                     i++) {
                                        /* If it isn't a hex character we have
                                         * an error */
                                        if (!((js[parser->pos] >= 48 &&
                                               js[parser->pos] <=
                                                       57) || /* 0-9 */
                                              (js[parser->pos] >= 65 &&
                                               js[parser->pos] <=
                                                       70) || /* A-F */
                                              (js[parser->pos] >= 97 &&
                                               js[parser->pos] <=
                                                       102))) { /* a-f */
                                                parser->pos = start;
                                                return JSMN_ERROR_INVAL;
                                        }
                                        parser->pos++;
                                }
                                parser->pos--;
                                break;
                        /* Unexpected symbol */
                        default:
                                parser->pos = start;
                                return JSMN_ERROR_INVAL;
                        }
                }
        }
        parser->pos = start;
        return JSMN_ERROR_PART;
}

/**
 * Parse JSON string and fill tokens.
 */
int jsmn_parse(
        struct jsmn_parser *parser,
        const char *js,
        const size_t len,
        struct jsmntok_t *tokens,
        const unsigned int num_tokens)
{
        int r;
        int i;
        struct jsmntok_t *token;
        int count = parser->toknext;

        for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
                char c;
                enum jsmntype_t type;

                c = js[parser->pos];
                switch (c) {
                case '{':
                case '[':
                        count++;
                        if (tokens == NULL) {
                                break;
                        }
                        token = jsmn_alloc_token(parser, tokens, num_tokens);
                        if (token == NULL) {
                                return JSMN_ERROR_NOMEM;
                        }
                        if (parser->toksuper != -1) {
                                struct jsmntok_t *t = &tokens[parser->toksuper];

                                /* In strict mode an object or array can't
                                 * become a key */
                                if (t->type == JSMN_OBJECT) {
                                        return JSMN_ERROR_INVAL;
                                }

                                t->size++;
                        }
                        token->type = (c == '{' ? JSMN_OBJECT : JSMN_ARRAY);
                        token->start = parser->pos;
                        parser->toksuper = parser->toknext - 1;
                        break;
                case '}':
                case ']':
                        if (tokens == NULL) {
                                break;
                        }
                        type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
                        for (i = parser->toknext - 1; i >= 0; i--) {
                                token = &tokens[i];
                                if (token->start != -1 && token->end == -1) {
                                        if (token->type != type) {
                                                return JSMN_ERROR_INVAL;
                                        }
                                        parser->toksuper = -1;
                                        token->end = parser->pos + 1;
                                        break;
                                }
                        }
                        /* Error if unmatched closing bracket */
                        if (i == -1) {
                                return JSMN_ERROR_INVAL;
                        }
                        for (; i >= 0; i--) {
                                token = &tokens[i];
                                if (token->start != -1 && token->end == -1) {
                                        parser->toksuper = i;
                                        break;
                                }
                        }
                        break;
                case '\"':
                        r = jsmn_parse_string(
                                parser, js, len, tokens, num_tokens);
                        if (r < 0) {
                                return r;
                        }
                        count++;
                        if (parser->toksuper != -1 && tokens != NULL) {
                                tokens[parser->toksuper].size++;
                        }
                        break;
                case '\t':
                case '\r':
                case '\n':
                case ' ':
                        break;
                case ':':
                        parser->toksuper = parser->toknext - 1;
                        break;
                case ',':
                        if (tokens != NULL && parser->toksuper != -1 &&
                            tokens[parser->toksuper].type != JSMN_ARRAY &&
                            tokens[parser->toksuper].type != JSMN_OBJECT) {
                                for (i = parser->toknext - 1; i >= 0; i--) {
                                        if (tokens[i].type == JSMN_ARRAY ||
                                            tokens[i].type == JSMN_OBJECT) {
                                                if (tokens[i].start != -1 &&
                                                    tokens[i].end == -1) {
                                                        parser->toksuper = i;
                                                        break;
                                                }
                                        }
                                }
                        }
                        break;

                /* In strict mode primitives are: numbers and booleans */
                case '-':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 't':
                case 'f':
                case 'n':
                        /* And they must not be keys of the object */
                        if (tokens != NULL && parser->toksuper != -1) {
                                const struct jsmntok_t *t =
                                        &tokens[parser->toksuper];
                                if (t->type == JSMN_OBJECT ||
                                    (t->type == JSMN_STRING && t->size != 0)) {
                                        return JSMN_ERROR_INVAL;
                                }
                        }

                        r = jsmn_parse_primitive(
                                parser, js, len, tokens, num_tokens);
                        if (r < 0) {
                                return r;
                        }
                        count++;
                        if (parser->toksuper != -1 && tokens != NULL) {
                                tokens[parser->toksuper].size++;
                        }
                        break;

                /* Unexpected char in strict mode */
                default:
                        return JSMN_ERROR_INVAL;
                }
        }

        if (tokens != NULL) {
                for (i = parser->toknext - 1; i >= 0; i--) {
                        /* Unmatched opened object or array */
                        if (tokens[i].start != -1 && tokens[i].end == -1) {
                                return JSMN_ERROR_PART;
                        }
                }
        }

        return count;
}

/**
 * Creates a new parser based over a given buffer with an array of tokens
 * available.
 */
void jsmn_init(struct jsmn_parser *parser)
{
        parser->pos = 0;
        parser->toknext = 0;
        parser->toksuper = -1;
}

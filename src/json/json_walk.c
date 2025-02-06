/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "json_walk.h"
#include <stdarg.h>
#include "../chk/chk.h"

struct mli_JsonWalk mli_JsonWalk_init(void)
{
        struct mli_JsonWalk out;
        out.json = NULL;
        out.token = 0;
        return out;
}

struct mli_JsonWalk mli_JsonWalk_set(const struct mli_Json *json)
{
        struct mli_JsonWalk out;
        out.json = json;
        out.token = 0;
        return out;
}

struct mli_JsonWalk mli_JsonWalk_copy(const struct mli_JsonWalk *self)
{
        struct mli_JsonWalk out;
        out.json = self->json;
        out.token = self->token;
        return out;
}

int mli_JsonWalk__type(const struct mli_JsonWalk *self)
{
        return self->json->tokens[self->token].type;
}

int mli_JsonWalk_to_key(struct mli_JsonWalk *self, const char *key)
{
        uint64_t key_token;
        chk_msg(mli_JsonWalk__type(self) == JSMN_OBJECT,
                "Can only go to key-value in json-object.");
        chk_msgf(
                mli_Json_token_by_key(self->json, self->token, key, &key_token),
                ("No key '%s'", key));
        self->token = key_token + 1;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_JsonWalk_to_idx(struct mli_JsonWalk *self, const uint64_t idx)
{
        uint64_t idx_token;
        chk_msg(mli_JsonWalk__type(self) == JSMN_ARRAY,
                "Can only go to idx-value in json-array.");
        chk_msgf(
                mli_Json_token_by_idx(self->json, self->token, idx, &idx_token),
                ("No index '%lu'", idx));
        self->token = idx_token;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

void mli_JsonWalk_to_root(struct mli_JsonWalk *self) { self->token = 0; }

int mli_JsonWalk_get_array_size(const struct mli_JsonWalk *self, uint64_t *size)
{
        chk_msg(mli_JsonWalk__type(self) == JSMN_ARRAY,
                "Can only get size of json-array.");
        (*size) = self->json->tokens[self->token].size;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_JsonWalk_get_string(
        const struct mli_JsonWalk *self,
        struct mli_String *val)
{
        return mli_Json_string_by_token(self->json, self->token, val);
}

int mli_JsonWalk_get_int64(const struct mli_JsonWalk *self, int64_t *val)
{
        return mli_Json_int64_by_token(self->json, self->token, val);
}

int mli_JsonWalk_get_double(const struct mli_JsonWalk *self, double *val)
{
        return mli_Json_double_by_token(self->json, self->token, val);
}

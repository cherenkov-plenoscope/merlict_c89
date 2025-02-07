/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_JSON_WALK_H_
#define MLI_JSON_WALK_H_

#include <stdint.h>
#include <stdarg.h>
#include "json.h"
#include "../chk/chk.h"
#include "../string/string.h"

struct mli_JsonWalk {
        const struct mli_Json *json;
        uint64_t token;
};

struct mli_JsonWalk mli_JsonWalk_init(void);
struct mli_JsonWalk mli_JsonWalk_set(const struct mli_Json *json);
struct mli_JsonWalk mli_JsonWalk_copy(const struct mli_JsonWalk *self);

chk_rc mli_JsonWalk_to_key(struct mli_JsonWalk *self, const char *key);
chk_rc mli_JsonWalk_to_idx(struct mli_JsonWalk *self, const uint64_t idx);
void mli_JsonWalk_to_root(struct mli_JsonWalk *self);

chk_rc mli_JsonWalk_get_array_size(
        const struct mli_JsonWalk *self,
        uint64_t *size);
chk_rc mli_JsonWalk_get_string(
        const struct mli_JsonWalk *self,
        struct mli_String *val);
chk_rc mli_JsonWalk_get_int64(const struct mli_JsonWalk *self, int64_t *val);
chk_rc mli_JsonWalk_get_double(const struct mli_JsonWalk *self, double *val);

enum jsmntype_t mli_JsonWalk__type(const struct mli_JsonWalk *self);

#endif

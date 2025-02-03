/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FUNC_INFO_H_
#define MLI_FUNC_INFO_H_

#include "../string/string.h"
#include "../io/io.h"

struct mli_FuncInfo {
        struct mli_String x;
        struct mli_String y;
};

struct mli_FuncInfo mli_FuncInfo_init(void);
void mli_FuncInfo_free(struct mli_FuncInfo *self);
int mli_FuncInfo_malloc(struct mli_FuncInfo *self);

int mli_FuncInfo_equal(
        const struct mli_FuncInfo *a,
        const struct mli_FuncInfo *b);
int mli_FuncInfo_to_io(const struct mli_FuncInfo *self, struct mli_IO *f);
int mli_FuncInfo_from_io(struct mli_FuncInfo *self, struct mli_IO *f);
#endif

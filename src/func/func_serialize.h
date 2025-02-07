/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FUNC_SERIALIZE_H_
#define MLI_FUNC_SERIALIZE_H_

#include "../chk/chk.h"
#include "../io/io.h"
#include "../func/func.h"

chk_rc mli_Func_from_io(struct mli_Func *func, struct mli_IO *f);
chk_rc mli_Func_to_io(const struct mli_Func *func, struct mli_IO *f);
#endif

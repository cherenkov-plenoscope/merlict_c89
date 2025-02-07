/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FUNC_CSV_H_
#define MLI_FUNC_CSV_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../func/func.h"
#include "../io/io.h"
#include "../string/string.h"

chk_rc mli_Func_from_csv(
        struct mli_Func *func,
        struct mli_String *xname,
        struct mli_String *yname,
        struct mli_IO *io);

#endif

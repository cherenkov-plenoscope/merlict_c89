/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SPECTRUM_H_
#define MLI_SPECTRUM_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../bool/bool.h"
#include "../func/func.h"
#include "../func/func_info.h"
#include "../string/string.h"

struct mli_Spectrum {
        struct mli_Func spectrum;
        struct mli_FuncInfo info;
        struct mli_String name;
};
void mli_Spectrum_free(struct mli_Spectrum *self);
struct mli_Spectrum mli_Spectrum_init(void);
mli_bool mli_Spectrum_equal(
        const struct mli_Spectrum *a,
        const struct mli_Spectrum *b);

chk_rc mli_Spectrum_to_io(const struct mli_Spectrum *self, struct mli_IO *f);
chk_rc mli_Spectrum_from_io(struct mli_Spectrum *self, struct mli_IO *f);

chk_rc mli_Spectrum_print_to_io(
        const struct mli_Spectrum *self,
        struct mli_IO *f);

#endif

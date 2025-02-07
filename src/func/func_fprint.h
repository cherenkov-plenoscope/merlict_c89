/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FUNC_plot_H_
#define MLI_FUNC_plot_H_

#include <stdint.h>
#include <stdio.h>
#include "../chk/chk.h"
#include "../func/func.h"

struct mli_Func_fprint_Config {
        double x_start;
        double x_stop;
        int x_num;
        double y_start;
        double y_stop;
        int y_num;
};

chk_rc mli_Func_fprint(
        FILE *f,
        const struct mli_Func *func,
        struct mli_Func_fprint_Config plot);

#endif

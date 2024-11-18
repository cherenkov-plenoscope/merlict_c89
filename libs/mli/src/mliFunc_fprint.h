/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIFUNC_plot_H_
#define MLIFUNC_plot_H_

#include <stdint.h>
#include <stdio.h>
#include "mliFunc.h"

struct mliFuncPlot {
        double x_start;
        double x_stop;
        int x_num;
        double y_start;
        double y_stop;
        int y_num;
};

int mliFunc_fprint(
        FILE *f,
        const struct mliFunc *func,
        struct mliFuncPlot plot);

#endif

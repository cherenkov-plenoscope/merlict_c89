/* Copyright 2018-2020 Sebastian Achim Mueller */
#include <stdint.h>
#include <stdio.h>
#include "mliFunc_fprint.h"

int mliFunc_fprint(FILE *f, const struct mliFunc *func, struct mliFuncPlot plot)
{
        int iy, ix;
        const double x_range = plot.x_stop - plot.x_start;
        const double y_range = plot.y_stop - plot.y_start;
        const double x_step = x_range / plot.x_num;
        const double y_step = y_range / plot.y_num;

        chk(mliFunc_is_valid(func));

        for (iy = 0; iy < plot.y_num; iy++) {
                if (iy == 0) {
                        fprintf(f, "  %-10.2e -|", plot.y_stop);
                } else if (iy == plot.y_num - 1) {
                        fprintf(f, "  %-10.2e -|", plot.y_start);
                } else {
                        fprintf(f, "    %-10s|", "");
                }

                for (ix = 0; ix < plot.x_num; ix++) {
                        double x, y;
                        int jy;
                        x = plot.x_start + x_step * ix;
                        if (mliFunc_in_range(func, x)) {
                                chk(mliFunc_evaluate(func, x, &y));
                                jy = (y - plot.y_start) / y_step;
                                if (iy == jy) {
                                        fprintf(f, "x");
                                } else {
                                        fprintf(f, " ");
                                }
                        }
                }
                fprintf(f, "\n");
        }
        fprintf(f, "    %-10s+", "");
        for (ix = 0; ix < plot.x_num; ix++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        fprintf(f, "          ");
        fprintf(f, "%-8.2e ", plot.x_start);

        for (ix = 0; ix < plot.x_num - 8; ix++) {
                fprintf(f, " ");
        }
        fprintf(f, "%-8.2e ", plot.x_stop);
        fprintf(f, "\n");
        return 1;
chk_error:
        return 0;
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "mliFunc_fprint.h"
#include "../../chk/src/chk.h"

int mliFunc_fprint(
        FILE *f,
        const struct mliFunc *func,
        struct mliFunc_fprint_Config cfg)
{
        int iy, ix;
        const double x_range = cfg.x_stop - cfg.x_start;
        const double y_range = cfg.y_stop - cfg.y_start;
        const double x_step = x_range / cfg.x_num;
        const double y_step = y_range / cfg.y_num;

        const int NUM_Y_SUB_STEPS = 3;
        chk(mliFunc_is_valid(func));

        for (iy = cfg.y_num - 1; iy >= 0; iy--) {
                if (iy == cfg.y_num - 1) {
                        fprintf(f, "  %-10.2e -|", cfg.y_stop);
                } else {
                        fprintf(f, "    %-10s|", "");
                }

                for (ix = 0; ix < cfg.x_num; ix++) {
                        double x, y;
                        double jy;
                        int jy_sub;
                        x = cfg.x_start + x_step * ix;
                        if (mliFunc_in_range(func, x)) {
                                chk(mliFunc_evaluate(func, x, &y));
                                jy = (y - cfg.y_start) / y_step;
                                jy_sub =
                                        (int)(NUM_Y_SUB_STEPS *
                                              (jy - floor(jy)));
                                if (iy == (int)(jy)) {
                                        switch (jy_sub) {
                                        case 0:
                                                fprintf(f, ",");
                                                break;
                                        case 1:
                                                fprintf(f, "-");
                                                break;
                                        case 2:
                                                fprintf(f, "'");
                                                break;
                                        default:
                                                fprintf(f, "?");
                                                break;
                                        }
                                } else {
                                        fprintf(f, " ");
                                }
                        }
                }
                fprintf(f, "\n");
        }
        fprintf(f, "  %-10.2e  +", cfg.y_start);
        for (ix = 0; ix < cfg.x_num; ix++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        fprintf(f, "          ");
        fprintf(f, "%-8.2e ", cfg.x_start);

        for (ix = 0; ix < cfg.x_num - 8; ix++) {
                fprintf(f, " ");
        }
        fprintf(f, "%-8.2e ", cfg.x_stop);
        fprintf(f, "\n");
        return 1;
chk_error:
        return 0;
}

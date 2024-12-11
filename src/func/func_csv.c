/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "func_csv.h"
#include "../mli/mliDynDouble.h"
#include "../cstr/cstr.h"
#include "../cstr/cstr_numbers.h"
#include "../chk/chk.h"
#include "../math/math.h"

int mli_Func_from_csv_split_line(
        struct mli_String *line,
        struct mli_String *sx,
        struct mli_String *sy)
{
        size_t i;
        int num_commas = 0;
        chk(mli_String_malloc(sx, 16));
        chk(mli_String_malloc(sy, 16));
        for (i = 0; i < line->size; i++) {
                char c = line->array[i];
                if (c == ',') {
                        if (num_commas > 0) {
                                return 1;
                        }
                        num_commas += 1;
                } else {
                        if (num_commas > 0) {
                                chk(mli_String_push_back(sy, c));
                        } else {
                                chk(mli_String_push_back(sx, c));
                        }
                }
        }
        return 1;
chk_error:
        return 0;
}

int mli_Func_from_csv(
        struct mli_Func *func,
        struct mli_String *xname,
        struct mli_String *yname,
        struct mli_IO *io)
{
        uint64_t line_number = 0u;
        double _x, _y;
        struct mli_String line = mli_String_init();
        struct mli_String sx = mli_String_init();
        struct mli_String sy = mli_String_init();
        struct mliDynDouble x = mliDynDouble_init();
        struct mliDynDouble y = mliDynDouble_init();

        chk(mliDynDouble_malloc(&x, 0u));
        chk(mliDynDouble_malloc(&y, 0u));

        mli_Func_free(func);
        while (1) {
                line_number += 1;
                chk_msg(line_number < 1000 * 1000 * 1000,
                        "Expected less than 1e9 lines in csv file. "
                        "Something went wrong.");

                if (mli_IO_eof(io)) {
                        break;
                }
                chk_msg(mli_IO_text_read_line(io, &line, '\n'),
                        "Can not read line.");
                chk_msg(line.size < 4096, "Expected line.size < 4096.");

                if (line.size > 0) {
                        chk_msg(mli_Func_from_csv_split_line(&line, &sx, &sy),
                                "Can not split line into tokens.");
                        if (line_number == 1) {
                                chk(mli_String_copy(xname, &sx));
                                chk(mli_String_copy(yname, &sy));
                        } else {
                                chk(mli_String_to_double(&_x, &sx));
                                chk(mli_String_to_double(&_y, &sy));
                                chk(mliDynDouble_push_back(&x, _x));
                                chk(mliDynDouble_push_back(&y, _y));
                        }
                }
        }

        chk_msg(x.size == y.size, "Expected same number x, y values.");
        chk_msg(mli_Func_malloc(func, x.size),
                "Failed to malloc mli_Func from file.");

        MLI_MATH_NCPY(x.array, func->x, x.size);
        MLI_MATH_NCPY(y.array, func->y, y.size);

        mli_String_free(&line);
        mli_String_free(&sx);
        mli_String_free(&sy);
        mliDynDouble_free(&x);
        mliDynDouble_free(&y);
        return 1;
chk_error:
        mli_String_free(&line);
        mli_String_free(&sx);
        mli_String_free(&sy);
        mliDynDouble_free(&x);
        mliDynDouble_free(&y);

        mli_String_free(xname);
        mli_String_free(yname);
        mli_Func_free(func);
        return 0;
}

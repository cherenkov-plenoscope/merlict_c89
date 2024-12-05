/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFunc_csv.h"
#include "mliName.h"
#include "mliDynDouble.h"
#include "../cstr/cstr.h"
#include "../cstr/cstr_numbers.h"
#include "../chk/chk.h"
#include "../math/math.h"

int mliFunc_malloc_from_csv(struct mliFunc *func, const char *str)
{
        uint64_t i = 0u;
        uint64_t p = 0u;
        uint64_t line_number = 0u;
        uint64_t line_length;
        uint64_t token_length;
        double _x, _y;
        char line[2 * MLI_NAME_CAPACITY];
        char token[MLI_NAME_CAPACITY];
        struct mliDynDouble x = mliDynDouble_init();
        struct mliDynDouble y = mliDynDouble_init();

        memset(line, '\0', sizeof(line));
        memset(token, '\0', sizeof(token));
        chk(mliDynDouble_malloc(&x, 0u));
        chk(mliDynDouble_malloc(&y, 0u));

        mliFunc_free(func);
        while (1) {
                line_number += 1;
                chk_msg(line_number < 1000 * 1000,
                        "Expected less than 1e6 lines in scv file. "
                        "Something went wrong.");

                line_length = mli_cstr_split(&str[p], '\n', line, sizeof(line));

                chk_msg(line_length < sizeof(line), "Line is too long.");

                if (line_length > 0) {
                        if (mli_cstr_starts_with(line, "#")) {
                                /* comment */
                        } else {
                                i = 0;
                                token_length = mli_cstr_split(
                                        &line[i], ',', token, sizeof(token));
                                chk_msg(token_length > 0,
                                        "Expected x column not to be empty.");
                                chk_msg(mli_cstr_to_double(&_x, token),
                                        "Failed to parse 'x' column.");
                                chk(mliDynDouble_push_back(&x, _x));
                                chk_msg(line[i + token_length] != '\0',
                                        "Expected y column to be present.");
                                i += token_length + 1;
                                token_length = mli_cstr_split(
                                        &line[i], ',', token, sizeof(token));
                                chk_msg(token_length > 0,
                                        "Expected y column not to be empty.");
                                chk_msg(mli_cstr_to_double(&_y, token),
                                        "Failed to parse 'y' column.");
                                chk(mliDynDouble_push_back(&y, _y));
                        }
                }

                if (str[p + line_length] == '\0') {
                        break;
                }
                p += line_length + 1;
        }

        /* copy x y into mliFunc */
        chk_msg(x.size == y.size, "Expected same number x, y values.");
        chk_msg(mliFunc_malloc(func, x.size),
                "Failed to malloc mliFunc from file.");

        MLI_MATH_NCPY(x.array, func->x, x.size);
        MLI_MATH_NCPY(y.array, func->y, y.size);

        mliDynDouble_free(&x);
        mliDynDouble_free(&y);
        return 1;
chk_error:
        mliDynDouble_free(&x);
        mliDynDouble_free(&y);
        mliFunc_free(func);
        return 0;
}

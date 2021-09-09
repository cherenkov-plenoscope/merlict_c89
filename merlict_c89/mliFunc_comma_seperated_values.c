/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFunc_comma_seperated_values.h"
#include "mliDynArray_template.h"

#define MLI_CSV_BUFF_CAPACITY 128

int mliFunc_malloc_from_csv(struct mliFunc *func, const char *str)
{
        return mliFunc_malloc_from_csv_dbg(func, str, 1);
}

int mliFunc_malloc_from_csv_dbg(
        struct mliFunc *func,
        const char *str,
        const int dbg)
{
        char line_delimiter = '\n';
        char token_delimiter = ',';
        char line[MLI_CSV_BUFF_CAPACITY];
        int line_length = 1;
        char token[MLI_CSV_BUFF_CAPACITY];
        int token_length = 1;
        int lpos = 0;
        int tpos = 0;
        int num_tokens;
        uint64_t line_number = 0u;
        const uint64_t debug_line_radius = 5u;
        uint64_t i;

        double _x;
        double _y;

        struct mliDynDouble xs = mliDynDouble_init();
        struct mliDynDouble ys = mliDynDouble_init();
        chk_msg(mliDynDouble_malloc(&xs, 0u), "Failed to malloc xs.");
        chk_msg(mliDynDouble_malloc(&ys, 0u), "Failed to malloc ys.");

        while (1) {
                line_number += 1;
                chk_msg(line_number < 1000 * 1000 * 1000,
                        "Expected less than 1e9 lines in "
                        "comma-seperated-value-file. "
                        "Something went wrong.");

                line_length = mli_string_split(
                        &str[tpos],
                        line_delimiter,
                        line,
                        MLI_CSV_BUFF_CAPACITY);
                chk_msg(line_length < MLI_CSV_BUFF_CAPACITY, "Line too long.");

                if (line_length == 0) {
                        break;
                }
                tpos = tpos + line_length + 1;

                if (line[0] == '#') {
                        continue;
                }

                lpos = 0;
                num_tokens = 0;

                while (1) {
                        token_length = mli_string_split(
                                &line[lpos],
                                token_delimiter,
                                token,
                                MLI_CSV_BUFF_CAPACITY);
                        chk_msg(token_length < MLI_CSV_BUFF_CAPACITY,
                                "Token too long.");
                        if (token_length == 0) {
                                break;
                        }
                        lpos = lpos + token_length + 1;
                        num_tokens++;

                        chk_msg(num_tokens <= 2,
                                "Expected only two tokens per line.");
                        if (num_tokens == 1) {
                                chk_msg(mli_string_to_float(&_x, token),
                                        "Failed to parse x-value.");
                        } else if (num_tokens == 2) {
                                chk_msg(mli_string_to_float(&_y, token),
                                        "Failed to parse y-value.");
                                break;
                        }
                }
                chk_msg(num_tokens == 2, "Expected two tokens per line.");

                chk_msg(mliDynDouble_push_back(&xs, _x), "Failed to grow x.");
                chk_msg(mliDynDouble_push_back(&ys, _y), "Failed to grow y.");
        }

        chk_msg(mliFunc_malloc(func, xs.size), "Failed to malloc func.");
        for (i = 0; i < xs.size; i++) {
                func->x[i] = xs.array[i];
                func->y[i] = ys.array[i];
        }

        chk_msg(mliFunc_is_valid(func),
                "Expected function from csv to be valid.");

        mliDynDouble_free(&ys);
        mliDynDouble_free(&xs);
        return 1;
error:
        mliDynDouble_free(&ys);
        mliDynDouble_free(&xs);
        mliFunc_free(func);

        if (dbg) {
                mli_lines_info_fprint(
                        stderr, str, line_number, debug_line_radius);
        }

        return 0;
}

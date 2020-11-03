/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFunc_comma_seperated_values.h"
#include "mliDynArray_template.h"

int mliFunc_malloc_from_csv(struct mliFunc *func, const char *text)
{
        char line_delimiter = '\n';
        char token_delimiter = ',';
        const int nbuff = 128;
        char *line;
        int line_length = 1;
        char *token;
        int token_length = 1;
        int lpos = 0;
        int tpos = 0;
        int num_tokens;
        uint64_t i;

        double _x;
        double _y;

        struct mliDynDouble xs = mliDynDouble_init();
        struct mliDynDouble ys = mliDynDouble_init();
        mli_check(mliDynDouble_malloc(&xs, 0u), "Failed to malloc xs.");
        mli_check(mliDynDouble_malloc(&ys, 0u), "Failed to malloc ys.");
        mli_malloc(line, char, nbuff);
        mli_malloc(token, char, nbuff);

        while (1) {
                line_length = mli_string_split(
                        &text[tpos],
                        line_delimiter,
                        line,
                        nbuff
                );
                mli_check(line_length < nbuff, "Line too long.");
                if (line_length == 0) {
                        break;
                }
                tpos = tpos + line_length;

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
                                nbuff
                        );
                        mli_check(token_length < nbuff, "Token too long.");
                        if (token_length == 0) {
                                break;
                        }
                        lpos = lpos + token_length;
                        num_tokens ++;

                        mli_check(
                                num_tokens <= 2,
                                "Expected only two tokens per line."
                        );
                        if (num_tokens == 1) {
                                mli_check(
                                        mli_string_to_float(&_x, token),
                                        "Failed to parse x-value."
                                );
                        } else if (num_tokens == 2) {
                                mli_check(
                                        mli_string_to_float(&_y, token),
                                        "Failed to parse y-value."
                                );
                                break;
                        }

                }
                mli_check(num_tokens == 2, "Expected two tokens per line.");

                mli_check(mliDynDouble_push_back(&xs, _x), "Failed to grow x.");
                mli_check(mliDynDouble_push_back(&ys, _y), "Failed to grow y.");
        }

        mli_check(mliFunc_malloc(func, xs.dyn.size), "Failed to malloc func.");
        for (i = 0; i < xs.dyn.size; i ++) {
                func->x[i] = xs.arr[i];
                func->y[i] = ys.arr[i];
        }

        mliDynDouble_free(&ys);
        mliDynDouble_free(&xs);
        free(line);
        free(token);
        return 1;
error:
        mliDynDouble_free(&ys);
        mliDynDouble_free(&xs);
        free(line);
        free(token);
        mliFunc_free(func);
        return 0;
}

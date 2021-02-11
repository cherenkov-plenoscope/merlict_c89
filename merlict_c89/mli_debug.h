/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_DEBUG_H_
#define MERLICT_C89_MLI_DEBUG_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

/*
 *  Based on Zed Shawn's awesome Debug Macros from his book:
 *  Learn C the hard way
 */

#define mli_clean_errno() (errno == 0 ? "None" : strerror(errno))
#define mli_c(A) mli_check(A, "Not expected.")
#define mli_check_mem(A) mli_check((A), "Out of memory.")

#define mli_log_err(M)                                                         \
        fprintf(stderr,                                                        \
                "[ERROR] (%s:%d: errno: %s) " M "\n",                          \
                __FILE__,                                                      \
                __LINE__,                                                      \
                mli_clean_errno())

#define mli_check(A, M)                                                        \
        if (!(A)) {                                                            \
                mli_log_err(M);                                                \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define mli_sentinel(M)                                                        \
        {                                                                      \
                mli_log_err(M);                                                \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define mli_malloc(PTR, TYPE, NUM)                                             \
        {                                                                      \
                PTR = (TYPE *)malloc(NUM * sizeof(TYPE));                      \
                mli_check_mem(PTR);                                            \
        }

#define mli_fwrite(PTR, SIZE_OF_TYPE, NUM, F)                                  \
        {                                                                      \
                const uint64_t num_written =                                   \
                        fwrite(PTR, SIZE_OF_TYPE, NUM, F);                     \
                mli_check(num_written == NUM, "Can not write to file.");       \
        }

#define mli_fread(PTR, SIZE_OF_TYPE, NUM, F)                                   \
        {                                                                      \
                const uint64_t num_read = fread(PTR, SIZE_OF_TYPE, NUM, F);    \
                mli_check(num_read == NUM, "Can not read from file.");         \
        }

#define mli_write_type(TYPE, VALUE, F)                                         \
        {                                                                      \
                TYPE __temp_value = (TYPE)VALUE;                               \
                const uint64_t __num_written =                                 \
                        fwrite(&__temp_value, sizeof(TYPE), 1, F);             \
                mli_check(__num_written == 1, "Can not write type to file.");  \
        }

void mli_eprintf(const char *format, ...);

int MLI_PRINT_LEVEL = 1;

#endif

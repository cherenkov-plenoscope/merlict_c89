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

#define chk_clean_errno() (errno == 0 ? "None" : strerror(errno))
#define chk(A) chk_msg(A, "Not expected.")
#define chk_memory(A) chk_msg((A), "Out of memory.")

#define chk_log_err(M)                                                         \
        fprintf(stderr,                                                        \
                "[ERROR] (%s:%d: errno: %s) " M "\n",                          \
                __FILE__,                                                      \
                __LINE__,                                                      \
                chk_clean_errno())

#define chk_msg(A, M)                                                \
        if (!(A)) {                                                            \
                chk_log_err(M);                                                \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define chk_sentinel(M)                                                        \
        {                                                                      \
                chk_log_err(M);                                                \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define chk_malloc(PTR, TYPE, NUM)                                       \
        {                                                                      \
                PTR = (TYPE *)malloc(NUM * sizeof(TYPE));                      \
                chk_memory(PTR);                                         \
        }

#define chk_fwrite(PTR, SIZE_OF_TYPE, NUM, F)                            \
        {                                                                      \
                const uint64_t num_written =                                   \
                        fwrite(PTR, SIZE_OF_TYPE, NUM, F);                     \
                chk_msg(                                             \
                        num_written == NUM, "Can not write to file.");         \
        }

#define chk_fread(PTR, SIZE_OF_TYPE, NUM, F)                             \
        {                                                                      \
                const uint64_t num_read = fread(PTR, SIZE_OF_TYPE, NUM, F);    \
                chk_msg(num_read == NUM, "Can not read from file."); \
        }

void chk_eprintf(const char *format, ...);

int MLI_PRINT_LEVEL = 1;

#endif

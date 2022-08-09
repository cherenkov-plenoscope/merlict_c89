/* Copyright 2018-2021 Sebastian Achim Mueller */
#ifndef CHK_DEBUG_H_
#define CHK_DEBUG_H_

#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

/*
 *  Based on Zed Shawn's awesome Debug Macros from his book:
 *  Learn C the hard way
 */

#define chk_clean_errno() (errno == 0 ? "None" : strerror(errno))

#define chk_eprintf(M, ...)                                                    \
        fprintf(stderr,                                                        \
                "[ERROR] (%s:%d: errno: %s) " M "\n",                          \
                __FILE__,                                                      \
                __LINE__,                                                      \
                chk_clean_errno(),                                             \
                __VA_ARGS__)

#define chk_eprint(M) chk_eprintf(M "%s", "")

#define chk_msgf(C, MSG, ...)                                                  \
        if (!(C)) {                                                            \
                chk_eprintf(MSG, __VA_ARGS__);                                 \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define chk_msg(C, MSG) chk_msgf(C, MSG "%s", "")

#define chk(C) chk_msg(C, "Not expected.")

#define chk_badf(MSG, ...)                                                     \
        {                                                                      \
                chk_eprintf(MSG, __VA_ARGS__);                                 \
                errno = 0;                                                     \
                goto error;                                                    \
        }

#define chk_bad(MSG) chk_badf(MSG "%s", "")

#define chk_mem(C) chk_msg((C), "Out of memory.")

#define chk_malloc(PTR, TYPE, NUM)                                             \
        {                                                                      \
                PTR = (TYPE *)malloc(NUM * sizeof(TYPE));                      \
                chk_mem(PTR);                                                  \
        }

#define chk_fwrite(PTR, SIZE_OF_TYPE, NUM, F)                                  \
        {                                                                      \
                const uint64_t num_written =                                   \
                        fwrite(PTR, SIZE_OF_TYPE, NUM, F);                     \
                chk_msg(num_written == NUM, "Can not write to file.");         \
        }

#define chk_fread(PTR, SIZE_OF_TYPE, NUM, F)                                   \
        {                                                                      \
                const uint64_t num_read = fread(PTR, SIZE_OF_TYPE, NUM, F);    \
                chk_msg(num_read == NUM, "Can not read from file.");           \
        }

#endif

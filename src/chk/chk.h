/* Copyright 2018-2021 Sebastian Achim Mueller */
#ifndef CHK_DEBUG_H_
#define CHK_DEBUG_H_

#include <stdio.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/*
 *  Based on Zed Shawn's awesome Debug Macros from his book:
 *  Learn C the hard way
 */

enum chk_rc_states { CHK_FAIL = 0, CHK_SUCCESS = 1 };

#define chk_rc int

int chk_eprintf(const char *format, ...);

#define chk_clean_errno() (errno == 0 ? "None" : strerror(errno))

#define chk_eprint_head()                                                      \
        chk_eprintf(                                                           \
                "[ERROR] (%s:%d: errno: %s) ",                                 \
                __FILE__,                                                      \
                __LINE__,                                                      \
                chk_clean_errno())

#define chk_eprint_line(MSG)                                                   \
        {                                                                      \
                chk_eprint_head();                                             \
                chk_eprintf("%s", MSG);                                        \
                chk_eprintf("\n");                                             \
        }

#define chk_msg(C, MSG)                                                        \
        if (!(C)) {                                                            \
                chk_eprint_line(MSG);                                          \
                errno = 0;                                                     \
                goto chk_error;                                                \
        }

#define chk_msgf(C, MSGFMT)                                                    \
        if (!(C)) {                                                            \
                chk_eprint_head();                                             \
                chk_eprintf MSGFMT;                                            \
                chk_eprintf("\n");                                             \
                errno = 0;                                                     \
                goto chk_error;                                                \
        }

#define chk_bad(MSG)                                                           \
        {                                                                      \
                chk_eprint_line(MSG);                                          \
                errno = 0;                                                     \
                goto chk_error;                                                \
        }

#define chk_badf(MSGFMT)                                                       \
        {                                                                      \
                chk_eprint_head();                                             \
                chk_eprintf MSGFMT;                                            \
                chk_eprintf("\n");                                             \
                errno = 0;                                                     \
                goto chk_error;                                                \
        }

#define chk(C) chk_msg(C, "Not expected.")

#define chk_mem(C) chk_msg((C), "Out of memory.")

#define chk_malloc(PTR, TYPE, NUM)                                             \
        {                                                                      \
                PTR = (TYPE *)malloc(NUM * sizeof(TYPE));                      \
                chk_mem(PTR);                                                  \
        }

#define chk_to_io(PTR, SIZE_OF_TYPE, NUM, F)                                   \
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

#define chk_dbg                                                                \
        {                                                                      \
                /*fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);*/           \
        }

#define chk_warning(MSG)                                                       \
        {                                                                      \
                chk_eprintf("[WARNING] (%s:%d) ", __FILE__, __LINE__);         \
                chk_eprintf("%s", MSG);                                        \
                chk_eprintf("\n");                                             \
        }

#endif

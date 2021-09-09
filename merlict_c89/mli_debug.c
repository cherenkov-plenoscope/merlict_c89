/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_debug.h"

void chk_eprintf(const char *format, ...)
{
        va_list args;
        va_start(args, format);
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, format, args);
        va_end(args);
}

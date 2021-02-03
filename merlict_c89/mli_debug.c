/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_debug.h"

void mli_stderr_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stderr, format, args);
    va_end(args);
}

/* Copyright 2018-2021 Sebastian Achim Mueller */
#include "chk_debug.h"
#include <stdarg.h>
#include <stdio.h>

int chk_eprintf(const char *format, ...)
{
        int r;
        va_list args;
        va_start(args, format);
        r = vfprintf(stderr, format, args);
        va_end(args);
        return r;
}

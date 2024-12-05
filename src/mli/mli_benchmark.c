/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "mli_benchmark.h"

double mli_clock2second(const clock_t t)
{
        return ((double)t) / CLOCKS_PER_SEC;
}

/* Copyright 2020 Sebastian A. Mueller*/
#include "mli_corsika_utils.h"

float mli_chars_to_float(const char *four_char_word)
{
        float f;
        memcpy(&f, four_char_word, sizeof(float));
        return f;
}

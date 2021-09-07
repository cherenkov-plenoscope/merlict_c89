/* Copyright 2020 Sebastian A. Mueller*/
#include "utils.h"


float mli_4chars_to_float(const char *four_char_word) {
        float f;
        assert(strlen(four_char_word) == 4);
        memcpy(&f, four_char_word, sizeof(float));
        return f;
}

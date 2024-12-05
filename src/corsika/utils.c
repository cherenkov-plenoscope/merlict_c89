/* Copyright 2020 Sebastian A. Mueller*/
#include "utils.h"
#include <string.h>
#include <math.h>

float mli_chars_to_float(const char *four_char_word)
{
        float f;
        memcpy(&f, four_char_word, sizeof(float));
        return f;
}

double mli_corsika_ux_to_cx(const double ux) { return -ux; }
double mli_corsika_vy_to_cy(const double vy) { return -vy; }
double mli_corsika_wz_to_cz(const double wz) { return -wz; }

double mli_corsika_cx_to_ux(const double cx) { return -cx; }
double mli_corsika_cy_to_vy(const double cy) { return -cy; }
double mli_corsika_cz_to_wz(const double cz) { return -cz; }

double mli_corsika_restore_direction_z_component(const double x, const double y)
{
        return sqrt(1.0 - x * x - y * y);
}

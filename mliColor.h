/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICOLOR_H_
#define MERLICT_MLICOLOR_H_

#include <math.h>
#include <stdint.h>

typedef struct {
    double r;
    double g;
    double b;
} mliColor;

mliColor mliColor_set(const double r, const double g, const double b) {
    mliColor rgb;
    rgb.r = r;
    rgb.g = g;
    rgb.b = b;
    return rgb;}

mliColor mliColor_mix(
    const mliColor a,
    const mliColor b,
    const double refl) {
    mliColor out;
    out.r = (1.f - refl)*a.r + refl * b.r;
    out.g = (1.f - refl)*a.g + refl * b.g;
    out.b = (1.f - refl)*a.b + refl * b.b;
    return out;}

mliColor mliColor_mean(
    const mliColor colors[],
    const uint32_t num_colors) {
    mliColor out = {0., 0., 0.};
    uint32_t i;
    for (i = 0; i < num_colors; i++) {
        out.r = out.r + colors[i].r;
        out.g = out.g + colors[i].g;
        out.b = out.b + colors[i].b;}
    out.r = out.r/num_colors;
    out.g = out.g/num_colors;
    out.b = out.b/num_colors;
    return out;}

mliColor mliColor_truncate_to_uint8(const mliColor color) {
    mliColor out;
    out.r = color.r;
    out.g = color.g;
    out.b = color.b;
    if (out.r > 255.) out.r = 255.;
    if (out.r < 0.) out.r = 0.;
    if (out.g > 255.) out.g = 255.;
    if (out.g < 0.) out.g = 0.;
    if (out.b > 255.) out.b = 255.;
    if (out.b < 0.) out.b = 0.;
    return out;}

int mliColor_is_equal(const mliColor a, const mliColor b) {
    if (a.r != b.r) return 0;
    if (a.g != b.g) return 0;
    if (a.b != b.b) return 0;
    return 1;}

#endif

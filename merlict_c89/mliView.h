/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIVIEW_H_
#define MERLICT_C89_MLIVIEW_H_

#include "mliVec.h"

struct mliView {
        struct mliVec position;
        struct mliVec rotation;
        double field_of_view;
};

struct mliView mliView_look_up_when_possible(
        const struct mliView camin,
        const double rate);
struct mliView mliView_decrease_fov(
        const struct mliView camin,
        const double rate);
struct mliView mliView_increase_fov(
        const struct mliView camin,
        const double rate);
struct mliView mliView_look_down_when_possible(
        const struct mliView camin,
        const double rate);
struct mliView mliView_look_right(
        const struct mliView camin,
        const double rate);
struct mliView mliView_move_up(const struct mliView camin, const double rate);
struct mliView mliView_move_right(
        const struct mliView camin,
        const double rate);
struct mliView mliView_move_forward(
        const struct mliView camin,
        const double rate);
struct mliVec mliView_direction_up(const struct mliView cam);
struct mliVec mliView_direction_right(const struct mliView cam);
struct mliVec mliView_optical_axis(const struct mliView cam);
struct mliHomTraComp mliView_to_HomTraComp(const struct mliView view);
#endif

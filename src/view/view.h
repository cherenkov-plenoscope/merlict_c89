/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VIEW_H_
#define MLI_VIEW_H_

#include "../vec/vec.h"

struct mli_View {
        struct mli_Vec position;
        struct mli_Vec rotation;
        double field_of_view;
};

struct mli_View mli_View_look_up_when_possible(
        const struct mli_View camin,
        const double rate);
struct mli_View mli_View_decrease_fov(
        const struct mli_View camin,
        const double rate);
struct mli_View mli_View_increase_fov(
        const struct mli_View camin,
        const double rate);
struct mli_View mli_View_look_down_when_possible(
        const struct mli_View camin,
        const double rate);
struct mli_View mli_View_look_right(
        const struct mli_View camin,
        const double rate);
struct mli_View mli_View_move_up(
        const struct mli_View camin,
        const double rate);
struct mli_View mli_View_move_right(
        const struct mli_View camin,
        const double rate);
struct mli_View mli_View_move_forward(
        const struct mli_View camin,
        const double rate);
struct mli_Vec mli_View_direction_up(const struct mli_View cam);
struct mli_Vec mli_View_direction_right(const struct mli_View cam);
struct mli_Vec mli_View_optical_axis(const struct mli_View cam);
struct mli_HomTraComp mli_View_to_HomTraComp(const struct mli_View view);
#endif

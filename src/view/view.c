/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "view.h"
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include "../mat/mat.h"
#include "../homtra/homtra.h"
#include "../math/math.h"

struct mli_Vec mli_View_optical_axis(const struct mli_View cam)
{
        struct mli_Mat rotation = mli_Mat_init_tait_bryan(
                cam.rotation.x, cam.rotation.y, cam.rotation.z);
        return mli_transform_orientation(&rotation, mli_Vec_init(0., 0., 1.));
}

struct mli_Vec mli_View_direction_right(const struct mli_View cam)
{
        struct mli_Mat rotation = mli_Mat_init_tait_bryan(
                cam.rotation.x, cam.rotation.y, cam.rotation.z);
        return mli_transform_orientation(&rotation, mli_Vec_init(1., 0., 0.));
}

struct mli_Vec mli_View_direction_up(const struct mli_View cam)
{
        struct mli_Mat rotation = mli_Mat_init_tait_bryan(
                cam.rotation.x, cam.rotation.y, cam.rotation.z);
        return mli_transform_orientation(&rotation, mli_Vec_init(0., 1., 0.));
}

struct mli_View mli_View_move_forward(
        const struct mli_View camin,
        const double rate)
{
        struct mli_View camout = camin;
        struct mli_Vec optical_axis = mli_View_optical_axis(camin);
        camout.position = mli_Vec_add(
                camout.position, mli_Vec_multiply(optical_axis, rate));
        return camout;
}

struct mli_View mli_View_move_right(
        const struct mli_View camin,
        const double rate)
{
        struct mli_View camout = camin;
        struct mli_Vec direction_right = mli_View_direction_right(camout);
        camout.position = mli_Vec_add(
                camout.position, mli_Vec_multiply(direction_right, rate));
        return camout;
}

struct mli_View mli_View_move_up(const struct mli_View camin, const double rate)
{
        struct mli_View camout = camin;
        camout.position.z += rate;
        return camout;
}

struct mli_View mli_View_look_right(
        const struct mli_View camin,
        const double rate)
{
        struct mli_View camout = camin;
        const double diff = camin.field_of_view * rate;
        camout.rotation.z = fmod(camout.rotation.z - diff, (2. * MLI_MATH_PI));
        return camout;
}

struct mli_View mli_View_look_down_when_possible(
        const struct mli_View camin,
        const double rate)
{
        struct mli_View camout = camin;
        const double diff = camin.field_of_view * rate;
        const double next_rotation_x = camout.rotation.x + diff;
        const int fals_forward_over = next_rotation_x > MLI_MATH_PI;
        if (fals_forward_over) {
                camout.rotation.x = MLI_MATH_PI;
        } else {
                camout.rotation.x = next_rotation_x;
        }
        return camout;
}

struct mli_View mli_View_increase_fov(
        const struct mli_View camin,
        const double rate)
{
        struct mli_View camout = camin;
        if (camout.field_of_view * rate > mli_math_deg2rad(170)) {
                camout.field_of_view = mli_math_deg2rad(170);
        } else {
                camout.field_of_view *= rate;
        }
        return camout;
}

struct mli_View mli_View_decrease_fov(
        const struct mli_View camin,
        const double rate)
{
        struct mli_View camout = camin;
        if (camout.field_of_view / rate < mli_math_deg2rad(.1)) {
                camout.field_of_view = mli_math_deg2rad(.1);
        } else {
                camout.field_of_view /= rate;
        }
        return camout;
}

struct mli_View mli_View_look_up_when_possible(
        const struct mli_View camin,
        const double rate)
{
        struct mli_View camout = camin;
        const double diff = -1.0 * camin.field_of_view * rate;
        const double next_rotation_x = camout.rotation.x + diff;
        const int fals_backwards_over = next_rotation_x < 0.0;
        if (fals_backwards_over) {
                camout.rotation.x = 0.0;
        } else {
                camout.rotation.x = next_rotation_x;
        }
        return camout;
}

struct mli_HomTraComp mli_View_to_HomTraComp(const struct mli_View view)
{
        struct mli_HomTraComp view2root_comp;
        view2root_comp.translation = view.position;
        view2root_comp.rotation = mli_Quaternion_set_tait_bryan(
                view.rotation.x, view.rotation.y, view.rotation.z);
        return view2root_comp;
}

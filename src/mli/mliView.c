/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliView.h"
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include "mliMat.h"
#include "mliHomTra.h"
#include "../math/math.h"

struct mli_Vec mliView_optical_axis(const struct mliView cam)
{
        struct mliMat rotation = mliMat_init_tait_bryan(
                cam.rotation.x, cam.rotation.y, cam.rotation.z);
        return mli_transform_orientation(&rotation, mli_Vec_init(0., 0., 1.));
}

struct mli_Vec mliView_direction_right(const struct mliView cam)
{
        struct mliMat rotation = mliMat_init_tait_bryan(
                cam.rotation.x, cam.rotation.y, cam.rotation.z);
        return mli_transform_orientation(&rotation, mli_Vec_init(1., 0., 0.));
}

struct mli_Vec mliView_direction_up(const struct mliView cam)
{
        struct mliMat rotation = mliMat_init_tait_bryan(
                cam.rotation.x, cam.rotation.y, cam.rotation.z);
        return mli_transform_orientation(&rotation, mli_Vec_init(0., 1., 0.));
}

struct mliView mliView_move_forward(
        const struct mliView camin,
        const double rate)
{
        struct mliView camout = camin;
        struct mli_Vec optical_axis = mliView_optical_axis(camin);
        camout.position = mli_Vec_add(
                camout.position, mli_Vec_multiply(optical_axis, rate));
        return camout;
}

struct mliView mliView_move_right(const struct mliView camin, const double rate)
{
        struct mliView camout = camin;
        struct mli_Vec direction_right = mliView_direction_right(camout);
        camout.position = mli_Vec_add(
                camout.position, mli_Vec_multiply(direction_right, rate));
        return camout;
}

struct mliView mliView_move_up(const struct mliView camin, const double rate)
{
        struct mliView camout = camin;
        camout.position.z += rate;
        return camout;
}

struct mliView mliView_look_right(const struct mliView camin, const double rate)
{
        struct mliView camout = camin;
        const double diff = camin.field_of_view * rate;
        camout.rotation.z = fmod(camout.rotation.z - diff, (2. * MLI_MATH_PI));
        return camout;
}

struct mliView mliView_look_down_when_possible(
        const struct mliView camin,
        const double rate)
{
        struct mliView camout = camin;
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

struct mliView mliView_increase_fov(
        const struct mliView camin,
        const double rate)
{
        struct mliView camout = camin;
        if (camout.field_of_view * rate > mli_math_deg2rad(170)) {
                camout.field_of_view = mli_math_deg2rad(170);
        } else {
                camout.field_of_view *= rate;
        }
        return camout;
}

struct mliView mliView_decrease_fov(
        const struct mliView camin,
        const double rate)
{
        struct mliView camout = camin;
        if (camout.field_of_view / rate < mli_math_deg2rad(.1)) {
                camout.field_of_view = mli_math_deg2rad(.1);
        } else {
                camout.field_of_view /= rate;
        }
        return camout;
}

struct mliView mliView_look_up_when_possible(
        const struct mliView camin,
        const double rate)
{
        struct mliView camout = camin;
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

struct mliHomTraComp mliView_to_HomTraComp(const struct mliView view)
{
        struct mliHomTraComp view2root_comp;
        view2root_comp.translation = view.position;
        view2root_comp.rotation = mli_Quaternion_set_tait_bryan(
                view.rotation.x, view.rotation.y, view.rotation.z);
        return view2root_comp;
}

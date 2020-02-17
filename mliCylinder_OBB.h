/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICYLINDER_OBB_H_
#define MERLICT_MLICYLINDER_OBB_H_

#include <math.h>
#include "mliScenery.h"
#include "mliOBB.h"
#include "mliCylinder.h"
#include "mliSphere_OBB.h"
#include "mliRay_OBB.h"

struct mliVec mliCylinder_start_local(const mliCylinder cylinder) {
    return mliVec_set(0., 0., -cylinder.length*.5);}

struct mliVec mliCylinder_end_local(const mliCylinder cylinder) {
    return mliVec_set(0., 0., cylinder.length*.5);}

struct mliVec mliCylinder_start_root(
    const mliCylinder cylinder,
    const mliHomTra *local2root) {
    return mliHomTra_pos(local2root, mliCylinder_start_local(cylinder));}

struct mliVec mliCylinder_end_root(
    const mliCylinder cylinder,
    const mliHomTra *local2root) {
    return mliHomTra_pos(local2root, mliCylinder_end_local(cylinder));}

int mliCylinder_has_overlap_obb(
    const mliCylinder cylinder,
    const mliHomTraComp local2root_comp,
    const mliOBB obb) {
    mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    struct mliVec start_root = mliCylinder_start_root(cylinder, &local2root);
    struct mliVec end_root = mliCylinder_end_root(cylinder, &local2root);
    mliRay cylinder_ray_root = mliRay_set(
        start_root,
        mliVec_substract(end_root, start_root));
    mliOBB obb_dilated = mliOBB_dilate(obb, cylinder.radius);
    double cylinder_ray_parameter;
    if (mliRay_has_overlap_obb(
        cylinder_ray_root,
        obb_dilated,
        &cylinder_ray_parameter)
    ) {
        if (cylinder_ray_parameter <= cylinder.length)
            return 1;
    }
    return 0;}

mliOBB mliCylinder_obb(
    const mliCylinder cylinder,
    const mliHomTraComp local2root_comp) {
    mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    struct mliVec start_root = mliCylinder_start_root(cylinder, &local2root);
    struct mliVec end_root = mliCylinder_end_root(cylinder, &local2root);
    mliOBB start_obb = mliSphere_obb(cylinder.radius, start_root);
    mliOBB end_obb = mliSphere_obb(cylinder.radius, end_root);
    return mliOBB_outermost(start_obb, end_obb);}

#endif

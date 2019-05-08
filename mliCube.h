/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICUBE_H_
#define MERLICT_MLICUBE_H_

#include "mliVec.h"
#include "mliMath.h"
#include "mliOBB.h"

typedef struct {
    /*
     * Cubic Oriented-Bounding-Box
     * oriented w.r.t. the unit-vectors.
     */
    mliVec lower;
    float edge_length;
} mliCube;

mliVec mliCube_upper(const mliCube a) {
    return mliVec_add(
        a.lower,
        mliVec_set(a.edge_length, a.edge_length, a.edge_length));}

mliOBB mliCube_to_obb(const mliCube a) {
    mliOBB out;
    out.lower = a.lower;
    out.upper = mliCube_upper(a);
    return out;}

mliVec mliCube_center(const mliCube a) {
    return mliVec_set(
        a.lower.x + a.edge_length*.5,
        a.lower.y + a.edge_length*.5,
        a.lower.z + a.edge_length*.5);}

mliCube mliCube_outermost_cube(const mliOBB a) {
    mliCube cube;
    mliVec center;
    mliVec half_diagonal;
    mliVec diff;
    float max_half_length;
    center = mliOBB_center(a);
    diff = mliVec_substract(a.upper, a.lower);
    max_half_length = .5*MLI_MAX3(diff.x, diff.y, diff.z);
    half_diagonal.x = max_half_length;
    half_diagonal.y = max_half_length;
    half_diagonal.z = max_half_length;
    cube.lower = mliVec_substract(center, half_diagonal);
    cube.edge_length = max_half_length*2.;
    return cube;}

mliCube mliCube_octree_child(
    const mliCube cube,
    const uint32_t sx,
    const uint32_t sy,
    const uint32_t sz) {
    mliCube child;
    mliVec length;
    mliVec center = mliCube_center(cube);
    length = mliVec_substract(center, cube.lower);
    child.lower = cube.lower;
    child.edge_length = .5*cube.edge_length;
    if (sx) {
        child.lower.x += length.x;}
    if (sy) {
        child.lower.y += length.y;}
    if (sz) {
        child.lower.z += length.z;}
    return child;}

#endif

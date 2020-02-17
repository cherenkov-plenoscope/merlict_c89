/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICUBE_H_
#define MERLICT_MLICUBE_H_

#include "mliVec.h"
#include "mli_math.h"
#include "mliOBB.h"

typedef struct {
    /*
     * Cubic Oriented-Bounding-Box
     * oriented w.r.t. the unit-vectors.
     */
    struct mliVec lower;
    double edge_length;
} mliCube;

struct mliVec mliCube_upper(const mliCube a) {
    return mliVec_add(
        a.lower,
        mliVec_set(a.edge_length, a.edge_length, a.edge_length));}

mliOBB mliCube_to_obb(const mliCube a) {
    mliOBB out;
    out.lower = a.lower;
    out.upper = mliCube_upper(a);
    return out;}

struct mliVec mliCube_center(const mliCube a) {
    return mliVec_set(
        a.lower.x + a.edge_length*.5,
        a.lower.y + a.edge_length*.5,
        a.lower.z + a.edge_length*.5);}

mliCube mliCube_outermost_cube(const mliOBB a) {
    mliCube cube;
    struct mliVec center;
    struct mliVec half_diagonal;
    struct mliVec diff;
    double max_half_length;
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
    struct mliVec length;
    struct mliVec center = mliCube_center(cube);
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

#define MLI_IS_BIT(var,pos) ((var) & (1<<(pos)))

mliCube mliCube_octree_child_code(
    const mliCube cube,
    const uint8_t a) {
    mliCube child;
    struct mliVec length;
    struct mliVec center = mliCube_center(cube);
    length = mliVec_substract(center, cube.lower);
    child.lower = cube.lower;
    child.edge_length = .5*cube.edge_length;
    if (MLI_IS_BIT(a, 2)) {
        child.lower.x += length.x;}
    if (MLI_IS_BIT(a, 1)) {
        child.lower.y += length.y;}
    if (MLI_IS_BIT(a, 0)) {
        child.lower.z += length.z;}
    return child;}

#endif

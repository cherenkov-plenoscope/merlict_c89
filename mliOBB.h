/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOBB_H_
#define MERLICT_MLIOBB_H_

#include "mliVec.h"
#include "mliMath.h"

typedef struct {
    mliVec lower;
    mliVec upper;
} mliOBB;

mliOBB mliOBB_outermost(const mliOBB a, const mliOBB b) {
    mliOBB obb;
    obb.lower.x = MLI_MIN2(a.lower.x, b.lower.x);
    obb.lower.y = MLI_MIN2(a.lower.y, b.lower.y);
    obb.lower.z = MLI_MIN2(a.lower.z, b.lower.z);
    obb.upper.x = MLI_MAX2(a.upper.x, b.upper.x);
    obb.upper.y = MLI_MAX2(a.upper.y, b.upper.y);
    obb.upper.z = MLI_MAX2(a.upper.z, b.upper.z);
    return obb;}

mliVec mliOBB_center(const mliOBB a) {
    mliVec sum = mliVec_add(&a.upper, &a.lower);
    return mliVec_multiply(&sum, .5);}

mliOBB mliOBB_outer_cube(const mliOBB a) {
    mliOBB cube;
    mliVec center;
    mliVec half_diagonal;
    float max_half_length;
    mliVec diff = mliVec_substract(&a.upper, &a.lower);
    max_half_length = .5*MLI_MAX3(diff.x, diff.y, diff.z);
    half_diagonal.x = max_half_length;
    half_diagonal.y = max_half_length;
    half_diagonal.z = max_half_length;
    center = mliOBB_center(a);
    cube.lower = mliVec_substract(&center, &half_diagonal);
    cube.upper = mliVec_add(&center, &half_diagonal);
    return cube;}

mliOBB mliOBB_octtree_child(
    const mliOBB node,
    const uint32_t sx,
    const uint32_t sy,
    const uint32_t sz) {
    mliOBB child;
    mliVec length;
    mliVec center = mliOBB_center(node);
    length = mliVec_substract(&center, &node.lower);
    child.lower = node.lower;
    child.upper = center;
    if (sx) {
        child.lower.x += length.x;
        child.upper.x += length.x;}
    if (sy) {
        child.lower.y += length.y;
        child.upper.y += length.y;}
    if (sz) {
        child.lower.z += length.z;
        child.upper.z += length.z;}
    return child;}

#endif

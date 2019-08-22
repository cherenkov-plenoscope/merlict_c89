/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_FRAME_H_
#define MERLICT_FRAME_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliMesh.h"
#include "mliSphericalCapHex.h"
#include "mliSphere.h"
#include "mliCylinder.h"
#include "mliHexagon.h"
#include "mliBiCirclePlane.h"
#include "mliDisc.h"
#include "mliSurface.h"
#include "mliSurfaces.h"
#include "mliVector.h"

#define MLI_FRAME 1000u
#define MLI_MESH 1001u

typedef struct mliFrame {
    uint64_t id;
    struct mliFrame* mother;
    mliVector children;

    mliHomTraComp frame2mother;

    uint32_t type;
    union {
        mliMesh* mesh;
        mliSphericalCapHex* spherical_cap_hex;
        mliSphere* sphere;
        mliCylinder* cylinder;
        mliHexagon* hexagon;
        mliBiCirclePlane* bicircleplane;
        mliDisc* disc;
    } primitive;
    uint64_t surface;
} mliFrame;

mliFrame mliFrame_init() {
    mliFrame f;
    f.id = 0u;
    f.children = mliVector_init();
    f.mother = NULL;
    f.frame2mother.trans = mliVec_set(0., 0., 0.);
    f.frame2mother.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 0.),
        0.);
    f.type = MLI_FRAME;
    f.surface = 0u;
    return f;}

int mliFrame_malloc(mliFrame *f, const uint64_t type) {
    f->type = type;
    switch(type) {
        case MLI_FRAME:
            mli_check(
                mliVector_malloc(&f->children, 0u, sizeof(mliFrame*)),
                "Can not allocate memory for children of frame.");
            break;
        case MLI_MESH:
            mli_malloc(f->primitive.mesh, mliMesh, 1u);
            *f->primitive.mesh = mliMesh_init();
            break;
        case MLI_SPHERICAL_CAP_HEX:
            mli_malloc(f->primitive.spherical_cap_hex, mliSphericalCapHex, 1u);
            break;
        case MLI_SPHERE:
            mli_malloc(f->primitive.sphere, mliSphere, 1u);
            break;
        case MLI_CYLINDER:
            mli_malloc(f->primitive.cylinder, mliCylinder, 1u);
            break;
        case MLI_HEXAGON:
            mli_malloc(f->primitive.hexagon, mliHexagon, 1u);
            break;
        case MLI_BICIRCLEPLANE:
            mli_malloc(f->primitive.bicircleplane, mliBiCirclePlane, 1u);
            break;
        case MLI_DISC:
            mli_malloc(f->primitive.disc, mliDisc, 1u);
            break;
        default: mli_sentinel("Unknown type of primitive."); break;
    }
    return 1;
error:
    return 0;
}

void mliFrame_free(mliFrame *f) {
    uint64_t c;
    switch(f->type) {
        case MLI_FRAME:
            for (c = 0; c < f->children.size; c++) {
                mliFrame* child = *((mliFrame**)mliVector_at(&f->children, c));
                mliFrame_free(child);
            }
            mliVector_free(&f->children);
            break;
        case MLI_MESH:
            mliMesh_free(f->primitive.mesh);
            free(f->primitive.mesh);
            break;
        case MLI_SPHERICAL_CAP_HEX:
            free(f->primitive.spherical_cap_hex);
            break;
        case MLI_SPHERE:
            free(f->primitive.sphere);
            break;
        case MLI_CYLINDER:
            free(f->primitive.cylinder);
            break;
        case MLI_HEXAGON:
            free(f->primitive.hexagon);
            break;
        case MLI_BICIRCLEPLANE:
            free(f->primitive.bicircleplane);
            break;
        case MLI_DISC:
            free(f->primitive.disc);
            break;
        default:
            assert(0);
            break;
    }
    *f = mliFrame_init();}

int mliFrame_set_mother_and_child(mliFrame* mother, mliFrame* child) {
    mli_check(
        mother->type == MLI_FRAME,
        "Expected mother to be of type FRAME");
    mli_check(
        mliVector_push_back(&mother->children, &child),
        "Can not push back child-frame.");
    child->mother = (struct mliFrame*)mother;
    return 1;
error:
    return 0;}

mliFrame* mliFrame_add(mliFrame* mother, const uint64_t type) {
    mliFrame* child = NULL;
    mli_malloc(child, mliFrame, 1u);
    *child = mliFrame_init();
    mli_check(
        mliFrame_malloc(child, type),
        "Can not allocate child-frame.");
    mli_check(
        mliFrame_set_mother_and_child(mother, child),
        "Can not allocate child-pointer.");
    return child;
error:
    return NULL;}

int mli_type_to_string(const uint64_t type, char* s) {
    switch (type) {
        case MLI_FRAME: sprintf(s, "Frame"); break;
        case MLI_MESH: sprintf(s, "Mesh"); break;
        case MLI_SPHERICAL_CAP_HEX: sprintf(s, "SphericalCapHex"); break;
        case MLI_SPHERE: sprintf(s, "Sphere"); break;
        case MLI_CYLINDER: sprintf(s, "Cylinder"); break;
        case MLI_HEXAGON: sprintf(s, "Hexagon"); break;
        case MLI_BICIRCLEPLANE: sprintf(s, "BiCirclePlane"); break;
        case MLI_DISC: sprintf(s, "Disc"); break;
        default: mli_sentinel("Type is unknown."); break;}
    return 1;
error:
    return 0;}

int mli_string_to_type(const char* s, uint64_t *type) {
    if (strcmp(s, "Frame") == 0) {
        *type = MLI_FRAME;
    } else if (strcmp(s, "Mesh") == 0) {
        *type = MLI_MESH;
    } else if (strcmp(s, "SphericalCapHex") == 0) {
        *type = MLI_SPHERICAL_CAP_HEX;
    } else if (strcmp(s, "Sphere") == 0) {
        *type = MLI_SPHERE;
    } else if (strcmp(s, "Cylinder") == 0) {
        *type = MLI_CYLINDER;
    } else if (strcmp(s, "Hexagon") == 0) {
        *type = MLI_HEXAGON;
    } else if (strcmp(s, "BiCirclePlane") == 0) {
        *type = MLI_BICIRCLEPLANE;
    } else if (strcmp(s, "Disc") == 0) {
        *type = MLI_DISC;
    } else {
        mli_sentinel("Type is unknown.");
    }
    return 1;
error:
    return 0;}

void __mliFrame_print(mliFrame *f, const uint64_t indention) {
    uint64_t c;
    char type_string[1024];
    mli_type_to_string(f->type, type_string);
    printf("%*s", (int)indention, "");
    printf(" __%s__ id:%lu, at:%p\n", type_string, f->id, (void*)f);
    printf("%*s", (int)indention, "");
    printf("|-mother: id:");
    if (f->mother != NULL) {
        printf("%lu,", f->mother->id);
    } else {
        printf("%p,", (void*)f->mother);
    }
    printf(" at:%p\n", (void*)f->mother);
    printf("%*s", (int)indention, "");
    printf("|-pos: (%0.1f, %0.1f, %0.1f)\n",
        f->frame2mother.trans.x,
        f->frame2mother.trans.y,
        f->frame2mother.trans.z);
    printf("%*s", (int)indention, "");
    printf("|-rot: (%.1f| %.1f, %.1f, %.1f)\n",
        f->frame2mother.rot.w,
        f->frame2mother.rot.x,
        f->frame2mother.rot.y,
        f->frame2mother.rot.z);
    if (f->type != MLI_FRAME) {
        printf("%*s", (int)indention, "");
        printf("|-surface: %lu\n", f->surface);
    }
    for (c = 0; c < f->children.size; c++) {
        mliFrame* child = *((mliFrame**)mliVector_at(&f->children, c));
        __mliFrame_print(child, indention + 4);
    }
}

void mliFrame_print(mliFrame *f) {
    __mliFrame_print(f, 0u);
}

#endif

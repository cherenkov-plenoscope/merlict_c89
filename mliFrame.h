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
#include "mliDynArray_template.h"

#define MLI_FRAME 1000u
#define MLI_MESH 1001u

#define MLI_FRAME_NAME_SIZE 64u


struct mliFrame;

MLIDYNARRAY_TEMPLATE(mli, FramePtr, struct mliFrame*)

struct mliFrame {
    char name[MLI_FRAME_NAME_SIZE];
    uint64_t id;
    struct mliFrame* mother;
    struct mliDynFramePtr children;

    struct mliHomTraComp frame2mother;
    struct mliHomTraComp frame2root;

    uint32_t type;
    union {
        struct mliMesh *mesh;
        struct mliSphericalCapHex *spherical_cap_hex;
        struct mliSphere *sphere;
        struct mliCylinder *cylinder;
        struct mliHexagon *hexagon;
        struct mliBiCirclePlane *bicircleplane;
        struct mliDisc *disc;
    } primitive;
    struct mliBoundaryLayer boundary_layer;
};

struct mliFrame mliFrame_init() {
    struct mliFrame f;
    MLI_ZEROS(f.name, MLI_FRAME_NAME_SIZE);
    f.id = 0u;
    f.children = mliDynFramePtr_init();
    f.mother = NULL;
    f.frame2mother.trans = mliVec_set(0., 0., 0.);
    f.frame2mother.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 0.),
        0.);
    f.frame2root = f.frame2mother;
    f.type = MLI_FRAME;
    f.boundary_layer.inner.surface = 0u;
    f.boundary_layer.outer.surface = 0u;
    f.boundary_layer.inner.medium = 0u;
    f.boundary_layer.outer.medium = 0u;
    return f;}

int mliFrame_malloc(struct mliFrame *f, const uint64_t type) {
    f->type = type;
    switch(type) {
        case MLI_FRAME:
            mli_check(mliDynFramePtr_malloc(&f->children, 0u),
                "Can not allocate memory for children of frame.");
            break;
        case MLI_MESH:
            mli_malloc(f->primitive.mesh, struct mliMesh, 1u);
            *f->primitive.mesh = mliMesh_init();
            break;
        case MLI_SPHERICAL_CAP_HEX:
            mli_malloc(
                f->primitive.spherical_cap_hex,
                struct mliSphericalCapHex,
                1u);
            break;
        case MLI_SPHERE:
            mli_malloc(f->primitive.sphere, struct mliSphere, 1u);
            break;
        case MLI_CYLINDER:
            mli_malloc(f->primitive.cylinder, struct mliCylinder, 1u);
            break;
        case MLI_HEXAGON:
            mli_malloc(f->primitive.hexagon, struct mliHexagon, 1u);
            break;
        case MLI_BICIRCLEPLANE:
            mli_malloc(f->primitive.bicircleplane, struct mliBiCirclePlane, 1u);
            break;
        case MLI_DISC:
            mli_malloc(f->primitive.disc, struct mliDisc, 1u);
            break;
        default: mli_sentinel("Unknown type of primitive."); break;
    }
    return 1;
error:
    return 0;
}

int mliFrame_free(struct mliFrame *f) {
    uint64_t c;
    switch(f->type) {
        case MLI_FRAME:
            for (c = 0; c < f->children.dyn.size; c++) {
                struct mliFrame *child = f->children.arr[c];
                mliFrame_free(child);
            }
            mliDynFramePtr_free(&f->children);
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
            mli_sentinel("Can not free unknown type of primitive.");
            break;
    }
    *f = mliFrame_init();
    return 1;
error:
    return 0;
}

int mliFrame_set_mother_and_child(
    struct mliFrame *mother,
    struct mliFrame *child)
{
    mli_check(mother->type == MLI_FRAME,
        "Expected mother to be of type FRAME");
    mli_check(mliDynFramePtr_push_back(&mother->children, child),
        "Can not push back child-frame.");

    child->mother = (struct mliFrame *)mother;
    return 1;
error:
    return 0;
}

struct mliFrame *mliFrame_add(struct mliFrame *mother, const uint64_t type)
{
    struct mliFrame *child = NULL;
    mli_malloc(child, struct mliFrame, 1u);
    (*child) = mliFrame_init();
    mli_check(
        mliFrame_malloc(child, type),
        "Can not allocate child-frame.");
    mli_check(mliFrame_set_mother_and_child(mother, child),
        "Can not allocate child-pointer.");
    return child;
error:
    return NULL;
}

int mli_type_to_string(const uint64_t type, char* s) {
    switch (type) {
        case MLI_TRIANGLE: sprintf(s, "Triangle"); break;
        case MLI_FRAME: sprintf(s, "Frame"); break;
        case MLI_MESH: sprintf(s, "Mesh"); break;
        case MLI_SPHERICAL_CAP_HEX: sprintf(s, "SphericalCapHex"); break;
        case MLI_SPHERE: sprintf(s, "Sphere"); break;
        case MLI_CYLINDER: sprintf(s, "Cylinder"); break;
        case MLI_HEXAGON: sprintf(s, "Hexagon"); break;
        case MLI_BICIRCLEPLANE: sprintf(s, "BiCirclePlane"); break;
        case MLI_DISC: sprintf(s, "Disc"); break;
        default:
            fprintf(stderr, "Unknown Type: %ld\n", type);
            mli_sentinel("Type is unknown."); break;}
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

void __mliFrame_print(const struct mliFrame *f, const uint64_t indention) {
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
        printf("|-boundary_layer (inner: %u, outer: %u)\n",
            f->boundary_layer.inner.surface, f->boundary_layer.outer.surface);
    }
    for (c = 0; c < f->children.dyn.size; c++) {
        const struct mliFrame *child = f->children.arr[c];
        __mliFrame_print(child, indention + 4);
    }
}

void mliFrame_print(struct mliFrame *f) {
    __mliFrame_print(f, 0u);
}

void mliFrame_set_frame2root(struct mliFrame *f) {
    if (f->mother == NULL) {
        f->frame2root = f->frame2mother;
    } else {
        f->frame2root = mliHomTraComp_sequence(
            f->mother->frame2root,
            f->frame2mother);
    }
    if (f->type == MLI_FRAME) {
        uint64_t c;
        for (c = 0; c < f->children.dyn.size; c++) {
            struct mliFrame *child = f->children.arr[c];
            mliFrame_set_frame2root(child);
        }
    }
}

#endif

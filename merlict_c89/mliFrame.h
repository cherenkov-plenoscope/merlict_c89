/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFRAME_H_
#define MERLICT_C89_MLIFRAME_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mli_primitive_codes.h"
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
#include "mliBoundaryLayer.h"
#include "mliDynArray_template.h"

#define MLI_FRAME 1000u
#define MLI_MESH 1001u
#define MLI_FRAME_NAME_SIZE 64u

MLIDYNARRAY_DEFINITON(mli, FramePtr, struct mliFrame *)

struct mliFrame {
        char name[MLI_FRAME_NAME_SIZE];
        uint64_t id;
        struct mliFrame *mother;
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

void mliFrame_set_frame2root(struct mliFrame *f);
void mliFrame_print(struct mliFrame *f);
void __mliFrame_print(const struct mliFrame *f, const uint64_t indention);
int mli_string_to_type(const char *s, uint64_t *type);
int mli_type_to_string(const uint64_t type, char *s);
struct mliFrame *mliFrame_add(struct mliFrame *mother, const uint64_t type);
int mliFrame_set_mother_and_child(
        struct mliFrame *mother,
        struct mliFrame *child);
int mliFrame_malloc(struct mliFrame *f, const uint64_t type);
int mliFrame_free(struct mliFrame *f);
struct mliFrame mliFrame_init(void);
#endif

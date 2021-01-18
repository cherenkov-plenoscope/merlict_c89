/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFRAME_H_
#define MERLICT_C89_MLIFRAME_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliVec.h"
#include "mliHomTra.h"
#include "mliFunc.h"
#include "mliObject.h"
#include "mliSurface.h"
#include "mliBoundaryLayer.h"
#include "mliDynArray_template.h"

#define MLI_FRAME 1000u
#define MLI_OBJECT 1001u

MLIDYNARRAY_DEFINITON(mli, FramePtr, struct mliFrame *)

struct mliFrame {
        uint32_t type;
        uint32_t id;
        struct mliHomTraComp frame2mother;
        struct mliHomTraComp frame2root;
        struct mliFrame *mother;

        struct mliDynFramePtr children;

        uint32_t object;
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
void mliFrame_free(struct mliFrame *f);
struct mliFrame mliFrame_init(void);
#endif

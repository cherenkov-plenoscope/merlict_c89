/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIFRAME_H_
#define MLIFRAME_H_

#include <stdint.h>
#include "../homtra/homtra.h"
#include "../vector/vector.h"
#include "mliDynUint32.h"
#include "mliDynFramePtr.h"

#define MLI_FRAME 1000u
#define MLI_OBJECT 1001u

struct mliFrame {
        uint32_t type;
        uint32_t id;
        struct mli_HomTraComp frame2mother;
        struct mli_HomTraComp frame2root;
        struct mliFrame *mother;

        struct mliDynFramePtr children;

        uint32_t object;
        struct mliDynUint32 boundary_layers;
};

void mliFrame_set_frame2root(struct mliFrame *f);
void mliFrame_print(struct mliFrame *f);
void mliFrame_print_walk(const struct mliFrame *f, const uint64_t indention);
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

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FRAME_H_
#define MLI_FRAME_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../homtra/homtra.h"
#include "../vector/uint32_vector.h"
#include "frame_ptr_vector.h"

enum mli_frame_types {
        MLI_FRAME_TYPE_FRAME = 1000,
        MLI_FRAME_TYPE_OBJECT = 1001
};

struct mli_Frame {
        uint32_t type;
        uint32_t id;
        struct mli_HomTraComp frame2mother;
        struct mli_HomTraComp frame2root;
        struct mli_Frame *mother;

        struct mli_FramePtrVector children;

        uint32_t object;
        struct mli_Uint32Vector boundary_layers;
};

void mli_Frame_set_frame2root(struct mli_Frame *f);
void mli_Frame_print(struct mli_Frame *f);
void mli_Frame_print_walk(const struct mli_Frame *f, const uint64_t indention);
chk_rc mli_frame_string_to_type(const char *s, uint64_t *type);
chk_rc mli_frame_type_to_string(const uint64_t type, char *s);
struct mli_Frame *mli_Frame_add(struct mli_Frame *mother, const uint64_t type);
chk_rc mli_Frame_set_mother_and_child(
        struct mli_Frame *mother,
        struct mli_Frame *child);
chk_rc mli_Frame_malloc(struct mli_Frame *f, const uint64_t type);
void mli_Frame_free(struct mli_Frame *f);
struct mli_Frame mli_Frame_init(void);
chk_rc mli_Frame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mli_Frame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);
chk_rc mli_Frame_estimate_num_robjects_and_total_num_boundary_layers_walk(
        const struct mli_Frame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);
#endif

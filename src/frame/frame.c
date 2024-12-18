/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "frame.h"
#include "../chk/chk.h"
#include "frame_ptr_vector.h"

struct mli_Frame;

struct mli_Frame mli_Frame_init(void)
{
        struct mli_Frame f;
        f.type = MLI_FRAME_TYPE_FRAME;
        f.id = 0u;
        f.frame2mother.translation = mli_Vec_init(0., 0., 0.);
        f.frame2mother.rotation = mli_Quaternion_set_tait_bryan(0., 0., 0.);
        f.frame2root = f.frame2mother;
        f.mother = NULL;
        f.children = mli_FramePtrVector_init();
        f.object = 0u;

        f.boundary_layers = mli_Uint32Vector_init();
        return f;
}

void mli_Frame_free(struct mli_Frame *f)
{
        uint64_t c;
        if (f->type == MLI_FRAME_TYPE_FRAME) {
                for (c = 0; c < f->children.size; c++) {
                        struct mli_Frame *child = f->children.array[c];
                        mli_Frame_free(child);
                }
                mli_FramePtrVector_free(&f->children);
        }
        if (f->type == MLI_FRAME_TYPE_OBJECT) {
                mli_Uint32Vector_free(&f->boundary_layers);
        }
        (*f) = mli_Frame_init();
}

int mli_Frame_malloc(struct mli_Frame *f, const uint64_t type)
{
        mli_Frame_free(f);
        f->type = type;
        if (type == MLI_FRAME_TYPE_FRAME) {
                chk_msg(mli_FramePtrVector_malloc(&f->children, 0u),
                        "Can not allocate children of frame.");
        }
        if (type == MLI_FRAME_TYPE_OBJECT) {
                chk_msg(mli_Uint32Vector_malloc(&f->boundary_layers, 0u),
                        "Failed to malloc frame's boundary_layers.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_Frame_set_mother_and_child(
        struct mli_Frame *mother,
        struct mli_Frame *child)
{
        chk_msg(mother->type == MLI_FRAME_TYPE_FRAME,
                "Expected mother to be of type FRAME");
        chk_msg(mli_FramePtrVector_push_back(&mother->children, child),
                "Can not push back child-frame.");

        child->mother = (struct mli_Frame *)mother;
        return 1;
chk_error:
        return 0;
}

struct mli_Frame *mli_Frame_add(struct mli_Frame *mother, const uint64_t type)
{
        struct mli_Frame *child = NULL;
        chk_malloc(child, struct mli_Frame, 1u);
        chk_msg(mli_Frame_malloc(child, type), "Can not allocate child-frame.");
        chk_msg(mli_Frame_set_mother_and_child(mother, child),
                "Can not allocate child-pointer.");
        return child;
chk_error:
        return NULL;
}

int mli_frame_type_to_string(const uint64_t type, char *s)
{
        switch (type) {
        case MLI_FRAME_TYPE_FRAME:
                sprintf(s, "frame");
                break;
        case MLI_FRAME_TYPE_OBJECT:
                sprintf(s, "object");
                break;
        default:
                chk_bad("Type is unknown.");
                break;
        }
        return 1;
chk_error:
        return 0;
}

int mli_frame_string_to_type(const char *s, uint64_t *type)
{
        if (strcmp(s, "frame") == 0) {
                *type = MLI_FRAME_TYPE_FRAME;
        } else if (strcmp(s, "object") == 0) {
                *type = MLI_FRAME_TYPE_OBJECT;
        } else {
                chk_bad("Type is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

void mli_Frame_print_walk(const struct mli_Frame *f, const uint64_t indention)
{
        uint64_t c;
        char type_string[1024];
        mli_frame_type_to_string(f->type, type_string);
        printf("%*s", (int)indention, "");
        printf(" __%s__ id:%u, at:%p\n", type_string, f->id, (void *)f);
        printf("%*s", (int)indention, "");
        printf("|-mother: id:");
        if (f->mother != NULL) {
                printf("%u,", f->mother->id);
        } else {
                printf("%p,", (void *)f->mother);
        }
        printf(" at:%p\n", (void *)f->mother);
        printf("%*s", (int)indention, "");
        printf("|-pos: (%0.1f, %0.1f, %0.1f)\n",
               f->frame2mother.translation.x,
               f->frame2mother.translation.y,
               f->frame2mother.translation.z);
        printf("%*s", (int)indention, "");
        printf("|-rotation: (%.1f| %.1f, %.1f, %.1f)\n",
               f->frame2mother.rotation.w,
               f->frame2mother.rotation.x,
               f->frame2mother.rotation.y,
               f->frame2mother.rotation.z);
        if (f->type == MLI_FRAME_TYPE_OBJECT) {
                uint32_t ii;
                printf("%*s", (int)indention, "");
                printf("|-boundary_layers [");
                for (ii = 0; ii < f->boundary_layers.size; ii++) {
                        uint32_t boundary_layer = f->boundary_layers.array[ii];
                        printf("%u,", boundary_layer);
                }
                printf("]\n");

                printf("%*s", (int)indention, "");
                printf("|-obj %u\n", f->object);
        }
        for (c = 0; c < f->children.size; c++) {
                struct mli_Frame *child = f->children.array[c];
                mli_Frame_print_walk(child, indention + 4);
        }
}

void mli_Frame_print(struct mli_Frame *f) { mli_Frame_print_walk(f, 0u); }

void mli_Frame_set_frame2root(struct mli_Frame *f)
{
        if (f->mother == NULL) {
                f->frame2root = f->frame2mother;
        } else {
                f->frame2root = mli_HomTraComp_sequence(
                        f->frame2mother, f->mother->frame2root);
        }
        if (f->type == MLI_FRAME_TYPE_FRAME) {
                uint64_t c;
                for (c = 0; c < f->children.size; c++) {
                        struct mli_Frame *child = f->children.array[c];
                        mli_Frame_set_frame2root(child);
                }
        }
}

int mli_Frame_estimate_num_robjects_and_total_num_boundary_layers_walk(
        const struct mli_Frame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        uint64_t c;
        switch (frame->type) {
        case MLI_FRAME_TYPE_FRAME:
                for (c = 0; c < frame->children.size; c++) {
                        chk(mli_Frame_estimate_num_robjects_and_total_num_boundary_layers_walk(
                                frame->children.array[c],
                                num_robjects,
                                total_num_boundary_layers));
                }
                break;
        case MLI_FRAME_TYPE_OBJECT:
                (*num_robjects) += 1;
                (*total_num_boundary_layers) += frame->boundary_layers.size;
                break;
        default:
                chk_bad("Expected either type 'frame' or 'object'.");
                break;
        }
        return 1;
chk_error:
        return 0;
}

int mli_Frame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mli_Frame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        (*num_robjects) = 0u;
        (*total_num_boundary_layers) = 0u;
        chk_msg(mli_Frame_estimate_num_robjects_and_total_num_boundary_layers_walk(
                        frame, num_robjects, total_num_boundary_layers),
                "Failed to walk tree of frames to estimate "
                "num_robjects and total_num_boundary_layers.");
        return 1;
chk_error:
        return 0;
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliFrame.h"

MLIDYNARRAY_IMPLEMENTATION(mli, FramePtr, struct mliFrame *)

struct mliFrame;

struct mliFrame mliFrame_init(void)
{
        struct mliFrame f;
        f.type = MLI_FRAME;
        f.id = 0u;
        f.frame2mother.translation = mliVec_set(0., 0., 0.);
        f.frame2mother.rotation = mliQuaternion_set_tait_bryan(0., 0., 0.);
        f.frame2root = f.frame2mother;
        f.mother = NULL;
        f.children = mliDynFramePtr_init();
        f.object = 0u;

        f.boundary_layers = mliDynUint32_init();
        return f;
}

void mliFrame_free(struct mliFrame *f)
{
        uint64_t c;
        if (f->type == MLI_FRAME) {
                for (c = 0; c < f->children.dyn.size; c++) {
                        struct mliFrame *child = f->children.arr[c];
                        mliFrame_free(child);
                }
                mliDynFramePtr_free(&f->children);
        }
        if (f->type == MLI_OBJECT) {
                mliDynUint32_free(&f->boundary_layers);
        }
        (*f) = mliFrame_init();
}

int mliFrame_malloc(struct mliFrame *f, const uint64_t type)
{
        mliFrame_free(f);
        f->type = type;
        if (type == MLI_FRAME) {
                mli_check(
                        mliDynFramePtr_malloc(&f->children, 0u),
                        "Can not allocate children of frame.");
        }
        if (type == MLI_OBJECT) {
                mli_check(
                        mliDynUint32_malloc(&f->boundary_layers, 0u),
                        "Failed to malloc frame's boundary_layers.");
        }
        return 1;
error:
        return 0;
}

int mliFrame_set_mother_and_child(
        struct mliFrame *mother,
        struct mliFrame *child)
{
        mli_check(
                mother->type == MLI_FRAME,
                "Expected mother to be of type FRAME");
        mli_check(
                mliDynFramePtr_push_back(&mother->children, child),
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
        mli_check(
                mliFrame_malloc(child, type), "Can not allocate child-frame.");
        mli_check(
                mliFrame_set_mother_and_child(mother, child),
                "Can not allocate child-pointer.");
        return child;
error:
        return NULL;
}

int mli_type_to_string(const uint64_t type, char *s)
{
        switch (type) {
        case MLI_FRAME:
                sprintf(s, "frame");
                break;
        case MLI_OBJECT:
                sprintf(s, "object");
                break;
        default:
                mli_sentinel("Type is unknown.");
                break;
        }
        return 1;
error:
        return 0;
}

int mli_string_to_type(const char *s, uint64_t *type)
{
        if (strcmp(s, "frame") == 0) {
                *type = MLI_FRAME;
        } else if (strcmp(s, "object") == 0) {
                *type = MLI_OBJECT;
        } else {
                mli_sentinel("Type is unknown.");
        }
        return 1;
error:
        return 0;
}

void __mliFrame_print(const struct mliFrame *f, const uint64_t indention)
{
        uint64_t c;
        char type_string[1024];
        mli_type_to_string(f->type, type_string);
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
        if (f->type == MLI_OBJECT) {
                uint32_t ii;
                printf("%*s", (int)indention, "");
                printf("|-boundary_layers [");
                for (ii = 0; ii < f->boundary_layers.dyn.size; ii++) {
                        printf("%u,", f->boundary_layers.arr[ii]);
                }
                printf("]\n");

                printf("%*s", (int)indention, "");
                printf("|-obj %u\n", f->object);
        }
        for (c = 0; c < f->children.dyn.size; c++) {
                const struct mliFrame *child = f->children.arr[c];
                __mliFrame_print(child, indention + 4);
        }
}

void mliFrame_print(struct mliFrame *f) { __mliFrame_print(f, 0u); }

void mliFrame_set_frame2root(struct mliFrame *f)
{
        if (f->mother == NULL) {
                f->frame2root = f->frame2mother;
        } else {
                f->frame2root = mliHomTraComp_sequence(
                        f->mother->frame2root, f->frame2mother);
        }
        if (f->type == MLI_FRAME) {
                uint64_t c;
                for (c = 0; c < f->children.dyn.size; c++) {
                        struct mliFrame *child = f->children.arr[c];
                        mliFrame_set_frame2root(child);
                }
        }
}

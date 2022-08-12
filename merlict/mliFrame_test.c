/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliFrame.h"

CASE("mliFrame_init, defaults")
{
        struct mliFrame f = mliFrame_init();
        CHECK(f.mother == NULL);
        CHECK(f.children.size == 0u);
        CHECK(f.type == MLI_FRAME);
}

CASE("mliFrame_malloc, free")
{
        struct mliFrame f = mliFrame_init();
        CHECK(mliFrame_malloc(&f, MLI_FRAME));
        mliFrame_free(&f);
        CHECK(f.mother == NULL);
        CHECK(f.children.size == 0u);
        CHECK(f.type == MLI_FRAME);
}

CASE("add two childs")
{
        struct mliFrame *child1 = NULL;
        struct mliFrame *child2 = NULL;
        struct mliFrame mother = mliFrame_init();
        CHECK(mliFrame_malloc(&mother, MLI_FRAME));
        mother.id = 1337;

        child1 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child1);
        CHECK(child1->type == MLI_FRAME);
        child1->id = 41;

        child2 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child2);
        CHECK(child2->type == MLI_FRAME);
        child2->id = 42;

        CHECK(mother.children.size == 2);
        CHECK(&mother == (struct mliFrame *)child1->mother);
        CHECK(&mother == (struct mliFrame *)child2->mother);
        mliFrame_free(&mother);
}

CASE("add grand childs")
{
        /*
         *                      |--child_00
         *          |--child_0--|
         *          |           |--child_01
         *  mother--|
         *          |           |--child_10
         *          |--child_1--|
         *                      |--child_11
         */
        struct mliFrame *child_0 = NULL;
        struct mliFrame *child_1 = NULL;
        struct mliFrame *child_00 = NULL;
        struct mliFrame *child_01 = NULL;
        struct mliFrame *child_10 = NULL;
        struct mliFrame *child_11 = NULL;
        struct mliFrame mother = mliFrame_init();
        CHECK(mliFrame_malloc(&mother, MLI_FRAME));
        mother.id = 1337;
        child_0 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child_0);
        child_0->id = 10;
        child_00 = mliFrame_add(child_0, MLI_FRAME);
        CHECK(child_00);
        child_00->id = 100;
        child_01 = mliFrame_add(child_0, MLI_FRAME);
        CHECK(child_01);
        child_01->id = 101;

        child_1 = mliFrame_add(&mother, MLI_FRAME);
        CHECK(child_1);
        child_1->id = 20;
        child_10 = mliFrame_add(child_1, MLI_FRAME);
        CHECK(child_10);
        child_10->id = 200;
        child_11 = mliFrame_add(child_1, MLI_FRAME);
        CHECK(child_11);
        child_11->id = 201;

        CHECK(mother.children.size == 2);
        CHECK(&mother == (struct mliFrame *)child_0->mother);
        CHECK(&mother == (struct mliFrame *)child_0->mother);

        CHECK(child_0->children.size == 2);
        CHECK(child_0 == (struct mliFrame *)child_00->mother);
        CHECK(child_0 == (struct mliFrame *)child_01->mother);

        CHECK(child_1->children.size == 2);
        CHECK(child_1 == (struct mliFrame *)child_10->mother);
        CHECK(child_1 == (struct mliFrame *)child_11->mother);
        /* mliFrame_print(&mother); */

        mliFrame_free(&mother);
}

CASE("basic object allocation and initialization")
{
        struct mliFrame *child = NULL;
        struct mliFrame mother = mliFrame_init();
        CHECK(mliFrame_malloc(&mother, MLI_FRAME));
        mother.id = 1337;

        child = mliFrame_add(&mother, MLI_OBJECT);
        CHECK(child);
        CHECK(child->type == MLI_OBJECT);
        child->id = 42;
        child->object = 13;
        mliFrame_free(&mother);
}

CASE("mapping frame-type-string to uint64")
{
        uint64_t i;
        uint64_t types[2] = {MLI_FRAME, MLI_OBJECT};
        uint64_t type;
        char type_string[1024];
        CHECK(!mli_string_to_type("Wtf?", &type));
        for (i = 0; i < 2; i++) {
                mli_type_to_string(types[i], type_string);
                CHECK(mli_string_to_type(type_string, &type));
                CHECK(type == types[i]);
        }
}

CASE("mliFrame_set_frame2root, only translation z-component")
{
        struct mliFrame *c1;
        struct mliFrame *c2;
        struct mliFrame *c1_c1;
        struct mliFrame root = mliFrame_init();
        CHECK(mliFrame_malloc(&root, MLI_FRAME));
        root.id = 1337;
        c1 = mliFrame_add(&root, MLI_FRAME);
        CHECK(c1);
        c1->id = 1;
        c1->frame2mother.translation = mliVec_init(0., 0., 1.);
        c2 = mliFrame_add(&root, MLI_FRAME);
        CHECK(c2);
        c2->id = 2;
        c2->frame2mother.translation = mliVec_init(0., 0., -1.);
        c1_c1 = mliFrame_add(c1, MLI_FRAME);
        CHECK(c1_c1);
        c1_c1->id = 11;
        c1_c1->frame2mother.translation = mliVec_init(0., 0., 1.);

        mliFrame_set_frame2root(&root);
        CHECK_MARGIN(root.frame2root.translation.z, 0., 1e-9);
        CHECK_MARGIN(c1->frame2root.translation.z, 1., 1e-9);
        CHECK_MARGIN(c2->frame2root.translation.z, -1., 1e-9);
        CHECK_MARGIN(c1_c1->frame2root.translation.z, 2., 1e-9);
}

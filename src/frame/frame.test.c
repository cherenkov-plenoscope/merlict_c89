/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Frame_init, defaults")
{
        struct mli_Frame f = mli_Frame_init();
        CHECK(f.mother == NULL);
        CHECK(f.children.size == 0u);
        CHECK(f.type == MLI_FRAME_TYPE_FRAME);
}

CASE("mli_Frame_malloc, free")
{
        struct mli_Frame f = mli_Frame_init();
        CHECK(mli_Frame_malloc(&f, MLI_FRAME_TYPE_FRAME));
        mli_Frame_free(&f);
        CHECK(f.mother == NULL);
        CHECK(f.children.size == 0u);
        CHECK(f.type == MLI_FRAME_TYPE_FRAME);
}

CASE("add two childs")
{
        struct mli_Frame *child1 = NULL;
        struct mli_Frame *child2 = NULL;
        struct mli_Frame mother = mli_Frame_init();
        CHECK(mli_Frame_malloc(&mother, MLI_FRAME_TYPE_FRAME));
        mother.id = 1337;

        child1 = mli_Frame_add(&mother, MLI_FRAME_TYPE_FRAME);
        CHECK(child1);
        CHECK(child1->type == MLI_FRAME_TYPE_FRAME);
        child1->id = 41;

        child2 = mli_Frame_add(&mother, MLI_FRAME_TYPE_FRAME);
        CHECK(child2);
        CHECK(child2->type == MLI_FRAME_TYPE_FRAME);
        child2->id = 42;

        CHECK(mother.children.size == 2);
        CHECK(&mother == (struct mli_Frame *)child1->mother);
        CHECK(&mother == (struct mli_Frame *)child2->mother);
        mli_Frame_free(&mother);
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
        struct mli_Frame *child_0 = NULL;
        struct mli_Frame *child_1 = NULL;
        struct mli_Frame *child_00 = NULL;
        struct mli_Frame *child_01 = NULL;
        struct mli_Frame *child_10 = NULL;
        struct mli_Frame *child_11 = NULL;
        struct mli_Frame mother = mli_Frame_init();
        CHECK(mli_Frame_malloc(&mother, MLI_FRAME_TYPE_FRAME));
        mother.id = 1337;
        child_0 = mli_Frame_add(&mother, MLI_FRAME_TYPE_FRAME);
        CHECK(child_0);
        child_0->id = 10;
        child_00 = mli_Frame_add(child_0, MLI_FRAME_TYPE_FRAME);
        CHECK(child_00);
        child_00->id = 100;
        child_01 = mli_Frame_add(child_0, MLI_FRAME_TYPE_FRAME);
        CHECK(child_01);
        child_01->id = 101;

        child_1 = mli_Frame_add(&mother, MLI_FRAME_TYPE_FRAME);
        CHECK(child_1);
        child_1->id = 20;
        child_10 = mli_Frame_add(child_1, MLI_FRAME_TYPE_FRAME);
        CHECK(child_10);
        child_10->id = 200;
        child_11 = mli_Frame_add(child_1, MLI_FRAME_TYPE_FRAME);
        CHECK(child_11);
        child_11->id = 201;

        CHECK(mother.children.size == 2);
        CHECK(&mother == (struct mli_Frame *)child_0->mother);
        CHECK(&mother == (struct mli_Frame *)child_0->mother);

        CHECK(child_0->children.size == 2);
        CHECK(child_0 == (struct mli_Frame *)child_00->mother);
        CHECK(child_0 == (struct mli_Frame *)child_01->mother);

        CHECK(child_1->children.size == 2);
        CHECK(child_1 == (struct mli_Frame *)child_10->mother);
        CHECK(child_1 == (struct mli_Frame *)child_11->mother);
        /* mli_Frame_print(&mother); */

        mli_Frame_free(&mother);
}

CASE("basic object allocation and initialization")
{
        struct mli_Frame *child = NULL;
        struct mli_Frame mother = mli_Frame_init();
        CHECK(mli_Frame_malloc(&mother, MLI_FRAME_TYPE_FRAME));
        mother.id = 1337;

        child = mli_Frame_add(&mother, MLI_FRAME_TYPE_OBJECT);
        CHECK(child);
        CHECK(child->type == MLI_FRAME_TYPE_OBJECT);
        child->id = 42;
        child->object = 13;
        mli_Frame_free(&mother);
}

CASE("mapping frame-type-string to uint64")
{
        uint64_t i;
        uint64_t types[2] = {MLI_FRAME_TYPE_FRAME, MLI_FRAME_TYPE_OBJECT};
        uint64_t type;
        char type_string[1024];
        CHECK(!mli_frame_string_to_type("Wtf?", &type));
        for (i = 0; i < 2; i++) {
                mli_frame_type_to_string(types[i], type_string);
                CHECK(mli_frame_string_to_type(type_string, &type));
                CHECK(type == types[i]);
        }
}

CASE("mli_Frame_set_frame2root, only translation z-component")
{
        struct mli_Frame *c1;
        struct mli_Frame *c2;
        struct mli_Frame *c1_c1;
        struct mli_Frame root = mli_Frame_init();
        CHECK(mli_Frame_malloc(&root, MLI_FRAME_TYPE_FRAME));
        root.id = 1337;
        c1 = mli_Frame_add(&root, MLI_FRAME_TYPE_FRAME);
        CHECK(c1);
        c1->id = 1;
        c1->frame2mother.translation = mli_Vec_set(0., 0., 1.);
        c2 = mli_Frame_add(&root, MLI_FRAME_TYPE_FRAME);
        CHECK(c2);
        c2->id = 2;
        c2->frame2mother.translation = mli_Vec_set(0., 0., -1.);
        c1_c1 = mli_Frame_add(c1, MLI_FRAME_TYPE_FRAME);
        CHECK(c1_c1);
        c1_c1->id = 11;
        c1_c1->frame2mother.translation = mli_Vec_set(0., 0., 1.);

        mli_Frame_set_frame2root(&root);
        CHECK_MARGIN(root.frame2root.translation.z, 0., 1e-9);
        CHECK_MARGIN(c1->frame2root.translation.z, 1., 1e-9);
        CHECK_MARGIN(c2->frame2root.translation.z, -1., 1e-9);
        CHECK_MARGIN(c1_c1->frame2root.translation.z, 2., 1e-9);
}

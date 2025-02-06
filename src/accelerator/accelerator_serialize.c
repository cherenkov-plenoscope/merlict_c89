/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "accelerator_serialize.h"
#include "../chk/chk.h"
#include "../magicid/magicid.h"
#include "../octree/octree_serialize.h"

int mli_Accelerator_to_io(const struct mli_Accelerator *self, struct mli_IO *f)
{
        uint64_t i = 0;

        /* magic identifier */
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_Accelerator"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        /* capacity */
        chk_IO_write(&self->num_objects, sizeof(uint32_t), 1, f);
        chk_IO_write(&self->num_robjects, sizeof(uint32_t), 1, f);

        for (i = 0; i < self->num_objects; i++) {
                chk(mli_OcTree_to_io(&self->object_octrees[i], f));
        }
        chk_IO_write(
                self->robject_aabbs,
                sizeof(struct mli_AABB),
                self->num_robjects,
                f);
        chk(mli_OcTree_to_io(&self->scenery_octree, f));

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Accelerator_from_io(struct mli_Accelerator *self, struct mli_IO *f)
{
        uint64_t i = 0u;
        struct mli_MagicId magic;

        uint32_t num_robjects = 0u;
        uint32_t num_objects = 0u;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Accelerator"));
        mli_MagicId_warn_version(&magic);

        /* capacity */
        chk_IO_read(&num_objects, sizeof(uint32_t), 1u, f);
        chk_IO_read(&num_robjects, sizeof(uint32_t), 1u, f);

        /* malloc */
        chk_mem(mli_Accelerator_malloc(self, num_objects, num_robjects));

        for (i = 0; i < self->num_objects; i++) {
                chk_mem(mli_OcTree_from_io(&self->object_octrees[i], f));
        }

        chk_IO_read(
                self->robject_aabbs,
                sizeof(struct mli_AABB),
                self->num_robjects,
                f);

        chk_mem(mli_OcTree_from_io(&self->scenery_octree, f));

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

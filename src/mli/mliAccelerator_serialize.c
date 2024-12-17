/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator_serialize.h"
#include "../chk/chk.h"
#include "mliMagicId.h"
#include "mliOcTree_serialize.h"

int mliAccelerator_fwrite(const struct mliAccelerator *accel, struct mli_IO *f)
{
        uint64_t i = 0;

        /* magic identifier */
        struct mliMagicId magic = mliMagicId_init();
        chk(mliMagicId_set(&magic, "mliAccelerator"));
        chk_IO_write(&magic, sizeof(struct mliMagicId), 1u, f);

        /* capacity */
        chk_IO_write(&accel->num_objects, sizeof(uint32_t), 1, f);
        chk_IO_write(&accel->num_robjects, sizeof(uint32_t), 1, f);

        for (i = 0; i < accel->num_objects; i++) {
                chk(mliOcTree_fwrite(&accel->object_octrees[i], f));
        }
        chk_IO_write(
                accel->robject_aabbs,
                sizeof(struct mli_AABB),
                accel->num_robjects,
                f);
        chk(mliOcTree_fwrite(&accel->scenery_octree, f));

        return 1;
chk_error:
        return 0;
}

int mliAccelerator_malloc_fread(struct mliAccelerator *accel, struct mli_IO *f)
{
        uint64_t i = 0u;
        struct mliMagicId magic;

        uint32_t num_robjects = 0u;
        uint32_t num_objects = 0u;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliAccelerator"));
        mliMagicId_warn_version(&magic);

        /* capacity */
        chk_IO_read(&num_objects, sizeof(uint32_t), 1u, f);
        chk_IO_read(&num_robjects, sizeof(uint32_t), 1u, f);

        /* malloc */
        chk_mem(mliAccelerator_malloc(accel, num_objects, num_robjects));

        for (i = 0; i < accel->num_objects; i++) {
                chk_mem(mliOcTree_malloc_fread(&accel->object_octrees[i], f));
        }

        chk_IO_read(
                accel->robject_aabbs,
                sizeof(struct mli_AABB),
                accel->num_robjects,
                f);

        chk_mem(mliOcTree_malloc_fread(&accel->scenery_octree, f));

        return 1;
chk_error:
        return 0;
}

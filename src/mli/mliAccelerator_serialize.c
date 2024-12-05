/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAccelerator_serialize.h"
#include "../chk/chk.h"
#include "mliMagicId.h"
#include "mliOcTree_serialize.h"

int mliAccelerator_fwrite(const struct mliAccelerator *accel, FILE *f)
{
        uint64_t i = 0;

        /* magic identifier */
        struct mliMagicId magic = mliMagicId_init();
        chk(mliMagicId_set(&magic, "mliAccelerator"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* capacity */
        chk_fwrite(&accel->num_objects, sizeof(uint32_t), 1, f);
        chk_fwrite(&accel->num_robjects, sizeof(uint32_t), 1, f);

        for (i = 0; i < accel->num_objects; i++) {
                mliOcTree_fwrite(&accel->object_octrees[i], f);
        }
        chk_fwrite(
                accel->robject_aabbs,
                sizeof(struct mliAABB),
                accel->num_robjects,
                f);
        mliOcTree_fwrite(&accel->scenery_octree, f);

        return 1;
chk_error:
        return 0;
}

int mliAccelerator_malloc_fread(struct mliAccelerator *accel, FILE *f)
{
        uint64_t i = 0u;
        struct mliMagicId magic;

        uint32_t num_robjects = 0u;
        uint32_t num_objects = 0u;

        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliAccelerator"));
        mliMagicId_warn_version(&magic);

        /* capacity */
        chk_fread(&num_objects, sizeof(uint32_t), 1u, f);
        chk_fread(&num_robjects, sizeof(uint32_t), 1u, f);

        /* malloc */
        chk_mem(mliAccelerator_malloc(accel, num_objects, num_robjects));

        for (i = 0; i < accel->num_objects; i++) {
                chk_mem(mliOcTree_malloc_fread(&accel->object_octrees[i], f));
        }

        chk_fread(
                accel->robject_aabbs,
                sizeof(struct mliAABB),
                accel->num_robjects,
                f);

        chk_mem(mliOcTree_malloc_fread(&accel->scenery_octree, f));

        return 1;
chk_error:
        return 0;
}

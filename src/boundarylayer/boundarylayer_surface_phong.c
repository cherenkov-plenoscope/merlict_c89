/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_phong.h"

int mli_BoundaryLayer_Surface_Phong_equal(
        const struct mli_BoundaryLayer_Surface_Phong *a,
        const struct mli_BoundaryLayer_Surface_Phong *b)
{
        if (a->specular_reflection_spectrum !=
            b->specular_reflection_spectrum) {
                return 0;
        }
        if (a->diffuse_reflection_spectrum != b->diffuse_reflection_spectrum) {
                return 0;
        }
        if (a->shininess_constant != b->shininess_constant) {
                return 0;
        }
        return 1;
}

int mli_BoundaryLayer_Surface_Phong_to_io(
        const struct mli_BoundaryLayer_Surface_Phong *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer_Surface_Phong"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(
                &self->specular_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_write(
                &self->diffuse_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->shininess_constant, sizeof(double), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_Phong_from_io(
        struct mli_BoundaryLayer_Surface_Phong *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_BoundaryLayer_Surface_Phong"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(
                &self->specular_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_read(
                &self->diffuse_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_read(&self->shininess_constant, sizeof(double), 1u, f);

        return 1;
chk_error:
        return 0;
}

/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_cook_torrance.h"

int mli_BoundaryLayer_Surface_Cook_Torrance_equal(
        const struct mli_BoundaryLayer_Surface_Cook_Torrance *a,
        const struct mli_BoundaryLayer_Surface_Cook_Torrance *b)
{
        if (a->specular_reflection_spectrum !=
            b->specular_reflection_spectrum) {
                return 0;
        }
        if (a->diffuse_reflection_spectrum != b->diffuse_reflection_spectrum) {
                return 0;
        }
        if (a->surface_roughness_constant != b->surface_roughness_constant) {
                return 0;
        }
        return 1;
}

int mli_BoundaryLayer_Surface_Cook_Torrance_to_io(
        const struct mli_BoundaryLayer_Surface_Cook_Torrance *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer_Surface_Cook_Torrance"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(
                &self->specular_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_write(
                &self->diffuse_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->surface_roughness_constant, sizeof(double), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_Cook_Torrance_from_io(
        struct mli_BoundaryLayer_Surface_Cook_Torrance *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(
                &magic, "mli_BoundaryLayer_Surface_Cook_Torrance"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(
                &self->specular_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_read(
                &self->diffuse_reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_read(&self->surface_roughness_constant, sizeof(double), 1u, f);

        return 1;
chk_error:
        return 0;
}

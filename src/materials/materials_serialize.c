/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "../materials/materials_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"
#include "../string/string_serialize.h"

int mli_Materials_to_io(const struct mli_Materials *self, struct mli_IO *f)
{
        uint64_t i;
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_Materials"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&self->spectra.size, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->surfaces.size, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->media.size, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->boundary_layers.size, sizeof(uint64_t), 1u, f);

        for (i = 0; i < self->spectra.size; i++) {
                chk(mli_Spectrum_to_io(&self->spectra.array[i], f));
        }
        for (i = 0; i < self->surfaces.size; i++) {
                chk(mli_BoundaryLayer_Surface_to_io(
                        &self->surfaces.array[i], f));
        }
        for (i = 0; i < self->media.size; i++) {
                chk(mli_BoundaryLayer_Medium_to_io(&self->media.array[i], f));
        }
        for (i = 0; i < self->boundary_layers.size; i++) {
                chk(mli_BoundaryLayer2_to_io(
                        &self->boundary_layers.array[i], f));
        }

        chk_IO_write(&self->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_io(struct mli_Materials *self, struct mli_IO *f)
{
        uint64_t i;
        struct mli_MagicId magic;
        struct mli_MaterialsCapacity cap;

        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Materials"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&cap.num_spectra, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_surfaces, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_media, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_boundary_layers, sizeof(uint64_t), 1u, f);

        chk(mli_Materials_malloc(self, cap));

        for (i = 0; i < self->spectra.size; i++) {
                chk(mli_Spectrum_from_io(&self->spectra.array[i], f));
        }
        for (i = 0; i < self->surfaces.size; i++) {
                chk(mli_BoundaryLayer_Surface_from_io(
                        &self->surfaces.array[i], f));
        }
        for (i = 0; i < self->media.size; i++) {
                chk(mli_BoundaryLayer_Medium_from_io(&self->media.array[i], f));
        }
        for (i = 0; i < self->boundary_layers.size; i++) {
                chk(mli_BoundaryLayer2_from_io(
                        &self->boundary_layers.array[i], f));
        }
        chk_IO_read(&self->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

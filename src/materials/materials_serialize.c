/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "../materials/materials_serialize.h"
#include "../medium/medium_serialize.h"
#include "../surface/surface_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"
#include "../string/string_serialize.h"

int mli_Materials_to_io(const struct mli_Materials *res, struct mli_IO *f)
{
        uint64_t i;
        struct mli_MagicId magic = mli_MagicId_init();

        /* magic identifier */
        chk(mli_MagicId_set(&magic, "mli_Materials"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&res->spectra.size, sizeof(uint64_t), 1u, f);
        chk_IO_write(&res->num_media, sizeof(uint64_t), 1u, f);
        chk_IO_write(&res->num_surfaces, sizeof(uint64_t), 1u, f);
        chk_IO_write(&res->num_boundary_layers, sizeof(uint64_t), 1u, f);

        for (i = 0; i < res->spectra.size; i++) {
                chk(mli_Spectrum_to_io(&res->spectra.array[i], f));
        }
        for (i = 0; i < res->num_media; i++) {
                chk(mli_String_to_io(&res->medium_names[i], f));
                chk(mli_Medium_to_io(&res->media[i], f));
        }
        for (i = 0; i < res->num_surfaces; i++) {
                chk(mli_String_to_io(&res->surface_names[i], f));
                chk(mli_Surface_to_io(&res->surfaces[i], f));
        }
        for (i = 0; i < res->num_boundary_layers; i++) {
                chk(mli_String_to_io(&res->boundary_layer_names[i], f));
                chk_IO_write(
                        &res->boundary_layers[i],
                        sizeof(struct mli_BoundaryLayer),
                        1,
                        f);
        }
        chk_IO_write(&res->default_refraction, sizeof(uint64_t), 1, f);
        chk_IO_write(&res->default_absorbtion, sizeof(uint64_t), 1, f);

        chk_IO_write(&res->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

int mli_Materials_from_io(struct mli_Materials *res, struct mli_IO *f)
{
        uint64_t i;
        struct mli_MagicId magic;
        struct mli_MaterialsCapacity cap;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Materials"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&cap.num_spectra, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_media, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_surfaces, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_boundary_layers, sizeof(uint64_t), 1u, f);

        chk(mli_Materials_malloc(res, cap));

        /* payload */
        for (i = 0; i < res->spectra.size; i++) {
                chk_msg(mli_Spectrum_from_io(&res->spectra.array[i], f),
                        "Failed to read spectrum.");
        }
        for (i = 0; i < res->num_media; i++) {
                chk_msg(mli_String_from_io(&res->medium_names[i], f),
                        "Failed to fread medium name.");
                chk_msg(mli_Medium_from_io(&res->media[i], f),
                        "Failed to fread Medium.");
        }
        for (i = 0; i < res->num_surfaces; i++) {
                chk_msg(mli_String_from_io(&res->surface_names[i], f),
                        "Failed to fread surface name.");
                chk_msg(mli_Surface_from_io(&res->surfaces[i], f),
                        "Failed to fread Surface.");
        }
        for (i = 0; i < res->num_boundary_layers; i++) {
                chk_msg(mli_String_from_io(&res->boundary_layer_names[i], f),
                        "Failed to fread boundary layer name.");
                chk_IO_read(
                        &res->boundary_layers[i],
                        sizeof(struct mli_BoundaryLayer),
                        1,
                        f);
        }
        chk_IO_read(&res->default_refraction, sizeof(uint64_t), 1, f);
        chk_IO_read(&res->default_absorbtion, sizeof(uint64_t), 1, f);

        chk_IO_read(&res->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

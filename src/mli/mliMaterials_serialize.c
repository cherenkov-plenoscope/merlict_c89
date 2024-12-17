/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_serialize.h"
#include "../medium/medium_serialize.h"
#include "mliSurface_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"
#include "../string/string_serialize.h"

int mliMaterials_to_io(const struct mliMaterials *res, struct mli_IO *f)
{
        uint64_t i;
        struct mli_MagicId magic = mli_MagicId_init();

        /* magic identifier */
        chk(mli_MagicId_set(&magic, "mliMaterials"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&res->num_media, sizeof(uint64_t), 1u, f);
        chk_IO_write(&res->num_surfaces, sizeof(uint64_t), 1u, f);
        chk_IO_write(&res->num_boundary_layers, sizeof(uint64_t), 1u, f);

        for (i = 0; i < res->num_media; i++) {
                chk(mli_String_to_io(&res->medium_names[i], f));
                chk(mliMedium_to_io(&res->media[i], f));
        }
        for (i = 0; i < res->num_surfaces; i++) {
                chk(mli_String_to_io(&res->surface_names[i], f));
                chk(mliSurface_to_io(&res->surfaces[i], f));
        }
        for (i = 0; i < res->num_boundary_layers; i++) {
                chk(mli_String_to_io(&res->boundary_layer_names[i], f));
                chk_IO_write(
                        &res->boundary_layers[i],
                        sizeof(struct mliBoundaryLayer),
                        1,
                        f);
        }
        chk_IO_write(&res->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

int mliMaterials_from_io(struct mliMaterials *res, struct mli_IO *f)
{
        uint64_t i;
        struct mli_MagicId magic;
        struct mliMaterialsCapacity cap;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mliMaterials"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&cap.num_media, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_surfaces, sizeof(uint64_t), 1u, f);
        chk_IO_read(&cap.num_boundary_layers, sizeof(uint64_t), 1u, f);

        chk(mliMaterials_malloc(res, cap));

        /* payload */
        for (i = 0; i < res->num_media; i++) {
                chk_dbg chk_msg(
                        mli_String_from_io(&res->medium_names[i], f),
                        "Failed to fread medium name.");
                chk_msg(mliMedium_from_io(&res->media[i], f),
                        "Failed to fread Medium.");
        }
        for (i = 0; i < res->num_surfaces; i++) {
                chk_msg(mli_String_from_io(&res->surface_names[i], f),
                        "Failed to fread surface name.");
                chk_msg(mliSurface_from_io(&res->surfaces[i], f),
                        "Failed to fread Surface.");
        }
        for (i = 0; i < res->num_boundary_layers; i++) {
                chk_msg(mli_String_from_io(&res->boundary_layer_names[i], f),
                        "Failed to fread boundary layer name.");
                chk_IO_read(
                        &res->boundary_layers[i],
                        sizeof(struct mliBoundaryLayer),
                        1,
                        f);
        }
        chk_IO_read(&res->default_medium, sizeof(uint64_t), 1, f);

        return 1;
chk_error:
        return 0;
}

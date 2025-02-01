/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_valid.h"
#include "../object/object.h"
#include "../chk/chk.h"

int mli_Materials_valid_media(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->media.size; i++) {
                chk_msg(mli_Medium_valid_wrt_materials(
                                &self->media.array[i], self),
                        "Medium is not valid.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.media[%lu]\n", i);
        return 0;
}

int mli_Materials_valid_spectra(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->spectra.size; i++) {
                chk_msg(mli_Func_is_valid(&self->spectra.array[i].spectrum),
                        "Expected spectrum function to be valid.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.spectra[%lu]\n", i);
        return 0;
}

int mli_Materials_valid_surfaces(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->surfaces.size; i++) {
                chk(self);
                chk_warning("IMPLEMENT ME!!!");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.surface[%lu]\n", i);
        return 0;
}

int mli_Materials_valid_boundary_layers(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->boundary_layers.size; i++) {
                struct mli_BoundaryLayer2 *layer =
                        &self->boundary_layers.array[i];
                chk_msg(layer->inner.surface < self->surfaces.size,
                        "inner.surface is invalid.");
                chk_msg(layer->outer.surface < self->surfaces.size,
                        "outer.surface is invalid.");
                chk_msg(layer->inner.medium < self->media.size,
                        "inner.medium is invalid.");
                chk_msg(layer->outer.medium < self->media.size,
                        "outer.medium is invalid.");

                chk_msg(mli_String_valid(&layer->name, 1),
                        "boundary_layer.name is invalid.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.boundary_layers[%lu]\n", i);
        return 0;
}

int mli_Materials_valid(const struct mli_Materials *self)
{
        chk_msg(self->default_medium <= self->media.size,
                "Expected default-medium to reference a valid medium.");
        chk(mli_Materials_valid_spectra(self));
        chk(mli_Materials_valid_media(self));
        chk(mli_Materials_valid_surfaces(self));
        chk(mli_Materials_valid_boundary_layers(self));
        return 1;
chk_error:
        return 0;
}

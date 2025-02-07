/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_valid.h"
#include "../object/object.h"
#include "../chk/chk.h"

mli_bool mli_Materials_valid_media(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->media.size; i++) {
                chk_msg(mli_Medium_valid_wrt_materials(
                                &self->media.array[i], self),
                        "Medium is not valid.");
        }
        return MLI_TRUE;
chk_error:
        chk_eprintf("In materials.media[%lu]\n", i);
        return MLI_FALSE;
}

mli_bool mli_Materials_valid_spectra(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->spectra.size; i++) {
                chk_msg(mli_Func_is_valid(&self->spectra.array[i].spectrum),
                        "Expected spectrum function to be valid.");
        }
        return MLI_TRUE;
chk_error:
        chk_eprintf("In materials.spectra[%lu]\n", i);
        return MLI_FALSE;
}

mli_bool mli_Materials_valid_surfaces(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->surfaces.size; i++) {
                chk_msg(mli_Surface_valid_wrt_materials(
                                &self->surfaces.array[i], self),
                        "Surface is not valid.");
        }
        return MLI_TRUE;
chk_error:
        chk_eprintf("In materials.surface[%lu]\n", i);
        return MLI_FALSE;
}

mli_bool mli_Materials_valid_boundary_layers(const struct mli_Materials *self)
{
        uint64_t i = 0u;
        for (i = 0; i < self->boundary_layers.size; i++) {
                struct mli_BoundaryLayer *layer =
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
        return MLI_TRUE;
chk_error:
        chk_eprintf("In materials.boundary_layers[%lu]\n", i);
        return MLI_FALSE;
}

mli_bool mli_Materials_valid(const struct mli_Materials *self)
{
        chk_msg(self->default_medium <= self->media.size,
                "Expected default-medium to reference a valid medium.");
        chk(mli_Materials_valid_spectra(self));
        chk(mli_Materials_valid_media(self));
        chk(mli_Materials_valid_surfaces(self));
        chk(mli_Materials_valid_boundary_layers(self));
        return MLI_TRUE;
chk_error:
        return MLI_FALSE;
}

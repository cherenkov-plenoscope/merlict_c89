/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials.h"
#include <stdlib.h>
#include "../chk/chk.h"
#include "../func/func_fprint.h"

struct mli_MaterialsCapacity mli_MaterialsCapacity_init(void)
{
        struct mli_MaterialsCapacity cap;
        cap.num_spectra = 0;
        cap.num_surfaces = 0;
        cap.num_media = 0;
        cap.num_boundary_layers = 0;
        return cap;
}

struct mli_Materials mli_Materials_init(void)
{
        struct mli_Materials res;
        res.spectra = mli_SpectrumArray_init();
        res.surfaces2 = mli_SurfaceArray_init();
        res.media2 = mli_MediumArray_init();
        res.layers2 = mli_BoundaryLayer2Array_init();

        res.default_medium = 0u;
        return res;
}

void mli_Materials_free(struct mli_Materials *self)
{
        mli_SpectrumArray_free(&self->spectra);
        mli_SurfaceArray_free(&self->surfaces2);
        mli_MediumArray_free(&self->media2);
        mli_BoundaryLayer2Array_free(&self->layers2);
        (*self) = mli_Materials_init();
}

int mli_Materials_malloc(
        struct mli_Materials *self,
        const struct mli_MaterialsCapacity rescap)
{
        uint64_t i;
        mli_Materials_free(self);

        chk(mli_SpectrumArray_malloc(&self->spectra, rescap.num_spectra));
        for (i = 0; i < self->spectra.size; i++) {
                self->spectra.array[i] = mli_Spectrum_init();
        }

        chk(mli_SurfaceArray_malloc(&self->surfaces2, rescap.num_surfaces));
        for (i = 0; i < self->surfaces2.size; i++) {
                self->surfaces2.array[i] = mli_BoundaryLayer_Surface_init();
        }

        chk(mli_MediumArray_malloc(&self->media2, rescap.num_media));
        for (i = 0; i < self->media2.size; i++) {
                self->media2.array[i] = mli_BoundaryLayer_Medium_init();
        }

        chk(mli_BoundaryLayer2Array_malloc(
                &self->layers2, rescap.num_boundary_layers));
        for (i = 0; i < self->layers2.size; i++) {
                self->layers2.array[i] = mli_BoundaryLayer2_init();
        }

        return 1;
chk_error:
        mli_Materials_free(self);
        return 0;
}

int mli_Materials_info_fprint(FILE *f, const struct mli_Materials *self)
{
        uint32_t i = 0;
        struct mli_String tmp = mli_String_init();

        fprintf(f, "materials\n");
        fprintf(f, "---------\n");
        fprintf(f, "\n");

        fprintf(f, "    media\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%3s ", "#");
        fprintf(f, "%24s ", "name");
        fprintf(f, "%12s ", "default");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        for (i = 0; i < self->media2.size; i++) {
                struct mli_BoundaryLayer_Medium *medium =
                        &self->media2.array[i];
                fprintf(f, "    ");
                fprintf(f, "% 3d ", i);
                fprintf(f, "%24s ", medium->name.array);

                if (i == self->default_medium) {
                        fprintf(f, "%12s", "True");
                }
                fprintf(f, "\n");
        }
        fprintf(f, "\n");

        fprintf(f, "    surfaces\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        for (i = 0; i < self->surfaces2.size; i++) {
                struct mli_BoundaryLayer_Surface *surface =
                        &self->surfaces2.array[i];
                struct mli_String tmp = mli_String_init();
                chk(mli_BoundaryLayer_Surface_type_to_string(
                        surface->type, &tmp));
                fprintf(f, "    ");
                fprintf(f, "% 3d ", i);
                fprintf(f, "%24s ", surface->name.array);
                fprintf(f, "%24s ", tmp.array);
                fprintf(f, "\n");
        }
        fprintf(f, "\n");

        fprintf(f, "    boundary layers\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        for (i = 0; i < self->layers2.size; i++) {
                struct mli_BoundaryLayer2 *layer = &self->layers2.array[i];
                fprintf(f, "    ");
                fprintf(f, "% 3d ", i);
                fprintf(f, "%24s ", layer->name.array);

                fprintf(f, "%8ld ", layer->inner.medium);
                fprintf(f, "%8ld ", layer->inner.surface);

                fprintf(f, "%8ld ", layer->outer.medium);
                fprintf(f, "%8ld ", layer->outer.surface);

                fprintf(f, "\n");
        }

        mli_String_free(&tmp);
        return 1;
chk_error:
        mli_String_free(&tmp);
        return 0;
}

int mli_Materials__has_surface_name_cstr(
        const struct mli_Materials *self,
        const char *name)
{
        uint64_t i;
        for (i = 0; i < self->surfaces2.size; i++) {
                if (mli_String_equal_cstr(
                            &self->surfaces2.array[i].name, name)) {
                        return 1;
                }
        }
        return 0;
}

int mli_Materials__has_medium_name_cstr(
        const struct mli_Materials *self,
        const char *name)
{
        uint64_t i;
        for (i = 0; i < self->media2.size; i++) {
                if (mli_String_equal_cstr(&self->media2.array[i].name, name)) {
                        return 1;
                }
        }
        return 0;
}

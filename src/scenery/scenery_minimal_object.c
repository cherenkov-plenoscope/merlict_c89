/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "scenery_minimal_object.h"
#include "../chk/chk.h"
#include "../color/color.h"
#include "../vec/vec_random.h"
#include "../prng/prng.h"
#include "../cstr/cstr.h"
#include "../object/object_wavefront.h"
#include "scenery_valid.h"

int mli_Func_malloc_color_spectrum(
        struct mli_Func *self,
        const struct mli_Color color)
{
        /* r: 600nm
         * g: 550nm
         * b: 450nm
         */

        chk(mli_Func_malloc(self, 11));
        self->x[0] = 200e-9;
        self->y[0] = 0.0;

        /* blue peak */
        self->x[1] = 400e-9;
        self->y[1] = 0.0;
        self->x[2] = 450e-9;
        self->y[2] = color.b;
        self->x[3] = 500e-9;
        self->y[3] = 0.0;

        /* green peak */
        self->x[4] = 501e-9;
        self->y[4] = 0.0;
        self->x[5] = 550e-9;
        self->y[5] = color.g;
        self->x[6] = 575e-9;
        self->y[6] = 0.0;

        /* red peak */
        self->x[7] = 576e-9;
        self->y[7] = 0.0;
        self->x[8] = 600e-9;
        self->y[8] = color.r;
        self->x[9] = 650e-9;
        self->y[9] = 0.0;

        self->x[10] = 1200e-9;
        self->y[10] = 0.0;

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

struct mli_Color mli_Color_random_uniform(struct mli_Prng *prng)
{
        struct mli_prng_UniformRange uniform_range;
        struct mli_Color out;
        uniform_range.start = 0.0;
        uniform_range.range = 1.0;

        out = mli_Color_set(
                mli_prng_draw_uniform(uniform_range, prng),
                mli_prng_draw_uniform(uniform_range, prng),
                mli_prng_draw_uniform(uniform_range, prng));
        return out;
}

int mli_Scenery_malloc_minimal_from_wavefront(
        struct mli_Scenery *self,
        const char *path)
{
        uint32_t i, total_num_boundary_layers;
        uint64_t spec;
        struct mli_Prng prng = mli_Prng_init_MT19937(1u);
        struct mli_IO str = mli_IO_init();
        struct mli_MaterialsCapacity mtlcap = mli_MaterialsCapacity_init();
        struct mli_String _path = mli_String_init();
        struct mli_String _mode = mli_String_init();
        struct mli_Spectrum *spectrum = NULL;
        struct mli_Surface *surface = NULL;
        struct mli_Medium *medium = NULL;
        struct mli_BoundaryLayer *layer = NULL;

        mli_Scenery_free(self);

        chk_msg(mli_Geometry_malloc(&self->geometry, 1u, 1u),
                "Failed to malloc geometry.");

        /* set object */
        chk(mli_String_from_cstr(&_path, path));
        chk(mli_String_from_cstr(&_mode, "r"));
        chk_msg(mli_IO_open_file(&str, &_path, &_mode), "Failed to open file.");
        chk_msg(mli_Object_malloc_from_wavefront(
                        &self->geometry.objects[0], &str),
                "Failed to malloc wavefront-object from string.");
        mli_IO_close(&str);

        mli_String_free(&_path);
        mli_String_free(&_mode);

        chk(mli_String_from_cstr(
                &self->geometry.object_names[0], "default-object"));

        /* set reference */
        self->geometry.robjects[0] = 0u;
        self->geometry.robject_ids[0] = 0u;
        self->geometry.robject2root[0] = mli_HomTraComp_set(
                mli_Vec_init(0.0, 0.0, 0.0),
                mli_Quaternion_set_tait_bryan(0.0, 0.0, 0.0));

        /* materials */
        total_num_boundary_layers = self->geometry.objects[0].num_materials;

        mtlcap.num_media = 1u;
        mtlcap.num_boundary_layers = total_num_boundary_layers;
        mtlcap.num_surfaces = total_num_boundary_layers;
        mtlcap.num_spectra = 2u + total_num_boundary_layers;

        chk_msg(mli_Materials_malloc(&self->materials, mtlcap),
                "Failed to malloc materials.");

        spec = 0;

        spectrum = &self->materials.spectra.array[spec];
        chk(mli_String_from_cstr(&spectrum->name, "vacuum_refraction"));
        chk(mli_Func_malloc_constant(
                &spectrum->spectrum, 200e-9, 1200e-9, 1.0));
        spec += 1;

        spectrum = &self->materials.spectra.array[spec];
        chk(mli_String_from_cstr(&spectrum->name, "vacuum_absorption"));
        chk(mli_Func_malloc_constant(
                &spectrum->spectrum, 200e-9, 1200e-9, 0.0));
        spec += 1;

        medium = &self->materials.media.array[0];
        mli_Medium_free(medium);
        chk(mli_String_from_cstr(&medium->name, "vacuum"));
        medium->refraction_spectrum = 0;
        medium->absorption_spectrum = 1;

        for (i = 0u; i < total_num_boundary_layers; i++) {
                spectrum = &self->materials.spectra.array[spec];
                mli_Spectrum_free(spectrum);
                chk(mli_String_from_cstr_fromat(
                        &spectrum->name, "reflection_%06u", i));
                chk(mli_Func_malloc_color_spectrum(
                        &spectrum->spectrum, mli_Color_random_uniform(&prng)));

                surface = &self->materials.surfaces.array[i];
                mli_Surface_free(surface);
                chk(mli_String_from_cstr_fromat(
                        &surface->name, "surface_%06u", i));
                surface->type = MLI_SURFACE_TYPE_COOKTORRANCE;
                surface->data.cooktorrance.reflection_spectrum = spec;
                surface->data.cooktorrance.diffuse_weight = 1.0;
                surface->data.cooktorrance.specular_weight = 0.0;
                surface->data.cooktorrance.roughness = 0.2;

                layer = &self->materials.boundary_layers.array[i];
                mli_BoundaryLayer_free(layer);
                chk(mli_String_from_cstr_fromat(&layer->name, "layer_%06u", i));
                layer->inner.medium = 0;
                layer->inner.surface = i;
                layer->outer.medium = 0;
                layer->outer.surface = i;

                spec += 1;
        }

        chk_msg(mli_GeometryToMaterialMap_malloc(
                        &self->geomap,
                        self->geometry.num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        /* set map */
        for (i = 0u; i < total_num_boundary_layers; i++) {
                mli_GeometryToMaterialMap_set(&self->geomap, 0u, i, i);
        }

        chk_msg(mli_Accelerator_malloc_from_Geometry(
                        &self->accelerator, &self->geometry),
                "Failed to malloc accelerator from geometry.");

        chk_msg(mli_Scenery_valid(self), "Expected scenery to be valid.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

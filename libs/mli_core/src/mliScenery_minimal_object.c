/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_minimal_object.h"
#include "chk.h"

struct mliColor mli_random_color(struct mliPrng *prng)
{
        struct mliRandomUniformRange uniform_8bit_range;
        uniform_8bit_range.start = 0.0;
        uniform_8bit_range.range = 255.0;
        return mliColor_set(
                mli_random_draw_uniform(uniform_8bit_range, prng),
                mli_random_draw_uniform(uniform_8bit_range, prng),
                mli_random_draw_uniform(uniform_8bit_range, prng));
}

int mliScenery_malloc_minimal_from_wavefront(
        struct mliScenery *scenery,
        const char *path)
{
        uint32_t i, total_num_boundary_layers;
        struct mliPrng prng = mliPrng_init_MT19937(1u);
        struct mliIo str = mliIo_init();
        struct mliMaterialsCapacity mtlcap = mliMaterialsCapacity_init();

        mliScenery_free(scenery);

        chk_msg(mliGeometry_malloc(&scenery->geometry, 1u, 1u),
                "Failed to malloc geometry.");

        /* set object */
        chk_msg(mliIo_malloc_from_path(&str, path), "Failed to read file.");
        chk_msg(mli_cstr_assert_only_NUL_LF_TAB_controls((char *)str.cstr),
                "Expected object-wavefront file to be free of "
                "control characters, except [NUL, TAB, LF].");
        chk_msg(mliObject_malloc_from_wavefront(
                        &scenery->geometry.objects[0], (char *)str.cstr),
                "Failed to malloc wavefront-object from string.");
        mliIo_free(&str);
        sprintf(scenery->geometry.object_names[0].cstr, "default-object");

        /* set reference */
        scenery->geometry.robjects[0] = 0u;
        scenery->geometry.robject_ids[0] = 0u;
        scenery->geometry.robject2root[0] = mliHomTraComp_set(
                mliVec_init(0.0, 0.0, 0.0),
                mliQuaternion_set_tait_bryan(0.0, 0.0, 0.0));

        /* materials */
        total_num_boundary_layers = scenery->geometry.objects[0].num_materials;

        mtlcap.num_media = 1u;
        mtlcap.num_boundary_layers = total_num_boundary_layers;
        mtlcap.num_surfaces = total_num_boundary_layers;

        chk_msg(mliMaterials_malloc(&scenery->materials, mtlcap),
                "Failed to malloc materials.");

        sprintf(scenery->materials.medium_names[0].cstr, "vacuum");

        chk(mliFunc_malloc(&scenery->materials.media[0].refraction, 2));
        scenery->materials.media[0].refraction.x[0] = 200e-9;
        scenery->materials.media[0].refraction.x[1] = 1200e-9;
        scenery->materials.media[0].refraction.y[0] = 1.0;
        scenery->materials.media[0].refraction.y[1] = 1.0;

        chk(mliFunc_malloc(&scenery->materials.media[0].absorbtion, 2));
        scenery->materials.media[0].absorbtion.x[0] = 200e-9;
        scenery->materials.media[0].absorbtion.x[1] = 1200e-9;
        scenery->materials.media[0].absorbtion.y[0] = 0.0;
        scenery->materials.media[0].absorbtion.y[1] = 0.0;

        for (i = 0u; i < total_num_boundary_layers; i++) {
                scenery->materials.surfaces[i].material = MLI_MATERIAL_PHONG;
                scenery->materials.surfaces[i].color = mli_random_color(&prng);

                chk(mliFunc_malloc(
                        &scenery->materials.surfaces[i].specular_reflection,
                        2));
                scenery->materials.surfaces[i].specular_reflection.x[0] =
                        200e-9;
                scenery->materials.surfaces[i].specular_reflection.x[1] =
                        1200e-9;
                scenery->materials.surfaces[i].specular_reflection.y[0] = 0.0;
                scenery->materials.surfaces[i].specular_reflection.y[1] = 0.0;

                chk(mliFunc_malloc(
                        &scenery->materials.surfaces[i].diffuse_reflection, 2));
                scenery->materials.surfaces[i].diffuse_reflection.x[0] = 200e-9;
                scenery->materials.surfaces[i].diffuse_reflection.x[1] =
                        1200e-9;
                scenery->materials.surfaces[i].diffuse_reflection.y[0] = 1.0;
                scenery->materials.surfaces[i].diffuse_reflection.y[1] = 1.0;

                sprintf(scenery->materials.surface_names[i].cstr,
                        "surface_%06u",
                        i);

                scenery->materials.boundary_layers[i].inner.medium = 0u;
                scenery->materials.boundary_layers[i].outer.medium = 0u;
                scenery->materials.boundary_layers[i].inner.surface = i;
                scenery->materials.boundary_layers[i].outer.surface = i;
                sprintf(scenery->materials.boundary_layer_names[i].cstr,
                        "boundary_layer_%06u",
                        i);
        }

        chk_msg(mliGeometryToMaterialMap_malloc(
                        &scenery->geomap,
                        scenery->geometry.num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        /* set map */
        for (i = 0u; i < total_num_boundary_layers; i++) {
                mliGeometryToMaterialMap_set(&scenery->geomap, 0u, i, i);
        }

        chk_msg(mliAccelerator_malloc_from_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Failed to malloc accelerator from geometry.");

        chk_msg(mliScenery_valid(scenery), "Expected scenery to be valid.");
        return 1;
error:
        return 0;
}

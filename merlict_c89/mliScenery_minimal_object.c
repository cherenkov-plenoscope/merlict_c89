/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_minimal_object.h"
#include "mli_debug.h"

struct mliColor mli_random_color(struct mliMT19937 *prng)
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
        struct mliMT19937 prng = mliMT19937_init(1u);
        struct mliString str = mliString_init();
        struct mliMaterialsCapacity mtlcap = mliMaterialsCapacity_init();

        mliScenery_free(scenery);

        mli_check(
                mliGeometry_malloc(&scenery->geometry, 1u, 1u),
                "Failed to malloc geometry.");

        /* set object */
        mli_check(
                mliString_malloc_from_path(&str, path), "Failed to read file.");
        mli_check(
                mli_string_assert_only_NUL_LF_TAB_controls(str.c_str),
                "Expected object-wavefront file to be free of "
                "control characters, except [NUL, TAB, LF].");
        mli_check(
                mliObject_malloc_from_wavefront(
                        &scenery->geometry.objects[0], str.c_str),
                "Failed to malloc wavefront-object from string.");
        mliString_free(&str);
        sprintf(scenery->geometry.object_names[0].c_str, "default-object");

        /* set reference */
        scenery->geometry.robjects[0] = 0u;
        scenery->geometry.robject_ids[0] = 0u;
        scenery->geometry.robject2root[0] = mliHomTraComp_set(
                mliVec_set(0.0, 0.0, 0.0),
                mliQuaternion_set_tait_bryan(0.0, 0.0, 0.0));

        /* materials */
        total_num_boundary_layers = scenery->geometry.objects[0].num_materials;

        mtlcap.num_media = 1u;
        mtlcap.num_functions = 2u;
        mtlcap.num_boundary_layers = total_num_boundary_layers;
        mtlcap.num_colors = total_num_boundary_layers;
        mtlcap.num_surfaces = total_num_boundary_layers;

        mli_check(
                mliMaterials_malloc(&scenery->materials, mtlcap),
                "Failed to malloc materials.");

        scenery->materials.functions[0] = mliFunc_init();
        mli_check(
                mliFunc_malloc(&scenery->materials.functions[0], 2),
                "Failed to malloc zero function.");
        scenery->materials.functions[0].x[0] = 200e-9;
        scenery->materials.functions[0].x[1] = 1200e-9;
        scenery->materials.functions[0].y[0] = 0.0;
        scenery->materials.functions[0].y[1] = 0.0;
        sprintf(scenery->materials.function_names[0].c_str, "zero");

        scenery->materials.functions[1] = mliFunc_init();
        mli_check(
                mliFunc_malloc(&scenery->materials.functions[1], 2),
                "Failed to malloc unity function.");
        scenery->materials.functions[1].x[0] = 200e-9;
        scenery->materials.functions[1].x[1] = 1200e-9;
        scenery->materials.functions[1].y[0] = 1.0;
        scenery->materials.functions[1].y[1] = 1.0;
        sprintf(scenery->materials.function_names[1].c_str, "unity");

        scenery->materials.media[0].refraction = 1u;
        scenery->materials.media[0].absorbtion = 0u;

        for (i = 0u; i < total_num_boundary_layers; i++) {
                scenery->materials.colors[i] = mli_random_color(&prng);
                sprintf(scenery->materials.color_names[i].c_str,
                        "color_%06u",
                        i);

                scenery->materials.surfaces[i].material = MLI_MATERIAL_PHONG;
                scenery->materials.surfaces[i].specular_reflection = 0u;
                scenery->materials.surfaces[i].diffuse_reflection = 1u;
                scenery->materials.surfaces[i].color = i;
                sprintf(scenery->materials.surface_names[i].c_str,
                        "surface_%06u",
                        i);

                scenery->materials.boundary_layers[i].inner.medium = 0u;
                scenery->materials.boundary_layers[i].outer.medium = 0u;
                scenery->materials.boundary_layers[i].inner.surface = i;
                scenery->materials.boundary_layers[i].outer.surface = i;
                sprintf(scenery->materials.boundary_layer_names[i].c_str,
                        "boundary_layer_%06u",
                        i);
        }

        mli_check(
                mliGeometryToMaterialMap_malloc(
                        &scenery->geomap,
                        scenery->geometry.num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        /* set map */
        for (i = 0u; i < total_num_boundary_layers; i++) {
                mliGeometryToMaterialMap_set(&scenery->geomap, 0u, i, i);
        }

        mli_check(
                mliAccelerator_malloc_from_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Failed to malloc accelerator from geometry.");

        mli_check(mliScenery_valid(scenery), "Expected scenery to be valid.");
        return 1;
error:
        return 0;
}

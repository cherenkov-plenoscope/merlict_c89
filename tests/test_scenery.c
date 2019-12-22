/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliScenery, mliMesh_malloc_from_object_file") {
    uint64_t i;
    mliScenery scenery = mliScenery_init();
    mliScenery scenery_back = mliScenery_init();
    mliMesh diff_cube_sphere = mliMesh_init();
    mliColor red = {255., 0., 0.};
    mliColor blue = {0., 0., 255.};
    mliColor green = {0., 255., 0.};
    mliColor violet = {255., 0., 255.};
    scenery.num_functions = 1u;
    scenery.num_colors = 4u;
    scenery.num_surfaces = 4u;
    scenery.num_spherical_cap_hex = 1u;
    scenery.num_spheres = 1u;
    scenery.num_cylinders = 1u;
    scenery.num_hexagons = 1u;
    scenery.num_bicircleplanes = 1u;
    scenery.num_discs = 1u;

    CHECK(mliMesh_malloc_from_object_file(
        "tests/resources/diff_cube_sphere.off",
        &diff_cube_sphere));

    scenery.num_vertices = diff_cube_sphere.num_vertices;
    scenery.num_triangles = diff_cube_sphere.num_faces;

    CHECK(mliScenery_malloc(&scenery));

    scenery.functions[0].num_points = 2u;
    CHECK(mliFunc_malloc(&scenery.functions[0]));
    scenery.functions[0].x[0] = 200.e-9;
    scenery.functions[0].x[1] = 1200.e-9;
    scenery.functions[0].y[0] = 0.;
    scenery.functions[0].y[1] = 0.;

    scenery.colors[0] = red;
    scenery.colors[1] = blue;
    scenery.colors[2] = green;
    scenery.colors[3] = violet;

    scenery.surfaces[0].color = 0u;
    scenery.surfaces[0].material = MLI_MATERIAL_PHONG;
    scenery.surfaces[0].boundary_layer_specular_reflection = 0u;
    scenery.surfaces[0].boundary_layer_diffuse_reflection = 0u;
    scenery.surfaces[0].boundary_layer_transmission = 0u;
    scenery.surfaces[0].medium_refraction = 0u;
    scenery.surfaces[0].medium_absorbtion = 0u;

    scenery.surfaces[1].color = 1u;
    scenery.surfaces[1].material = MLI_MATERIAL_PHONG;
    scenery.surfaces[1].boundary_layer_specular_reflection = 0u;
    scenery.surfaces[1].boundary_layer_diffuse_reflection = 0u;
    scenery.surfaces[1].boundary_layer_transmission = 0u;
    scenery.surfaces[1].medium_refraction = 0u;
    scenery.surfaces[1].medium_absorbtion = 0u;

    scenery.surfaces[2].color = 2u;
    scenery.surfaces[2].material = MLI_MATERIAL_PHONG;
    scenery.surfaces[2].boundary_layer_specular_reflection = 0u;
    scenery.surfaces[2].boundary_layer_diffuse_reflection = 0u;
    scenery.surfaces[2].boundary_layer_transmission = 0u;
    scenery.surfaces[2].medium_refraction = 0u;
    scenery.surfaces[2].medium_absorbtion = 0u;

    scenery.surfaces[3].color = 3u;
    scenery.surfaces[3].material = MLI_MATERIAL_PHONG;
    scenery.surfaces[3].boundary_layer_specular_reflection = 0u;
    scenery.surfaces[3].boundary_layer_diffuse_reflection = 0u;
    scenery.surfaces[3].boundary_layer_transmission = 0u;
    scenery.surfaces[3].medium_refraction = 0u;
    scenery.surfaces[3].medium_absorbtion = 0u;

    mliVec_ncpy(
        diff_cube_sphere.vertices,
        scenery.vertices,
        diff_cube_sphere.num_vertices);

    mliFace_ncpy(
        diff_cube_sphere.faces,
        scenery.triangles,
        diff_cube_sphere.num_faces);

    for (i = 0; i < scenery.num_triangles; i++) {
        scenery.triangles_surfaces[i].outer = 0u;
        scenery.triangles_surfaces[i].inner = 0u;}

    /* spherical_cap_hex */
    scenery.spherical_cap_hex[0].curvature_radius = 10*2.;
    scenery.spherical_cap_hex[0].inner_hex_radius = 3.3;
    scenery.spherical_cap_hex_T[0].trans = mliVec_set(0., -2, 0);
    scenery.spherical_cap_hex_T[0].rot =
        mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 0.),
            0.);
    scenery.spherical_cap_hex_surfaces[0].outer = 2u;
    scenery.spherical_cap_hex_surfaces[0].inner = 2u;

    /* spheres */
    scenery.spheres[0].radius = 2.5;
    scenery.spheres_surfaces[0].outer = 1u;
    scenery.spheres_surfaces[0].inner = 1u;
    scenery.spheres_T[0].trans = mliVec_set(0., 0., 0.);
    scenery.spheres_T[0].rot =
        mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 0., 0.),
            0.);

    /* cylinders */
    scenery.cylinders[0].radius = 1.;
    scenery.cylinders[0].length = 7.;
    scenery.cylinders_surfaces[0].outer = 3u;
    scenery.cylinders_surfaces[0].inner = 3u;
    scenery.cylinders_T[0].trans = mliVec_set(0., 3., 0.);
    scenery.cylinders_T[0].rot =
        mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(30));

    /* hexagons */
    scenery.hexagons[0].inner_radius = 1.;
    scenery.hexagons_surfaces[0].outer = 0u;
    scenery.hexagons_surfaces[0].inner = 0u;
    scenery.hexagons_T[0].trans = mliVec_set(2., 3., 0.);
    scenery.hexagons_T[0].rot =
        mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(-30));

    /* bicircleplanes */
    scenery.bicircleplanes[0].y_width = 2.;
    scenery.bicircleplanes[0].x_height = 1.;
    scenery.bicircleplanes_surfaces[0].outer = 0u;
    scenery.bicircleplanes_surfaces[0].inner = 0u;
    scenery.bicircleplanes_T[0].trans = mliVec_set(0., 2., 5.);
    scenery.bicircleplanes_T[0].rot =
        mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(0.));

    /* discs */
    scenery.discs[0].radius = 2.;
    scenery.discs_surfaces[0].outer = 1u;
    scenery.discs_surfaces[0].inner = 1u;
    scenery.discs_T[0].trans = mliVec_set(0., -2., -5.);
    scenery.discs_T[0].rot =
        mliQuaternion_set_rotaxis_and_angle(
            mliVec_set(0., 1., 0.),
            mli_deg2rad(0.));

    mliScenery_write_to_path(&scenery, "tests/resources/scn1.mli.tmp");
    mliScenery_read_from_path(&scenery_back, "tests/resources/scn1.mli.tmp");

    CHECK(mliScenery_is_equal(&scenery, &scenery_back));

    mliScenery_free(&scenery);
    mliScenery_free(&scenery_back);
}

CASE("mliScenery, render image") {
    mliScenery scenery = mliScenery_init();
    mliOcTree octree;
    mliCamera camera;
    mliImage img = mliImage_init();
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    octree = mliOcTree_from_scenery(&scenery);
    /* mliNode_print(&octree.root, 0); */

    camera.position.x = 0.;
    camera.position.y = 0.;
    camera.position.z = -20.;
    camera.rotation.x = 0.;
    camera.rotation.y = 0.;
    camera.rotation.z = 0.;
    camera.field_of_view = mli_deg2rad(80.);

    img.num_cols = 640u;
    img.num_rows = 480u;
    CHECK(mliImage_malloc(&img));
    mliCamera_render_image(&camera, &scenery, &octree, &img);
    mliImage_write_to_ppm(&img, "tests/resources/scn1.ppm.tmp");

    mliImage_free(&img);
    mliOcTree_free(&octree);
    mliScenery_free(&scenery);
}

CASE("scenery indexes are valid") {
    mliScenery scenery = mliScenery_init();
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    CHECK(mliScenery_valid(&scenery));     /* <--- to be tested */
    mliScenery_free(&scenery);
}

CASE("mliScenery_overlap_obb") {
    mliOBB obb;
    uint64_t i;
    uint64_t num_surface_entities;
    mliScenery scenery = mliScenery_init();
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    num_surface_entities = scenery.num_triangles;
    num_surface_entities += scenery.num_spherical_cap_hex;
    num_surface_entities += scenery.num_spheres;
    num_surface_entities += scenery.num_cylinders;
    num_surface_entities += scenery.num_hexagons;
    num_surface_entities += scenery.num_bicircleplanes;
    num_surface_entities += scenery.num_discs;

    CHECK(mliScenery_num_objects(&scenery) == num_surface_entities);

    obb.lower = mliVec_set(-1e2, -1e2, -1e2);
    obb.upper = mliVec_set(1e2, 1e2, 1e2);
    for (i = 0u; i <= mliScenery_num_objects(&scenery); i++) {
        CHECK(mliScenery_overlap_obb(
            &scenery,
            i,
            obb));
    }

    mliScenery_free(&scenery);
}

CASE("mliScenery_outermost_obb") {
    mliScenery scenery = mliScenery_init();
    mliOBB obb;
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    CHECK(mliScenery_valid(&scenery));

    obb = mliScenery_outermost_obb(&scenery);
    CHECK_MARGIN(obb.lower.x, -7.5, 1e-7);
    CHECK_MARGIN(obb.lower.y, -7.5, 1e-7);
    CHECK_MARGIN(obb.lower.z, -7.5, 1e-7);
    CHECK_MARGIN(obb.upper.x, +7.5, 1e-7);
    CHECK_MARGIN(obb.upper.y, +7.5, 1e-7);
    CHECK_MARGIN(obb.upper.z, +7.5, 1e-7);

    mliScenery_free(&scenery);
}

CASE("read non existing file") {
    mliScenery scenery = mliScenery_init();
    CHECK(!mliScenery_read_from_path(&scenery, "does_not_exist.tmp"));
}

CASE("scenery intersection interface -> sphere") {
    mliScenery scenery = mliScenery_init();
    mliIntersection isec;
    int hit = 0;
    uint64_t sphere_idx;
    double sphere_radius = 2.5;
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");

    sphere_idx = scenery.num_triangles +
        scenery.num_spherical_cap_hex;

    hit = mliScenery_intersection(
        &scenery,
        mliRay_set(
            mliVec_set(0., 0., -10.),
            mliVec_set(0. ,0., 1.)),
        sphere_idx,
        &isec);

    CHECK(hit);
    CHECK(isec.object_idx == sphere_idx);
    CHECK(
        mliVec_equal_margin(
            isec.position,
            mliVec_set(0., 0., -sphere_radius),
            1e-6));
    CHECK(
        mliVec_equal_margin(
            isec.surface_normal,
            mliVec_set(0., 0., -1),
            1e-6));
    CHECK(isec.from_outside_to_inside);
    CHECK_MARGIN(isec.distance_of_ray, 10 - sphere_radius, 1e-6);

    mliScenery_free(&scenery);
}

CASE("mliOcTree_from_scenery") {
    mliScenery scenery = mliScenery_init();
    mliOcTree octree;
    mliIntersection isec;
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    octree = mliOcTree_from_scenery(&scenery);

    /*mliNode_print(&tree, 0u);*/
    /*
    fprintf(stderr ,"num_nodes: %u\n", mliNode_num_nodes(&tree));
    fprintf(
        stderr,
        "capacity_objects: %u\n",
        mliNode_capacity_objects(&tree));*/

    mli_ray_octree_traversal(
        &scenery,
        &octree,
        mliRay_set(
            mliVec_set(0.1 ,2.5, 10.),
            mliVec_set(0. ,0., -1.)),
        &isec);

    mliOcTree_free(&octree);
    mliScenery_free(&scenery);
}

CASE("mliScenery asymetric") {
    mliScenery scenery = mliScenery_init();
    mliVec offset;
    uint64_t i;
    scenery.num_functions = 1u;
    scenery.num_colors = 1u;
    scenery.num_surfaces = 1u;
    scenery.num_spherical_cap_hex = 0u;
    scenery.num_spheres = 50u;
    scenery.num_vertices = 0u;
    scenery.num_triangles = 0u;

    CHECK(mliScenery_malloc(&scenery));

    scenery.functions[0].num_points = 2u;
    CHECK(mliFunc_malloc(&scenery.functions[0]));
    scenery.functions[0].x[0] = 200.e-9;
    scenery.functions[0].x[1] = 1200.e-9;
    scenery.functions[0].y[0] = 0.;
    scenery.functions[0].y[1] = 0.;

    scenery.colors[0] = mliColor_set(255., 0., 0.);

    scenery.surfaces[0].color = 0u;
    scenery.surfaces[0].material = MLI_MATERIAL_PHONG;
    scenery.surfaces[0].boundary_layer_specular_reflection = 0u;
    scenery.surfaces[0].boundary_layer_diffuse_reflection = 0u;
    scenery.surfaces[0].boundary_layer_transmission = 0u;
    scenery.surfaces[0].medium_refraction = 0u;
    scenery.surfaces[0].medium_absorbtion = 0u;
    offset = mliVec_set(0, 0, -8);
    for (i = 0; i < scenery.num_spheres; i ++) {
        const double phi = 2.*MLI_PI*(double)(i)/scenery.num_spheres;
        const double ii = (double)(i)/scenery.num_spheres;
        const double zoff = 0.5 - ii;
        scenery.spheres[i].radius = 0.1;
        scenery.spheres_surfaces[i].outer = 0u;
        scenery.spheres_surfaces[i].inner = 0u;
        scenery.spheres_T[i].trans = mliVec_set(
            5*cos(phi) + offset.x,
            -5*sin(phi) + offset.y,
            5*zoff + offset.z);
        scenery.spheres_T[i].rot =
            mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 0.),
                0.);
    }
    mliScenery_write_to_path(&scenery, "tests/resources/scn_asym.mli.tmp");
    mliScenery_free(&scenery);
}

CASE("render image asymetric scenery") {
    mliScenery scenery = mliScenery_init();
    mliOcTree octree;
    mliCamera camera;
    mliImage img = mliImage_init();
    mliScenery_read_from_path(&scenery, "tests/resources/scn_asym.mli.tmp");
    octree = mliOcTree_from_scenery(&scenery);
    CHECK(mliNode_num_nodes(&octree.root) == 9);
    /* mliNode_print(&octree.root, 0); */
    camera.position.x = 0.;
    camera.position.y = 0.;
    camera.position.z = -20.;
    camera.rotation.x = 0.;
    camera.rotation.y = 0.;
    camera.rotation.z = 0.;
    camera.field_of_view = mli_deg2rad(80.);

    img.num_cols = 640u;
    img.num_rows = 480u;
    CHECK(mliImage_malloc(&img));
    mliCamera_render_image(&camera, &scenery, &octree, &img);
    mliImage_write_to_ppm(&img, "tests/resources/scn_asym.ppm.tmp");

    mliImage_free(&img);
    mliOcTree_free(&octree);
    mliScenery_free(&scenery);
}

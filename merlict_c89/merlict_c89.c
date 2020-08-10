/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_version.c"
#include "mli_debug.c"
#include "mli_math.c"
#include "mliFunc.c"
#include "mli_dual_circle_prism.c"
#include "mliMT19937.c"
#include "mli_random.c"
#include "mliVec.c"
#include "mliRay.c"
#include "mliEdge.c"
#include "mliRotMat.c"
#include "mliQuaternion.c"
#include "mliHomTra.c"
#include "mliOBB.c"
#include "mliCube.c"
#include "mliIntersection.c"
#include "mliFace.c"
#include "mliMesh.c"
#include "mliTriangle_intersection.c"
#include "mliColor.c"
#include "mliPixels.c"
#include "mliImage.c"
#include "mliQuadraticEquation.c"
#include "mliDynArray.c"
#include "mliDynArray_template.c"
#include "mliDynArray_channels_template.c"
#include "mli_variable_length_integer.c"
#include "mliMap.c"
#include "mliBoundaryLayer.c"
#include "mliMedium.c"
#include "mliSurface.c"
#include "mli_primitive_codes.c"
#include "mliFrame.c"
#include "mliSceneryResources.c"
#include "mliSceneryResources_write.c"
#include "mliSceneryResources_read.c"
#include "mliSceneryResources_equal.c"
#include "mliSceneryResources_valid.c"
#include "mliUserScenery.c"
#include "mliScenery.c"
#include "mliScenery_write.c"
#include "mliScenery_read.c"
#include "mliScenery_equal.c"
#include "mliScenery_valid.c"
#include "mliScenery_object_interface.c"
#include "mliRay_OBB.c"
#include "mliHexagonalPrismZ.c"
#include "mliOuterPrismBound.c"
#include "mliVec_OBB.c"
#include "mliTriangle_OBB.c"
#include "mli_inside_polygon.c"
#include "mli_intersection_and_scenery.c"
#include "mliSphere.c"
#include "mliSphere_OBB.c"
#include "mliSphere_intersection.c"
#include "mli_spherical_cap.c"
#include "mliSphericalCapHex.c"
#include "mliSphericalCapHex_OBB.c"
#include "mliSphericalCapHex_intersection.c"
#include "mliCylinder.c"
#include "mliCylinder_OBB.c"
#include "mli_cylindrical_prism.c"
#include "mliCylinder_intersection.c"
#include "mliHexagon.c"
#include "mliHexagon_OBB.c"
#include "mliHexagon_intersection.c"
#include "mliBiCirclePlane.c"
#include "mliBiCirclePlane_OBB.c"
#include "mliBiCirclePlane_intersection.c"
#include "mliDisc.c"
#include "mliDisc_OBB.c"
#include "mliDisc_intersection.c"
#include "mliOctOverlaps.c"
#include "mliTmpOcTree.c"
#include "mliOcTree.c"
#include "mliOcTree_write.c"
#include "mliOcTree_read.c"
#include "mliOcTree_equal.c"
#include "mli_ray_octree_traversal.c"
#include "mli_from_outside_to_inside.c"
#include "mli_xyplane.c"
#include "mliTracer.c"
#include "mliCamera.c"
#include "mli_string_to.c"
#include "jsmn.c"
#include "mli_json.c"
#include "mliScenery_json.c"
#include "mliFunc_malloc_from_json_token.c"
#include "mliVec_from_json_token.c"
#include "mli_frame_to_scenery.c"
#include "mliPhoton.c"
#include "mliPhotonInteraction.c"
#include "mliDynPhoton.c"
#include "mliDynPhotonInteraction.c"
#include "mli_lambertian_cosine_law.c"
#include "mliFresnel.c"
#include "mli_photon_sources.c"
#include "mli_photon_propagation.c"
#include "mli_lens_maker.c"
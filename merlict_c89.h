/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MERLICT_MERLICT_C89_H_
#define MERLICT_MERLICT_C89_H_

#include "mli_version.h"

#include "mli_debug.h"
#include "mli_math.h"
#include "mliFunc.h"
#include "mli_dual_circle_prism.h"
#include "mliMT19937.h"
#include "mli_random.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliEdge.h"
#include "mliRotMat.h"
#include "mliQuaternion.h"
#include "mliHomTra.h"
#include "mliOBB.h"
#include "mliCube.h"
#include "mliIntersection.h"
#include "mliFace.h"
#include "mliMesh.h"
#include "mliTriangle_intersection.h"
#include "mliColor.h"
#include "mliPixels.h"
#include "mliImage.h"
#include "mliQuadraticEquation.h"

#include "mliDynArray.h"
#include "mliDynArray_template.h"
#include "mliDynArray_channels_template.h"

#include "mli_variable_length_integer.h"
#include "mliMap.h"

#include "mliBoundaryLayer.h"
#include "mliMedium.h"
#include "mliSurface.h"

#include "mli_primitive_codes.h"
#include "mliFrame.h"
#include "mliSceneryResources.h"
#include "mliSceneryResources_write.h"
#include "mliSceneryResources_read.h"
#include "mliSceneryResources_equal.h"
#include "mliSceneryResources_valid.h"
#include "mliUserScenery.h"
#include "mliScenery.h"
#include "mliScenery_write.h"
#include "mliScenery_read.h"
#include "mliScenery_equal.h"
#include "mliScenery_valid.h"
#include "mliScenery_object_interface.h"

#include "mliRay_OBB.h"
#include "mliHexagonalPrismZ.h"
#include "mliOuterPrismBound.h"

#include "mliVec_OBB.h"
#include "mliTriangle_OBB.h"
#include "mli_inside_polygon.h"

#include "mli_intersection_and_scenery.h"

#include "mliSphere.h"
#include "mliSphere_OBB.h"
#include "mliSphere_intersection.h"

#include "mli_spherical_cap.h"
#include "mliSphericalCapHex.h"
#include "mliSphericalCapHex_OBB.h"
#include "mliSphericalCapHex_intersection.h"

#include "mliCylinder.h"
#include "mliCylinder_OBB.h"
#include "mli_cylindrical_prism.h"
#include "mliCylinder_intersection.h"

#include "mliHexagon.h"
#include "mliHexagon_OBB.h"
#include "mliHexagon_intersection.h"

#include "mliBiCirclePlane.h"
#include "mliBiCirclePlane_OBB.h"
#include "mliBiCirclePlane_intersection.h"

#include "mliDisc.h"
#include "mliDisc_OBB.h"
#include "mliDisc_intersection.h"

#include "mliOctOverlaps.h"
#include "mliTmpOcTree.h"
#include "mliOcTree.h"
#include "mliOcTree_write.h"
#include "mliOcTree_read.h"
#include "mliOcTree_equal.h"
#include "mli_ray_octree_traversal.h"
#include "mli_from_outside_to_inside.h"
#include "mli_xyplane.h"

#include "mliTracer.h"
#include "mliCamera.h"

#include "mli_string_to.h"
#include "jsmn.h"
#include "mli_json.h"
#include "mliScenery_json.h"
#include "mliFunc_malloc_from_json_token.h"
#include "mliVec_from_json_token.h"

#include "mli_frame_to_scenery.h"

#include "mliPhoton.h"
#include "mliPhotonInteraction.h"
#include "mliDynPhoton.h"
#include "mliDynPhotonInteraction.h"

#include "mli_lambertian_cosine_law.h"
#include "mliFresnel.h"
#include "mli_photon_sources.h"
#include "mli_photon_propagation.h"
#include "mli_lens_maker.h"

#endif

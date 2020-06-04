/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MERLICT_MERLICT_C89_H_
#define MERLICT_MERLICT_C89_H_

#include "mli_testing.h"
#include "mli_debug.h"
#include "mli_math.h"
#include "mli_dual_circle_prism.h"
#include "mliMT19937.h"
#include "mli_random.h"
#include "mliVec.h"
#include "mliIntersection.h"
#include "mliRotMat.h"
#include "mliQuaternion.h"
#include "mliRay.h"
#include "mliFace.h"
#include "mliMesh.h"
#include "mliFunc.h"
#include "mliTriangle_intersection.h"
#include "mliColor.h"
#include "mliImage.h"
#include "mliPixels.h"
#include "mliQuadraticEquation.h"
#include "mliHomTra.h"
#include "mliCamera.h"
#include "mliBoundaryLayer.h"

#include "mliScenery.h"
#include "mliScenery_write.h"
#include "mliScenery_read.h"
#include "mliScenery_equal.h"
#include "mliScenery_valid.h"
#include "mliScenery_object_interface.h"

#include "mliTracer.h"
#include "mliRay_OBB.h"
#include "mliHexagonalPrismZ.h"
#include "mliSphericalCapHex.h"
#include "mliOuterPrismBound.h"
#include "mliDynArray.h"
#include "mliDynArray_template.h"
#include "mliDynArray_channels_template.h"
#include "mliVec_OBB.h"
#include "mliTriangle_OBB.h"

#include "mli_intersection_and_scenery.h"

#include "mli_inside_polygon.h"

#include "mliCylinder.h"
#include "mliCylinder_OBB.h"
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

#include "mliTmpOcTree.h"
#include "mliOcTree.h"
#include "mliOcTree_write.h"
#include "mliOcTree_read.h"
#include "mliOcTree_equal.h"
#include "mli_ray_octree_traversal.h"
#include "mli_from_outside_to_inside.h"
#include "mli_xyplane.h"

#include "mli_string_to.h"
#include "mli_json.h"
#include "mliScenery_json.h"
#include "mliFunc_malloc_from_json_token.h"
#include "mliVec_from_json_token.h"

#include "mliMap.h"
#include "mliFrame.h"
#include "mliUserScenery.h"
#include "mli_frame_to_scenery.h"

#include "mli_lambertian_cosine_law.h"
#include "mliFresnel.h"
#include "mliDynPhotonInteraction.h"
#include "mliPhoton.h"
#include "mliDynPhoton.h"
#include "mli_photon_sources.h"
#include "mliPhotonInteraction.h"
#include "mli_photon_propagation.h"

#include "mli_lens_maker.h"

#endif

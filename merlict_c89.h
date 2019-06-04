/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MERLICT_MERLICT_C89_H_
#define MERLICT_MERLICT_C89_H_

#include "mli_debug.h"
#include "mliMath.h"
#include "mli_dual_circle_prism.h"
#include "mliMT19937.h"
#include "mliVec.h"
#include "mliRotMat.h"
#include "mliQuaternion.h"
#include "mliRay.h"
#include "mliUnitTest.h"
#include "mliMesh.h"
#include "mliFunc.h"
#include "mliTriangle_intersection.h"
#include "mliColor.h"
#include "mliImage.h"
#include "mliQuadraticEquation.h"
#include "mliHomTra.h"
#include "mliCamera.h"
#include "mliScenery.h"
#include "mliScenery_object_interface.h"
#include "mliTracer.h"
#include "mliRay_OBB.h"
#include "mliHexagonalPrismZ.h"
#include "mliSphericalCapHex.h"
#include "mliOuterPrismBound.h"
#include "mliVector.h"
#include "mliVec_OBB.h"
#include "mliTriangle_OBB.h"

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

#include "mliOcTree.h"
#include "mli_ray_octree_traversal.h"
#include "mli_from_outside_to_inside.h"
#include "mli_xyplane.h"

#include "jsmn.h"

#endif

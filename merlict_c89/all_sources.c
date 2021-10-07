/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_version.c"

#include "mli_math.c"
#include "mliMagicId.c"
#include "mliGeometryId.c"
#include "mliFunc.c"
#include "mliFunc_json.c"
#include "mliFunc_serialize.c"
#include "mliMT19937.c"
#include "mliPCG32.c"
#include "mliPrng.c"
#include "mli_random.c"
#include "mliName.c"
#include "mliVec.c"
#include "mliVec_json.c"
#include "mliRay.c"
#include "mliRotMat.c"
#include "mliQuaternion.c"
#include "mliHomTra.c"
#include "mliOBB.c"
#include "mliCube.c"
#include "mliIntersection.c"
#include "mliIntersectionSurfaceNormal.c"
#include "mliFace.c"
#include "mli_quadratic_equation.c"
#include "mli_barycentric.c"
#include "mliObject.c"
#include "mliObject_valid.c"
#include "mliObject_serialize.c"
#include "mliObject_OBB.c"
#include "mliObject_wavefront.c"
#include "mliTriangle_intersection.c"
#include "mliColor.c"
#include "mliColor_json.c"
#include "mliPixels.c"
#include "mliPixelWalk.c"
#include "mliImage.c"
#include "mliImage_ppm.c"
#include "mliImage_print.c"
#include "mliDynArray_template.c"
#include "mliDynMap.c"
#include "mliDynMap_json.c"
#include "mliBoundaryLayer.c"
#include "mliMedium.c"
#include "mliMedium_serialize.c"
#include "mliMedium_json.c"
#include "mliSurface.c"
#include "mliSurface_json.c"
#include "mliFrame.c"
#include "mliFrame_json.c"
#include "mliMaterials.c"
#include "mliMaterials_serialize.c"
#include "mliMaterials_equal.c"
#include "mliMaterials_valid.c"
#include "mliUserScenery.c"
#include "mliUserScenery_json.c"
#include "mliGeometry.c"
#include "mliGeometry_equal.c"
#include "mliGeometry_valid.c"
#include "mliGeometry_serialize.c"
#include "mliGeometry_OBB.c"
#include "mliAccelerator.c"
#include "mliAccelerator_serialize.c"
#include "mliAccelerator_valid.c"
#include "mliAccelerator_equal.c"
#include "mliGeometryToMaterialMap.c"
#include "mliGeometryToMaterialMap_equal.c"
#include "mliGeometryToMaterialMap_valid.c"
#include "mliGeometryToMaterialMap_serialize.c"
#include "mliScenery.c"
#include "mliScenery_valid.c"
#include "mliScenery_tar.c"
#include "mliScenery_serialize.c"
#include "mliScenery_equal.c"
#include "mliScenery_minimal_object.c"
#include "mliRay_OBB.c"
#include "mliVec_OBB.c"
#include "mliTriangle_OBB.c"
#include "mli_intersection_and_scenery.c"
#include "mliOctOverlaps.c"
#include "mliTmpOcTree.c"
#include "mliOcTree.c"
#include "mliOcTree_serialize.c"
#include "mliOcTree_equal.c"
#include "mliOcTree_valid.c"
#include "mli_ray_octree_traversal.c"
#include "mli_ray_scenery_query.c"
#include "mli_from_outside_to_inside.c"
#include "mliTracer.c"
#include "mliAtmosphere.c"
#include "mliView.c"
#include "mliPinHoleCamera.c"
#include "mliApertureCamera.c"
#include "mli_string_to.c"
#include "mliDynStr.c"
#include "jsmn.c"
#include "mliJson.c"
#include "mliArchive.c"
#include "mliTar.c"
#include "mliQuaternion_json.c"
#include "mli_frame_to_scenery.c"
#include "mliPhoton.c"
#include "mliPhotonInteraction.c"
#include "mliDynPhoton.c"
#include "mliDynPhotonInteraction.c"
#include "mli_lambertian_cosine_law.c"
#include "mliFresnel.c"
#include "mli_photon_sources.c"
#include "mli_photon_propagation.c"

/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_PHONG_H_
#define MLI_BOUNDARYLAYER_SURFACE_PHONG_H_

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG 4000

struct mli_BoundaryLayer_Surface_Phong {
        uint64_t specular_reflection_spectrum;
        uint64_t diffuse_reflection_spectrum;
        double shininess_constant;
};

#endif

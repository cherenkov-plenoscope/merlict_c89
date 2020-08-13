/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliDisc")
{
        struct mliDisc disc;
        struct mliHomTraComp local2root_comp;
        struct mliOBB obb;
        disc.radius = 1.;
        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_tait_bryan(0., 0., 0.);

        obb.lower = mliVec_set(-1, -1, -1.);
        obb.upper = mliVec_set(1., 1., 1.);
        CHECK(mliDisc_has_overlap_obb(disc, local2root_comp, obb));

        obb.lower = mliVec_set(-1, -1, .1);
        obb.upper = mliVec_set(1., 1., 2.1);
        CHECK(!mliDisc_has_overlap_obb(disc, local2root_comp, obb));
}

/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTriangle_OBB.h"

/* Copyright 2019 Sebastian Achim Mueller */

/* Voorhies, Douglas,
 * Triangle-Cube Intersection,
 * Graphics Gems III, p. 236-239, code: p. 521-526 */

/*___________________________________________________________________________*/

/* Which of the six face-plane(s) is point P outside of? */

int64_t __mli_face_plane(struct mliVec p)
{
        int64_t outcode;
        outcode = 0;
        if (p.x > .5)
                outcode |= 0x01;
        if (p.x < -.5)
                outcode |= 0x02;
        if (p.y > .5)
                outcode |= 0x04;
        if (p.y < -.5)
                outcode |= 0x08;
        if (p.z > .5)
                outcode |= 0x10;
        if (p.z < -.5)
                outcode |= 0x20;
        return (outcode);
}

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

/* Which of the twelve edge plane(s) is point P outside of? */

int64_t __mli_bevel_2d(struct mliVec p)
{
        int64_t outcode;
        outcode = 0;
        if (p.x + p.y > 1.0)
                outcode |= 0x001;
        if (p.x - p.y > 1.0)
                outcode |= 0x002;
        if (-p.x + p.y > 1.0)
                outcode |= 0x004;
        if (-p.x - p.y > 1.0)
                outcode |= 0x008;
        if (p.x + p.z > 1.0)
                outcode |= 0x010;
        if (p.x - p.z > 1.0)
                outcode |= 0x020;
        if (-p.x + p.z > 1.0)
                outcode |= 0x040;
        if (-p.x - p.z > 1.0)
                outcode |= 0x080;
        if (p.y + p.z > 1.0)
                outcode |= 0x100;
        if (p.y - p.z > 1.0)
                outcode |= 0x200;
        if (-p.y + p.z > 1.0)
                outcode |= 0x400;
        if (-p.y - p.z > 1.0)
                outcode |= 0x800;
        return (outcode);
}

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

/* Which of the eight corner plane(s) is point P outside of? */

int64_t __mli_bevel_3d(struct mliVec p)
{
        int64_t outcode;
        outcode = 0;
        if ((p.x + p.y + p.z) > 1.5)
                outcode |= 0x01;
        if ((p.x + p.y - p.z) > 1.5)
                outcode |= 0x02;
        if ((p.x - p.y + p.z) > 1.5)
                outcode |= 0x04;
        if ((p.x - p.y - p.z) > 1.5)
                outcode |= 0x08;
        if ((-p.x + p.y + p.z) > 1.5)
                outcode |= 0x10;
        if ((-p.x + p.y - p.z) > 1.5)
                outcode |= 0x20;
        if ((-p.x - p.y + p.z) > 1.5)
                outcode |= 0x40;
        if ((-p.x - p.y - p.z) > 1.5)
                outcode |= 0x80;
        return (outcode);
}

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

/* Test the point "alpha" of the way from P1 to P2 */
/* See if it is on a face of the cube              */
/* Consider only faces in "mask"                   */

int64_t __mli_check_point(
        struct mliVec p1,
        struct mliVec p2,
        double alpha,
        int64_t mask)
{
        struct mliVec plane_point;
        plane_point.x = MLI_LERP(alpha, p1.x, p2.x);
        plane_point.y = MLI_LERP(alpha, p1.y, p2.y);
        plane_point.z = MLI_LERP(alpha, p1.z, p2.z);
        return (__mli_face_plane(plane_point) & mask);
}

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

/* Compute intersection of P1 --> P2 line segment with face planes */
/* Then test intersection point to see if it is on cube face       */
/* Consider only face planes in "outcode_diff"                     */
/* Note: Zero bits in "outcode_diff" means face line is outside of */

int64_t __mli_check_line(
        struct mliVec p1,
        struct mliVec p2,
        int64_t outcode_diff)
{
        if ((0x01 & outcode_diff) != 0)
                if (__mli_check_point(
                            p1, p2, (0.5f - p1.x) / (p2.x - p1.x), 0x3e) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        if ((0x02 & outcode_diff) != 0)
                if (__mli_check_point(
                            p1, p2, (-0.5f - p1.x) / (p2.x - p1.x), 0x3d) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        if ((0x04 & outcode_diff) != 0)
                if (__mli_check_point(
                            p1, p2, (0.5f - p1.y) / (p2.y - p1.y), 0x3b) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        if ((0x08 & outcode_diff) != 0)
                if (__mli_check_point(
                            p1, p2, (-0.5f - p1.y) / (p2.y - p1.y), 0x37) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        if ((0x10 & outcode_diff) != 0)
                if (__mli_check_point(
                            p1, p2, (0.5f - p1.z) / (p2.z - p1.z), 0x2f) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        if ((0x20 & outcode_diff) != 0)
                if (__mli_check_point(
                            p1, p2, (-0.5f - p1.z) / (p2.z - p1.z), 0x1f) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        return (MLI_OUTSIDE);
}

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

/* Test if 3D point is inside 3D triangle */

int64_t __mli_point_triangle_intersection(struct mliVec p, struct mliTriangle t)
{
        int64_t sign12, sign23, sign31;
        struct mliVec vect12, vect23, vect31, vect1h, vect2h, vect3h;
        struct mliVec cross12_1p, cross23_2p, cross31_3p;

        /* First, a quick bounding-box test:                               */
        /* If P is outside triangle bbox, there cannot be an intersection. */

        if (p.x > MLI_MAX3(t.v1.x, t.v2.x, t.v3.x))
                return (MLI_OUTSIDE);
        if (p.y > MLI_MAX3(t.v1.y, t.v2.y, t.v3.y))
                return (MLI_OUTSIDE);
        if (p.z > MLI_MAX3(t.v1.z, t.v2.z, t.v3.z))
                return (MLI_OUTSIDE);
        if (p.x < MLI_MIN3(t.v1.x, t.v2.x, t.v3.x))
                return (MLI_OUTSIDE);
        if (p.y < MLI_MIN3(t.v1.y, t.v2.y, t.v3.y))
                return (MLI_OUTSIDE);
        if (p.z < MLI_MIN3(t.v1.z, t.v2.z, t.v3.z))
                return (MLI_OUTSIDE);

        /* For each triangle side, make a vector out of it by subtracting
         * vertexes; */
        /* make another vector from one vertex to point P. */
        /* The crossproduct of these two vectors is orthogonal to both and the
         */
        /* signs of its X,Y,Z components indicate whether P was to the inside or
         */
        /* to the outside of this triangle side. */

        vect12 = mliVec_substract(t.v1, t.v2);
        vect1h = mliVec_substract(t.v1, p);
        cross12_1p = mliVec_cross(vect12, vect1h);
        sign12 = MLI_SIGN3(cross12_1p);
        /* Extract X,Y,Z signs as 0..7 or 0...63 integer */

        vect23 = mliVec_substract(t.v2, t.v3);
        vect2h = mliVec_substract(t.v2, p);
        cross23_2p = mliVec_cross(vect23, vect2h);
        sign23 = MLI_SIGN3(cross23_2p);

        vect31 = mliVec_substract(t.v3, t.v1);
        vect3h = mliVec_substract(t.v3, p);
        cross31_3p = mliVec_cross(vect31, vect3h);
        sign31 = MLI_SIGN3(cross31_3p);

        /* If all three crossproduct vectors agree in their component signs,  */
        /* then the point must be inside all three.                           */
        /* P cannot be MLI_OUTSIDE all three sides simultaneously. */

        /* this is the old test; with the revised MLI_SIGN3() macro, the test
         * needs to be revised. */
        return ((sign12 & sign23 & sign31) == 0) ? MLI_OUTSIDE : MLI_INSIDE;
}

/*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

/**********************************************/
/* This is the main algorithm procedure.      */
/* Triangle t is compared with a unit cube,   */
/* centered on the origin.                    */
/* It returns MLI_INSIDE (0) or MLI_OUTSIDE(1) if t   */
/* intersects or does not intersect the cube. */
/**********************************************/

int64_t __mli_triangle_cube_intersection(struct mliTriangle t)
{
        int64_t v1_test, v2_test, v3_test;
        double d, denom;
        struct mliVec vect12, vect13, norm;
        struct mliVec hitpp, hitpn, hitnp, hitnn;

        /* First compare all three vertexes with all six face-planes */
        /* If any vertex is inside the cube, return immediately!     */

        if ((v1_test = __mli_face_plane(t.v1)) == MLI_INSIDE)
                return (MLI_INSIDE);
        if ((v2_test = __mli_face_plane(t.v2)) == MLI_INSIDE)
                return (MLI_INSIDE);
        if ((v3_test = __mli_face_plane(t.v3)) == MLI_INSIDE)
                return (MLI_INSIDE);

        /* If all three vertexes were outside of one or more face-planes, */
        /* return immediately with a trivial rejection!                   */

        if ((v1_test & v2_test & v3_test) != 0)
                return (MLI_OUTSIDE);

        /* Now do the same trivial rejection test for the 12 edge planes */

        v1_test |= __mli_bevel_2d(t.v1) << 8;
        v2_test |= __mli_bevel_2d(t.v2) << 8;
        v3_test |= __mli_bevel_2d(t.v3) << 8;
        if ((v1_test & v2_test & v3_test) != 0)
                return (MLI_OUTSIDE);

        /* Now do the same trivial rejection test for the 8 corner planes */

        v1_test |= __mli_bevel_3d(t.v1) << 24;
        v2_test |= __mli_bevel_3d(t.v2) << 24;
        v3_test |= __mli_bevel_3d(t.v3) << 24;
        if ((v1_test & v2_test & v3_test) != 0)
                return (MLI_OUTSIDE);

        /* If vertex 1 and 2, as a pair, cannot be trivially rejected */
        /* by the above tests, then see if the v1-->v2 triangle edge  */
        /* intersects the cube.  Do the same for v1-->v3 and v2-->v3. */
        /* Pass to the intersection algorithm the "OR" of the outcode */
        /* bits, so that only those cube faces which are spanned by   */
        /* each triangle edge need be tested.                         */

        if ((v1_test & v2_test) == 0)
                if (__mli_check_line(t.v1, t.v2, v1_test | v2_test) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        if ((v1_test & v3_test) == 0)
                if (__mli_check_line(t.v1, t.v3, v1_test | v3_test) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);
        if ((v2_test & v3_test) == 0)
                if (__mli_check_line(t.v2, t.v3, v2_test | v3_test) ==
                    MLI_INSIDE)
                        return (MLI_INSIDE);

        /* By now, we know that the triangle is not off to any side,     */
        /* and that its sides do not penetrate the cube.  We must now    */
        /* test for the cube intersecting the interior of the triangle.  */
        /* We do this by looking for intersections between the cube      */
        /* diagonals and the triangle...first finding the intersection   */
        /* of the four diagonals with the plane of the triangle, and     */
        /* then if that intersection is inside the cube, pursuing        */
        /* whether the intersection point is inside the triangle itself. */

        /* To find plane of the triangle, first perform crossproduct on  */
        /* two triangle side vectors to compute the normal vector.       */

        vect12 = mliVec_substract(t.v1, t.v2);
        vect13 = mliVec_substract(t.v1, t.v3);
        norm = mliVec_cross(vect12, vect13);

        /* The normal vector "norm" X,Y,Z components are the coefficients */
        /* of the triangles AX + BY + CZ + D = 0 plane equation.  If we   */
        /* solve the plane equation for X=Y=Z (a diagonal), we get        */
        /* -D/(A+B+C) as a metric of the distance from cube center to the */
        /* diagonal/plane intersection.  If this is between -0.5 and 0.5, */
        /* the intersection is inside the cube.  If so, we continue by    */
        /* doing a point/triangle intersection.                           */
        /* Do this for all four diagonals.                                */

        d = norm.x * t.v1.x + norm.y * t.v1.y + norm.z * t.v1.z;

        /* if one of the diagonals is parallel to the plane,
         * the other will intersect the plane */
        if (fabs(denom = (norm.x + norm.y + norm.z)) > MLI_TRI_EPS)
        /* skip parallel diagonals to the plane; division by 0 can occur */
        {
                hitpp.x = hitpp.y = hitpp.z = d / denom;
                if (fabs(hitpp.x) <= 0.5)
                        if (__mli_point_triangle_intersection(hitpp, t) ==
                            MLI_INSIDE)
                                return (MLI_INSIDE);
        }
        if (fabs(denom = (norm.x + norm.y - norm.z)) > MLI_TRI_EPS) {
                hitpn.z = -(hitpn.x = hitpn.y = d / denom);
                if (fabs(hitpn.x) <= 0.5)
                        if (__mli_point_triangle_intersection(hitpn, t) ==
                            MLI_INSIDE)
                                return (MLI_INSIDE);
        }
        if (fabs(denom = (norm.x - norm.y + norm.z)) > MLI_TRI_EPS) {
                hitnp.y = -(hitnp.x = hitnp.z = d / denom);
                if (fabs(hitnp.x) <= 0.5)
                        if (__mli_point_triangle_intersection(hitnp, t) ==
                            MLI_INSIDE)
                                return (MLI_INSIDE);
        }
        if (fabs(denom = (norm.x - norm.y - norm.z)) > MLI_TRI_EPS) {
                hitnn.y = hitnn.z = -(hitnn.x = d / denom);
                if (fabs(hitnn.x) <= 0.5)
                        if (__mli_point_triangle_intersection(hitnn, t) ==
                            MLI_INSIDE)
                                return (MLI_INSIDE);
        }

        /* No edge touched the cube; no cube diagonal touched the triangle. */
        /* We're done...there was no intersection.                          */

        return (MLI_OUTSIDE);
}

void __mliTriangle_transform_into_obb(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c,
        const struct mliOBB obb,
        struct mliVec *a_out,
        struct mliVec *b_out,
        struct mliVec *c_out)
{
        struct mliVec obb_center = mliOBB_center(obb);
        const double scale_x = obb.upper.x - obb.lower.x;
        const double scale_y = obb.upper.y - obb.lower.y;
        const double scale_z = obb.upper.z - obb.lower.z;
        /* translate */
        (*a_out) = mliVec_substract(a, obb_center);
        (*b_out) = mliVec_substract(b, obb_center);
        (*c_out) = mliVec_substract(c, obb_center);
        /* scale */
        a_out->x /= scale_x;
        b_out->x /= scale_x;
        c_out->x /= scale_x;
        a_out->y /= scale_y;
        b_out->y /= scale_y;
        c_out->y /= scale_y;
        a_out->z /= scale_z;
        b_out->z /= scale_z;
        c_out->z /= scale_z;
}

int mliTriangle_has_overlap_obb(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c,
        const struct mliOBB obb)
{
        struct mliTriangle tri;
        __mliTriangle_transform_into_obb(
                a, b, c, obb, &tri.v1, &tri.v2, &tri.v3);
        if (__mli_triangle_cube_intersection(tri) == MLI_INSIDE)
                return 1;
        else
                return 0;
}

struct mliOBB mliTriangle_obb(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c)
{
        struct mliOBB obb;
        obb.lower.x = MLI_MIN3(a.x, b.x, c.x);
        obb.lower.y = MLI_MIN3(a.y, b.y, c.y);
        obb.lower.z = MLI_MIN3(a.z, b.z, c.z);
        obb.upper.x = MLI_MAX3(a.x, b.x, c.x);
        obb.upper.y = MLI_MAX3(a.y, b.y, c.y);
        obb.upper.z = MLI_MAX3(a.z, b.z, c.z);
        return obb;
}

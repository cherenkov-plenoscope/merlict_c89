/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "AxisAlignedGrid.h"
#include "../math/math.h"
#include "../ray/ray_AABB.h"
#include <assert.h>
#include <math.h>

/* Inspired by:
 * A Fast Voxel Traversal Algorithm for Ray Tracing
 * John Amanatides and Andrew Woo
 * Dept. of Computer Science
 * University of Toronto
 * Toronto, Ontario, Canada M5S 1A4
 */

struct mliIdx3 mliIdx3_set(const int64_t x, const int64_t y, const int64_t z)
{
        struct mliIdx3 iii;
        iii.x = x;
        iii.y = y;
        iii.z = z;
        return iii;
}

struct mli_AxisAlignedGrid mli_AxisAlignedGrid_set(
        struct mli_AABB bounds,
        struct mliIdx3 num_bins)
{
        struct mli_AxisAlignedGrid grid;
        grid.bounds = bounds;
        grid.num_bins = num_bins;
        assert(mli_AABB_valid(grid.bounds));
        assert(grid.num_bins.x > 0);
        assert(grid.num_bins.y > 0);
        assert(grid.num_bins.z > 0);
        grid.bin_width.x = (grid.bounds.upper.x - grid.bounds.lower.x) /
                           ((double)grid.num_bins.x);
        grid.bin_width.y = (grid.bounds.upper.y - grid.bounds.lower.y) /
                           ((double)grid.num_bins.y);
        grid.bin_width.z = (grid.bounds.upper.z - grid.bounds.lower.z) /
                           ((double)grid.num_bins.z);
        return grid;
}

struct mliIdx3 mli_AxisAlignedGrid_get_voxel_idx(
        const struct mli_AxisAlignedGrid *grid,
        struct mli_Vec point)
{
        struct mli_Vec pg = mli_Vec_substract(point, grid->bounds.lower);
        struct mliIdx3 iii;
        pg.x /= grid->bin_width.x;
        pg.y /= grid->bin_width.y;
        pg.z /= grid->bin_width.z;
        iii.x = (int64_t)floor(pg.x);
        iii.y = (int64_t)floor(pg.y);
        iii.z = (int64_t)floor(pg.z);
        return iii;
}

int mli_AxisAlignedGrid_find_voxel_of_first_interaction(
        const struct mli_AxisAlignedGrid *grid,
        const struct mli_Ray *ray,
        struct mliIdx3 *bin)
{
        if (mli_AABB_is_point_inside(grid->bounds, ray->support)) {
                (*bin) = mli_AxisAlignedGrid_get_voxel_idx(grid, ray->support);
                return MLI_AXISALIGNEDGRID_RAY_STARTS_INSIDE_GRID;
        } else {
                double ray_parameter_near, ray_parameter_far;
                int has_intersection;
                mli_Ray_aabb_intersections(
                        (*ray),
                        grid->bounds,
                        &ray_parameter_near,
                        &ray_parameter_far);
                has_intersection =
                        mli_Ray_aabb_intersections_is_valid_given_near_and_far(
                                ray_parameter_near, ray_parameter_far);
                if (has_intersection) {
                        struct mli_Vec inner;
                        inner = mli_Ray_at(ray, ray_parameter_near);
                        (*bin) = mli_AxisAlignedGrid_get_voxel_idx(grid, inner);

                        if (bin->x >= grid->num_bins.x) {
                                bin->x -= 1;
                        }
                        if (bin->y >= grid->num_bins.y) {
                                bin->y -= 1;
                        }
                        if (bin->z >= grid->num_bins.z) {
                                bin->z -= 1;
                        }

                        return MLI_AXISALIGNEDGRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS;
                } else {
                        return MLI_AXISALIGNEDGRID_RAY_DOES_NOT_INTERSECT_GRID;
                }
        }
        return 0;
}

struct mli_Vec mli_AxisAlignedGridTraversal_first_plane(
        const struct mli_AxisAlignedGrid *grid,
        const struct mliIdx3 voxel,
        const struct mli_Vec ray_direction)
{
        struct mli_Vec voxel_lower = mli_Vec_init(
                grid->bounds.lower.x + (double)voxel.x * grid->bin_width.x,
                grid->bounds.lower.y + (double)voxel.y * grid->bin_width.y,
                grid->bounds.lower.z + (double)voxel.z * grid->bin_width.z);
        struct mli_Vec voxel_upper = mli_Vec_init(
                grid->bounds.lower.x +
                        (double)(voxel.x + 1) * grid->bin_width.x,
                grid->bounds.lower.y +
                        (double)(voxel.y + 1) * grid->bin_width.y,
                grid->bounds.lower.z +
                        (double)(voxel.z + 1) * grid->bin_width.z);

        struct mli_Vec first;

        if (ray_direction.x >= 0.0) {
                first.x = voxel_upper.x;
        } else {
                first.x = voxel_lower.x;
        }

        if (ray_direction.y >= 0.0) {
                first.y = voxel_upper.y;
        } else {
                first.y = voxel_lower.y;
        }

        if (ray_direction.z >= 0.0) {
                first.z = voxel_upper.z;
        } else {
                first.z = voxel_lower.z;
        }

        return first;
}

double calc_t_for_x_plane(const double x_plane, const struct mli_Ray *ray)
{
        return -(ray->support.x - x_plane) / (ray->direction.x);
}

double calc_t_for_y_plane(const double y_plane, const struct mli_Ray *ray)
{
        return -(ray->support.y - y_plane) / (ray->direction.y);
}

double calc_t_for_z_plane(const double z_plane, const struct mli_Ray *ray)
{
        return -(ray->support.z - z_plane) / (ray->direction.z);
}

struct mli_AxisAlignedGridTraversal mli_AxisAlignedGridTraversal_start(
        const struct mli_AxisAlignedGrid *grid,
        const struct mli_Ray *ray)
{
        struct mli_AxisAlignedGridTraversal traversal;

        traversal.grid = grid;
        traversal.valid = mli_AxisAlignedGrid_find_voxel_of_first_interaction(
                grid, ray, &traversal.voxel);
        if (traversal.valid) {
                struct mli_Vec first_plane;
                traversal.step.x = MLI_MATH_SIGN(ray->direction.x);
                traversal.step.y = MLI_MATH_SIGN(ray->direction.y);
                traversal.step.z = MLI_MATH_SIGN(ray->direction.z);

                first_plane = mli_AxisAlignedGridTraversal_first_plane(
                        grid, traversal.voxel, ray->direction);

                traversal.tMax.x = calc_t_for_x_plane(first_plane.x, ray);
                traversal.tMax.y = calc_t_for_y_plane(first_plane.y, ray);
                traversal.tMax.z = calc_t_for_z_plane(first_plane.z, ray);

                traversal.tDelta.x = grid->bin_width.x / fabs(ray->direction.x);
                traversal.tDelta.y = grid->bin_width.y / fabs(ray->direction.y);
                traversal.tDelta.z = grid->bin_width.z / fabs(ray->direction.z);
        }
        return traversal;
}

int mli_AxisAlignedGridTraversal_next(
        struct mli_AxisAlignedGridTraversal *traversal)
{
        struct mli_AxisAlignedGridTraversal *t = traversal;
        int RAY_LEFT_GRID = 0;

        if (t->tMax.x < t->tMax.y) {
                if (t->tMax.x < t->tMax.z) {
                        t->voxel.x += t->step.x;
                        if (t->voxel.x < 0 ||
                            t->voxel.x >= t->grid->num_bins.x) {
                                traversal->valid = RAY_LEFT_GRID;
                                return traversal->valid;
                        }
                        t->tMax.x += t->tDelta.x;
                } else {
                        t->voxel.z += t->step.z;
                        if (t->voxel.z < 0 ||
                            t->voxel.z >= t->grid->num_bins.z) {
                                traversal->valid = RAY_LEFT_GRID;
                                return traversal->valid;
                        }
                        t->tMax.z += t->tDelta.z;
                }
        } else {
                if (t->tMax.y < t->tMax.z) {
                        t->voxel.y += t->step.y;
                        if (t->voxel.y < 0 ||
                            t->voxel.y >= t->grid->num_bins.y) {
                                traversal->valid = RAY_LEFT_GRID;
                                return traversal->valid;
                        }
                        t->tMax.y += t->tDelta.y;
                } else {
                        t->voxel.z += t->step.z;
                        if (t->voxel.z < 0 ||
                            t->voxel.z >= t->grid->num_bins.z) {
                                traversal->valid = RAY_LEFT_GRID;
                                return traversal->valid;
                        }
                        t->tMax.z += t->tDelta.z;
                }
        }
        return traversal->valid;
}

void mli_AxisAlignedGridTraversal_fprint(
        FILE *f,
        struct mli_AxisAlignedGridTraversal *traversal)
{
        struct mli_AxisAlignedGridTraversal *t = traversal;
        fprintf(f,
                "  grid.bounds.upper: [%f, %f, %f]\n",
                t->grid->bounds.upper.x,
                t->grid->bounds.upper.y,
                t->grid->bounds.upper.z);
        fprintf(f,
                "  grid.bounds.lower: [%f, %f, %f]\n",
                t->grid->bounds.lower.x,
                t->grid->bounds.lower.y,
                t->grid->bounds.lower.z);
        fprintf(f,
                "  grid.num_bins: [%ld, %ld, %ld]\n",
                t->grid->num_bins.x,
                t->grid->num_bins.y,
                t->grid->num_bins.z);

        fprintf(f, "  valid: %d\n", t->valid);
        fprintf(f,
                "  voxel: [%ld, %ld, %ld]\n",
                t->voxel.x,
                t->voxel.y,
                t->voxel.z);
        fprintf(f, "  step: [%f, %f, %f]\n", t->step.x, t->step.y, t->step.z);
        fprintf(f, "  tMax: [%f, %f, %f]\n", t->tMax.x, t->tMax.y, t->tMax.z);
        fprintf(f,
                "  tDelta: [%f, %f, %f]\n",
                t->tDelta.x,
                t->tDelta.y,
                t->tDelta.z);
}

void mli_Ray_fprint(FILE *f, struct mli_Ray *ray)
{
        fprintf(f,
                "[%f, %f, %f] + lam*[%f, %f, %f]",
                ray->support.x,
                ray->support.y,
                ray->support.z,
                ray->direction.x,
                ray->direction.y,
                ray->direction.z);
}

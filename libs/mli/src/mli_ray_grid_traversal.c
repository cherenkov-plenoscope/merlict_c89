/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "mli_ray_grid_traversal.h"
#include "mli_math.h"
#include "mliRay_AABB.h"
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

struct mliAxisAlignedGrid mliAxisAlignedGrid_set(
        struct mliAABB bounds,
        struct mliIdx3 num_bins)
{
        struct mliAxisAlignedGrid grid;
        grid.bounds = bounds;
        grid.num_bins = num_bins;
        assert(mliAABB_valid(grid.bounds));
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

struct mliIdx3 mliAxisAlignedGrid_get_voxel_idx(
        const struct mliAxisAlignedGrid *grid,
        struct mliVec point)
{
        struct mliVec pg = mliVec_substract(point, grid->bounds.lower);
        struct mliIdx3 iii;
        pg.x /= grid->bin_width.x;
        pg.y /= grid->bin_width.y;
        pg.z /= grid->bin_width.z;
        iii.x = (int64_t)floor(pg.x);
        iii.y = (int64_t)floor(pg.y);
        iii.z = (int64_t)floor(pg.z);
        return iii;
}

int mliAxisAlignedGrid_find_voxel_of_first_interaction(
        const struct mliAxisAlignedGrid *grid,
        const struct mliRay *ray,
        struct mliIdx3 *bin)
{
        if (mliAABB_is_point_inside(grid->bounds, ray->support)) {
                (*bin) = mliAxisAlignedGrid_get_voxel_idx(grid, ray->support);
                return MLI_AXIS_ALIGNED_GRID_RAY_STARTS_INSIDE_GRID;
        } else {
                double ray_parameter;
                int has_intersection = mliRay_has_overlap_aabb(
                        (*ray), grid->bounds, &ray_parameter);
                if (has_intersection) {
                        struct mliVec inner = mliRay_at(ray, ray_parameter);
                        (*bin) = mliAxisAlignedGrid_get_voxel_idx(grid, inner);

                        if (bin->x >= grid->num_bins.x) {
                                bin->x -= 1;
                        }
                        if (bin->y >= grid->num_bins.y) {
                                bin->y -= 1;
                        }
                        if (bin->z >= grid->num_bins.z) {
                                bin->z -= 1;
                        }

                        return MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS;
                } else {
                        return MLI_AXIS_ALIGNED_GRID_RAY_DOES_NOT_INTERSECT_GRID;
                }
        }
        return 0;
}

struct mliVec mliAxisAlignedGridTraversal_first_plane(
        const struct mliAxisAlignedGrid *grid,
        const struct mliIdx3 voxel,
        const struct mliVec ray_direction)
{
        struct mliVec voxel_lower = mliVec_init(
                grid->bounds.lower.x + (double)voxel.x * grid->bin_width.x,
                grid->bounds.lower.y + (double)voxel.y * grid->bin_width.y,
                grid->bounds.lower.z + (double)voxel.z * grid->bin_width.z);
        struct mliVec voxel_upper = mliVec_init(
                grid->bounds.lower.x +
                        (double)(voxel.x + 1) * grid->bin_width.x,
                grid->bounds.lower.y +
                        (double)(voxel.y + 1) * grid->bin_width.y,
                grid->bounds.lower.z +
                        (double)(voxel.z + 1) * grid->bin_width.z);

        struct mliVec first;

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

double calc_t_for_x_plane(const double x_plane, const struct mliRay *ray)
{
        return -(ray->support.x - x_plane) / (ray->direction.x);
}

double calc_t_for_y_plane(const double y_plane, const struct mliRay *ray)
{
        return -(ray->support.y - y_plane) / (ray->direction.y);
}

double calc_t_for_z_plane(const double z_plane, const struct mliRay *ray)
{
        return -(ray->support.z - z_plane) / (ray->direction.z);
}

struct mliAxisAlignedGridTraversal mliAxisAlignedGridTraversal_start(
        const struct mliAxisAlignedGrid *grid,
        const struct mliRay *ray)
{
        struct mliAxisAlignedGridTraversal traversal;

        traversal.grid = grid;
        traversal.valid = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                grid, ray, &traversal.voxel);
        if (traversal.valid) {
                struct mliVec first_plane;
                traversal.step.x = MLI_SIGN(ray->direction.x);
                traversal.step.y = MLI_SIGN(ray->direction.y);
                traversal.step.z = MLI_SIGN(ray->direction.z);

                first_plane = mliAxisAlignedGridTraversal_first_plane(
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

int mliAxisAlignedGridTraversal_next(
        struct mliAxisAlignedGridTraversal *traversal)
{
        struct mliAxisAlignedGridTraversal *t = traversal;
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

void mliAxisAlignedGridTraversal_fprint(
        FILE *f,
        struct mliAxisAlignedGridTraversal *traversal)
{
        struct mliAxisAlignedGridTraversal *t = traversal;
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

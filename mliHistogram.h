/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIHISTOGRAM_H_INCLUDED__
#define MERLICT_MLIHISTOGRAM_H_INCLUDED__

#include <stdint.h>
#include <assert.h>
#include "mliMath.h"

/*
typedef struct {
    uint64_t num_bins;
    double *bin_edges;
    uint64_t underflow;
    uint64_t overflow;
    uint64_t *bins;
} mliHistogram;

void mliHistogram_malloc(mliHistogram* hist) {
    hist->bin_edges = (double)malloc((hist->num_bins + 1u)*sizeof(double));
    hist->bins = (uint64_t)malloc((hist->num_bins)*sizeof(uint64_t));}

void mliHistogram_free(mliHistogram* hist) {
    free(hist->bin_edges);
    free(hist->bins);
    hist->underflow = 0u;
    hist->overflow = 0u;
    hist->num_bins = 0u;}
*/

void mli_histogram(
    const double *bin_edges,
    const uint64_t num_bin_edges,
    uint64_t *underflow_bin,
    uint64_t *bins,
    uint64_t *overflow_bin,
    const double point) {
    uint64_t idx_upper = mli_upper_compare(bin_edges, num_bin_edges, point);
    if (idx_upper == 0) {
        (*underflow_bin) += 1u;
    } else if (idx_upper == num_bin_edges) {
        (*overflow_bin) += 1u;
    } else {
        bins[idx_upper - 1] += 1u;
    }
}

void mli_bin_edges_linspace(
    const double start,
    const double stop,
    double *bin_edges,
    const uint64_t num_bin_edges) {
    uint64_t i;
    const double range = stop - start;
    const double step = range/(double)(num_bin_edges - 1u);
    for (i = 0; i < num_bin_edges; i++) {
        bin_edges[i] = (double)i * step;
    }
}

#endif

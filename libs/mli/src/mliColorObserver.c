/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "mliColorObserver.h"
#include "chk.h"
#include "mliFunc.h"

struct mliColorObserver mliColorObserver_init(void)
{
        struct mliColorObserver colobs;
        colobs.r = mliFunc_init();
        colobs.g = mliFunc_init();
        colobs.b = mliFunc_init();
        return colobs;
}

void mliColorObserver_free(struct mliColorObserver *colobs)
{
        mliFunc_free(&colobs->r);
        mliFunc_free(&colobs->g);
        mliFunc_free(&colobs->b);
}

int mliColorObserver_malloc_cie1931(struct mliColorObserver *colobs)
{
        float red[32][2] = {
                {3.59e-7, 0.00313729}, {3.95e-7, 0.0157643},
                {4.07e-7, 0.0399472},  {4.17e-7, 0.111873},
                {4.25e-7, 0.225938},   {4.34e-7, 0.325427},
                {4.45e-7, 0.350076},   {4.56e-7, 0.312901},
                {4.66e-7, 0.232106},   {4.77e-7, 0.123945},
                {4.88e-7, 0.0476752},  {4.99e-7, 0.0204449},
                {5.10e-7, 0.0257893},  {5.21e-7, 0.0879818},
                {5.31e-7, 0.194860},   {5.47e-7, 0.408545},
                {5.60e-7, 0.632843},   {5.74e-7, 0.863571},
                {5.82e-7, 0.973668},   {5.92e-7, 1.05807},
                {6.03e-7, 1.05453},    {6.13e-7, 0.960971},
                {6.20e-7, 0.841939},   {6.30e-7, 0.620949},
                {6.42e-7, 0.406344},   {6.49e-7, 0.297104},
                {6.59e-7, 0.184051},   {6.70e-7, 0.101417},
                {6.81e-7, 0.0601510},  {6.92e-7, 0.0379925},
                {7.14e-7, 0.0290624},  {7.49e-7, 0.022833},
        };

        float green[24][2] = {{3.60e-7, 0.00619806}, {4.16e-7, 0.0140582},
                              {4.28e-7, 0.0181371},  {4.50e-7, 0.0443920},
                              {4.72e-7, 0.107352},   {4.83e-7, 0.169252},
                              {4.93e-7, 0.252946},   {5.01e-7, 0.359788},
                              {5.12e-7, 0.587986},   {5.18e-7, 0.702608},
                              {5.25e-7, 0.817763},   {5.34e-7, 0.928310},
                              {5.45e-7, 0.992962},   {5.56e-7, 1.00761},
                              {5.67e-7, 0.967707},   {5.78e-7, 0.879856},
                              {5.97e-7, 0.666386},   {6.15e-7, 0.445792},
                              {6.34e-7, 0.233702},   {6.45e-7, 0.148795},
                              {6.56e-7, 0.0902547},  {6.67e-7, 0.0553526},
                              {6.86e-7, 0.0286734},  {7.49e-7, 0.0198185}};

        float blue[38][2] = {{3.60e-7, 0.00316790}, {3.87e-7, 0.0245620},
                             {3.93e-7, 0.0321075},  {3.98e-7, 0.0644949},
                             {4.02e-7, 0.0901703},  {4.06e-7, 0.152735},
                             {4.15e-7, 0.398040},   {4.18e-7, 0.621742},
                             {4.22e-7, 0.931972},   {4.26e-7, 1.22845},
                             {4.30e-7, 1.48744},    {4.31e-7, 1.54615},
                             {4.33e-7, 1.61009},    {4.34e-7, 1.67360},
                             {4.37e-7, 1.73750},    {4.40e-7, 1.77390},
                             {4.46e-7, 1.77763},    {4.52e-7, 1.75330},
                             {4.55e-7, 1.70983},    {4.59e-7, 1.65363},
                             {4.61e-7, 1.59153},    {4.63e-7, 1.52051},
                             {4.64e-7, 1.45918},    {4.69e-7, 1.22189},
                             {4.75e-7, 0.969627},   {4.81e-7, 0.716157},
                             {4.89e-7, 0.470289},   {4.94e-7, 0.361365},
                             {4.98e-7, 0.298012},   {5.03e-7, 0.235268},
                             {5.09e-7, 0.177356},   {5.14e-7, 0.127632},
                             {5.19e-7, 0.0869871},  {5.26e-7, 0.0609592},
                             {5.37e-7, 0.0351387},  {5.48e-7, 0.0236983},
                             {5.81e-7, 0.0223867},  {7.49e-7, 0.0228487}};
        uint64_t i;

        mliColorObserver_free(colobs);

        chk_msg(mliFunc_malloc(&colobs->r, sizeof(red) / sizeof(red[0])),
                "Can't malloc red channel in ColorObserver.");
        for (i = 0; i < colobs->r.num_points; i++) {
                colobs->r.x[i] = red[i][0];
                colobs->r.y[i] = red[i][1];
        }

        chk_msg(mliFunc_malloc(&colobs->g, sizeof(green) / sizeof(green[0])),
                "Can't malloc green channel in ColorObserver.");
        for (i = 0; i < colobs->g.num_points; i++) {
                colobs->g.x[i] = green[i][0];
                colobs->g.y[i] = green[i][1];
        }

        chk_msg(mliFunc_malloc(&colobs->b, sizeof(blue) / sizeof(blue[0])),
                "Can't malloc blue channel in ColorObserver.");
        for (i = 0; i < colobs->b.num_points; i++) {
                colobs->b.x[i] = blue[i][0];
                colobs->b.y[i] = blue[i][1];
        }

chk_error:
        mliColorObserver_free(colobs);
        return 0;
}

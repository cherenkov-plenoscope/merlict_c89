/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIRGBEFFICIENCY_H_
#define MERLICT_MLIRGBEFFICIENCY_H_

#include <stdint.h>
#include "mliFunc.h"
#include "mliColor.h"
#include "mliImage.h"

struct mliRgbEfficiency {
        struct mliFunc r;
        struct mliFunc g;
        struct mliFunc b;
};

struct mliRgbEfficiency mliRgbEfficiency_init(void)
{
        struct mliRgbEfficiency imss;
        imss.r = mliFunc_init();
        imss.g = mliFunc_init();
        imss.b = mliFunc_init();
        return imss;
}

void mliRgbEfficiency_free(struct mliRgbEfficiency *imss)
{
        mliFunc_free(&imss->r);
        mliFunc_free(&imss->g);
        mliFunc_free(&imss->b);
        *imss = mliRgbEfficiency_init();
}

int mliRgbEfficiency_malloc_default_cmos(struct mliRgbEfficiency *imss)
{
        mliRgbEfficiency_free(imss);
        mli_c(mliFunc_malloc(&imss->r, 7));
        mli_c(mliFunc_malloc(&imss->g, 7));
        mli_c(mliFunc_malloc(&imss->b, 7));

        /* blue */
        imss->b.x[0] = 200e-9;
        imss->b.y[0] = 0.0;
        imss->b.x[1] = 300e-9;
        imss->b.y[1] = 0.0;
        imss->b.x[2] = 425e-9;
        imss->b.y[2] = 0.9;
        imss->b.x[3] = 450e-9;
        imss->b.y[3] = 1.0;
        imss->b.x[4] = 500e-9;
        imss->b.y[4] = 0.9;
        imss->b.x[5] = 550e-9;
        imss->b.y[5] = 0.0;
        imss->b.x[6] = 1200e-9;
        imss->b.y[6] = 0.0;

        /* green */
        imss->g.x[0] = 200e-9;
        imss->g.y[0] = 0.0;
        imss->g.x[1] = 450e-9;
        imss->g.y[1] = 0.0;
        imss->g.x[2] = 500e-9;
        imss->g.y[2] = 0.8;
        imss->g.x[3] = 525e-9;
        imss->g.y[3] = 0.9;
        imss->g.x[4] = 550e-9;
        imss->g.y[4] = 0.8;
        imss->g.x[5] = 625e-9;
        imss->g.y[5] = 0.0;
        imss->g.x[6] = 1200e-9;
        imss->g.y[6] = 0.0;

        /* red */
        imss->r.x[0] = 200e-9;
        imss->r.y[0] = 0.0;
        imss->r.x[1] = 550e-9;
        imss->r.y[1] = 0.0;
        imss->r.x[2] = 580e-9;
        imss->r.y[2] = 0.7;
        imss->r.x[3] = 600e-9;
        imss->r.y[3] = 0.8;
        imss->r.x[4] = 680e-9;
        imss->r.y[4] = 0.7;
        imss->r.x[5] = 800e-9;
        imss->r.y[5] = 0.0;
        imss->r.x[6] = 1200e-9;
        imss->r.y[6] = 0.0;
        return 1;
error:
        return 0;
}

struct mliLightSource {
        uint64_t id;
        struct mliVec position;
        struct mliFunc emission_spectrum;
};

struct mliLightSource mliLightSource_init(void)
{
        struct mliLightSource s;
        s.id = 0;
        s.position = mliVec_set(0, 0, 0);
        s.emission_spectrum = mliFunc_init();
        return s;
}

void mliLightSource_free(struct mliLightSource *s)
{
        mliFunc_free(&s->emission_spectrum);
        *s = mliLightSource_init();
}

int mliLightSource_malloc(struct mliLightSource *s, const uint64_t num_points)
{
        uint64_t i;
        mliLightSource_free(s);
        mli_check(
                mliFunc_malloc(&s->emission_spectrum, num_points),
                "Failed to malloc emission_spectrum in light-source.");
        for (i = 0; i < s->emission_spectrum.num_points; i++) {
                s->emission_spectrum.x[i] = 0.0;
                s->emission_spectrum.y[i] = 0.0;
        }
        return 1;
error:
        return 0;
}

int mliLightSource_malloc_sunlight_spectrum(struct mliLightSource *s)
{
        struct mliFunc *f = &s->emission_spectrum;
        mli_check(
                mliLightSource_malloc(s, 292),
                "Failed to malloc spectrum for sunlight.");
        f->x[0] = 2.00e-07;
        f->y[0] = 0.00;
        f->x[1] = 3.02e-07;
        f->y[1] = 4.32;
        f->x[2] = 3.07e-07;
        f->y[2] = 14.74;
        f->x[3] = 3.10e-07;
        f->y[3] = 24.73;
        f->x[4] = 3.13e-07;
        f->y[4] = 34.12;
        f->x[5] = 3.16e-07;
        f->y[5] = 44.81;
        f->x[6] = 3.19e-07;
        f->y[6] = 57.99;
        f->x[7] = 3.24e-07;
        f->y[7] = 70.53;
        f->x[8] = 3.30e-07;
        f->y[8] = 79.68;
        f->x[9] = 3.37e-07;
        f->y[9] = 91.03;
        f->x[10] = 3.46e-07;
        f->y[10] = 106.20;
        f->x[11] = 3.53e-07;
        f->y[11] = 117.79;
        f->x[12] = 3.55e-07;
        f->y[12] = 128.49;
        f->x[13] = 3.58e-07;
        f->y[13] = 142.08;
        f->x[14] = 3.61e-07;
        f->y[14] = 152.39;
        f->x[15] = 3.68e-07;
        f->y[15] = 152.62;
        f->x[16] = 3.77e-07;
        f->y[16] = 153.97;
        f->x[17] = 3.80e-07;
        f->y[17] = 168.72;
        f->x[18] = 3.82e-07;
        f->y[18] = 181.46;
        f->x[19] = 3.86e-07;
        f->y[19] = 172.46;
        f->x[20] = 3.90e-07;
        f->y[20] = 200.87;
        f->x[21] = 3.92e-07;
        f->y[21] = 277.52;
        f->x[22] = 3.94e-07;
        f->y[22] = 227.20;
        f->x[23] = 3.96e-07;
        f->y[23] = 329.55;
        f->x[24] = 4.00e-07;
        f->y[24] = 341.52;
        f->x[25] = 4.06e-07;
        f->y[25] = 360.75;
        f->x[26] = 4.07e-07;
        f->y[26] = 373.63;
        f->x[27] = 4.13e-07;
        f->y[27] = 386.33;
        f->x[28] = 4.15e-07;
        f->y[28] = 357.07;
        f->x[29] = 4.20e-07;
        f->y[29] = 352.79;
        f->x[30] = 4.25e-07;
        f->y[30] = 371.89;
        f->x[31] = 4.26e-07;
        f->y[31] = 404.53;
        f->x[32] = 4.28e-07;
        f->y[32] = 430.67;
        f->x[33] = 4.31e-07;
        f->y[33] = 450.36;
        f->x[34] = 4.34e-07;
        f->y[34] = 472.10;
        f->x[35] = 4.37e-07;
        f->y[35] = 489.88;
        f->x[36] = 4.39e-07;
        f->y[36] = 509.12;
        f->x[37] = 4.42e-07;
        f->y[37] = 532.05;
        f->x[38] = 4.45e-07;
        f->y[38] = 551.25;
        f->x[39] = 4.48e-07;
        f->y[39] = 567.49;
        f->x[40] = 4.58e-07;
        f->y[40] = 593.35;
        f->x[41] = 4.69e-07;
        f->y[41] = 619.56;
        f->x[42] = 4.75e-07;
        f->y[42] = 606.05;
        f->x[43] = 4.79e-07;
        f->y[43] = 598.87;
        f->x[44] = 4.82e-07;
        f->y[44] = 623.32;
        f->x[45] = 4.91e-07;
        f->y[45] = 646.28;
        f->x[46] = 5.01e-07;
        f->y[46] = 666.06;
        f->x[47] = 5.09e-07;
        f->y[47] = 638.92;
        f->x[48] = 5.12e-07;
        f->y[48] = 660.42;
        f->x[49] = 5.20e-07;
        f->y[49] = 690.48;
        f->x[50] = 5.28e-07;
        f->y[50] = 705.54;
        f->x[51] = 5.34e-07;
        f->y[51] = 708.19;
        f->x[52] = 5.44e-07;
        f->y[52] = 736.93;
        f->x[53] = 5.47e-07;
        f->y[53] = 725.91;
        f->x[54] = 5.59e-07;
        f->y[54] = 737.38;
        f->x[55] = 5.72e-07;
        f->y[55] = 750.54;
        f->x[56] = 5.78e-07;
        f->y[56] = 739.39;
        f->x[57] = 5.81e-07;
        f->y[57] = 729.78;
        f->x[58] = 5.84e-07;
        f->y[58] = 745.39;
        f->x[59] = 5.93e-07;
        f->y[59] = 771.86;
        f->x[60] = 6.04e-07;
        f->y[60] = 781.68;
        f->x[61] = 6.12e-07;
        f->y[61] = 785.25;
        f->x[62] = 6.14e-07;
        f->y[62] = 774.31;
        f->x[63] = 6.20e-07;
        f->y[63] = 775.73;
        f->x[64] = 6.25e-07;
        f->y[64] = 790.69;
        f->x[65] = 6.31e-07;
        f->y[65] = 813.65;
        f->x[66] = 6.39e-07;
        f->y[66] = 793.81;
        f->x[67] = 6.42e-07;
        f->y[67] = 778.36;
        f->x[68] = 6.47e-07;
        f->y[68] = 768.48;
        f->x[69] = 6.54e-07;
        f->y[69] = 800.37;
        f->x[70] = 6.58e-07;
        f->y[70] = 829.64;
        f->x[71] = 6.63e-07;
        f->y[71] = 833.02;
        f->x[72] = 6.71e-07;
        f->y[72] = 814.83;
        f->x[73] = 6.72e-07;
        f->y[73] = 785.30;
        f->x[74] = 6.75e-07;
        f->y[74] = 746.16;
        f->x[75] = 6.79e-07;
        f->y[75] = 703.63;
        f->x[76] = 6.86e-07;
        f->y[76] = 765.23;
        f->x[77] = 6.90e-07;
        f->y[77] = 803.88;
        f->x[78] = 7.00e-07;
        f->y[78] = 812.80;
        f->x[79] = 7.03e-07;
        f->y[79] = 740.88;
        f->x[80] = 7.03e-07;
        f->y[80] = 791.51;
        f->x[81] = 7.07e-07;
        f->y[81] = 696.11;
        f->x[82] = 7.09e-07;
        f->y[82] = 671.49;
        f->x[83] = 7.18e-07;
        f->y[83] = 685.00;
        f->x[84] = 7.22e-07;
        f->y[84] = 713.52;
        f->x[85] = 7.25e-07;
        f->y[85] = 753.22;
        f->x[86] = 7.32e-07;
        f->y[86] = 791.69;
        f->x[87] = 7.36e-07;
        f->y[87] = 811.58;
        f->x[88] = 7.40e-07;
        f->y[88] = 822.46;
        f->x[89] = 7.44e-07;
        f->y[89] = 771.30;
        f->x[90] = 7.45e-07;
        f->y[90] = 680.30;
        f->x[91] = 7.45e-07;
        f->y[91] = 699.87;
        f->x[92] = 7.45e-07;
        f->y[92] = 719.44;
        f->x[93] = 7.46e-07;
        f->y[93] = 557.80;
        f->x[94] = 7.48e-07;
        f->y[94] = 463.41;
        f->x[95] = 7.49e-07;
        f->y[95] = 371.35;
        f->x[96] = 7.53e-07;
        f->y[96] = 303.13;
        f->x[97] = 7.56e-07;
        f->y[97] = 425.51;
        f->x[98] = 7.58e-07;
        f->y[98] = 513.16;
        f->x[99] = 7.58e-07;
        f->y[99] = 533.06;
        f->x[100] = 7.61e-07;
        f->y[100] = 665.91;
        f->x[101] = 7.61e-07;
        f->y[101] = 691.35;
        f->x[102] = 7.64e-07;
        f->y[102] = 807.17;
        f->x[103] = 7.68e-07;
        f->y[103] = 764.29;
        f->x[104] = 7.70e-07;
        f->y[104] = 806.86;
        f->x[105] = 7.83e-07;
        f->y[105] = 778.43;
        f->x[106] = 7.93e-07;
        f->y[106] = 774.34;
        f->x[107] = 7.98e-07;
        f->y[107] = 753.31;
        f->x[108] = 8.01e-07;
        f->y[108] = 708.55;
        f->x[109] = 8.02e-07;
        f->y[109] = 660.03;
        f->x[110] = 8.10e-07;
        f->y[110] = 643.52;
        f->x[111] = 8.20e-07;
        f->y[111] = 681.70;
        f->x[112] = 8.25e-07;
        f->y[112] = 714.81;
        f->x[113] = 8.29e-07;
        f->y[113] = 743.93;
        f->x[114] = 8.33e-07;
        f->y[114] = 757.72;
        f->x[115] = 8.39e-07;
        f->y[115] = 731.59;
        f->x[116] = 8.41e-07;
        f->y[116] = 709.70;
        f->x[117] = 8.47e-07;
        f->y[117] = 724.37;
        f->x[118] = 8.52e-07;
        f->y[118] = 740.58;
        f->x[119] = 8.58e-07;
        f->y[119] = 732.19;
        f->x[120] = 8.67e-07;
        f->y[120] = 745.73;
        f->x[121] = 8.74e-07;
        f->y[121] = 735.76;
        f->x[122] = 8.79e-07;
        f->y[122] = 713.75;
        f->x[123] = 8.81e-07;
        f->y[123] = 675.05;
        f->x[124] = 8.82e-07;
        f->y[124] = 633.00;
        f->x[125] = 8.84e-07;
        f->y[125] = 589.22;
        f->x[126] = 8.86e-07;
        f->y[126] = 538.67;
        f->x[127] = 8.90e-07;
        f->y[127] = 532.67;
        f->x[128] = 8.93e-07;
        f->y[128] = 546.36;
        f->x[129] = 9.01e-07;
        f->y[129] = 531.10;
        f->x[130] = 9.09e-07;
        f->y[130] = 541.24;
        f->x[131] = 9.11e-07;
        f->y[131] = 577.89;
        f->x[132] = 9.14e-07;
        f->y[132] = 504.93;
        f->x[133] = 9.16e-07;
        f->y[133] = 426.52;
        f->x[134] = 9.16e-07;
        f->y[134] = 449.46;
        f->x[135] = 9.17e-07;
        f->y[135] = 345.14;
        f->x[136] = 9.17e-07;
        f->y[136] = 369.21;
        f->x[137] = 9.19e-07;
        f->y[137] = 276.66;
        f->x[138] = 9.20e-07;
        f->y[138] = 201.56;
        f->x[139] = 9.25e-07;
        f->y[139] = 165.91;
        f->x[140] = 9.32e-07;
        f->y[140] = 244.62;
        f->x[141] = 9.38e-07;
        f->y[141] = 257.15;
        f->x[142] = 9.46e-07;
        f->y[142] = 285.91;
        f->x[143] = 9.50e-07;
        f->y[143] = 325.19;
        f->x[144] = 9.52e-07;
        f->y[144] = 356.04;
        f->x[145] = 9.54e-07;
        f->y[145] = 380.47;
        f->x[146] = 9.55e-07;
        f->y[146] = 407.26;
        f->x[147] = 9.57e-07;
        f->y[147] = 436.42;
        f->x[148] = 9.58e-07;
        f->y[148] = 469.09;
        f->x[149] = 9.60e-07;
        f->y[149] = 500.72;
        f->x[150] = 9.65e-07;
        f->y[150] = 535.90;
        f->x[151] = 9.71e-07;
        f->y[151] = 567.23;
        f->x[152] = 9.74e-07;
        f->y[152] = 600.88;
        f->x[153] = 9.78e-07;
        f->y[153] = 642.92;
        f->x[154] = 9.83e-07;
        f->y[154] = 668.10;
        f->x[155] = 9.91e-07;
        f->y[155] = 660.94;
        f->x[156] = 1.00e-06;
        f->y[156] = 651.71;
        f->x[157] = 1.02e-06;
        f->y[157] = 639.05;
        f->x[158] = 1.03e-06;
        f->y[158] = 623.45;
        f->x[159] = 1.05e-06;
        f->y[159] = 605.41;
        f->x[160] = 1.06e-06;
        f->y[160] = 586.71;
        f->x[161] = 1.08e-06;
        f->y[161] = 565.21;
        f->x[162] = 1.08e-06;
        f->y[162] = 537.23;
        f->x[163] = 1.09e-06;
        f->y[163] = 506.17;
        f->x[164] = 1.09e-06;
        f->y[164] = 473.23;
        f->x[165] = 1.09e-06;
        f->y[165] = 440.63;
        f->x[166] = 1.09e-06;
        f->y[166] = 400.01;
        f->x[167] = 1.10e-06;
        f->y[167] = 364.17;
        f->x[168] = 1.10e-06;
        f->y[168] = 334.52;
        f->x[169] = 1.10e-06;
        f->y[169] = 299.54;
        f->x[170] = 1.10e-06;
        f->y[170] = 259.22;
        f->x[171] = 1.10e-06;
        f->y[171] = 221.42;
        f->x[172] = 1.10e-06;
        f->y[172] = 186.16;
        f->x[173] = 1.11e-06;
        f->y[173] = 141.00;
        f->x[174] = 1.11e-06;
        f->y[174] = 97.93;
        f->x[175] = 1.12e-06;
        f->y[175] = 88.16;
        f->x[176] = 1.12e-06;
        f->y[176] = 116.38;
        f->x[177] = 1.13e-06;
        f->y[177] = 134.28;
        f->x[178] = 1.13e-06;
        f->y[178] = 173.11;
        f->x[179] = 1.14e-06;
        f->y[179] = 149.54;
        f->x[180] = 1.14e-06;
        f->y[180] = 209.67;
        f->x[181] = 1.15e-06;
        f->y[181] = 254.62;
        f->x[182] = 1.15e-06;
        f->y[182] = 303.22;
        f->x[183] = 1.15e-06;
        f->y[183] = 350.70;
        f->x[184] = 1.15e-06;
        f->y[184] = 392.91;
        f->x[185] = 1.16e-06;
        f->y[185] = 452.44;
        f->x[186] = 1.17e-06;
        f->y[186] = 442.30;
        f->x[187] = 1.18e-06;
        f->y[187] = 465.02;
        f->x[188] = 1.19e-06;
        f->y[188] = 456.88;
        f->x[189] = 1.20e-06;
        f->y[189] = 472.05;
        f->x[190] = 1.21e-06;
        f->y[190] = 500.50;
        f->x[191] = 1.23e-06;
        f->y[191] = 514.80;
        f->x[192] = 1.24e-06;
        f->y[192] = 505.18;
        f->x[193] = 1.25e-06;
        f->y[193] = 469.50;
        f->x[194] = 1.25e-06;
        f->y[194] = 433.45;
        f->x[195] = 1.26e-06;
        f->y[195] = 413.07;
        f->x[196] = 1.27e-06;
        f->y[196] = 469.55;
        f->x[197] = 1.28e-06;
        f->y[197] = 471.03;
        f->x[198] = 1.29e-06;
        f->y[198] = 440.54;
        f->x[199] = 1.29e-06;
        f->y[199] = 409.78;
        f->x[200] = 1.30e-06;
        f->y[200] = 379.11;
        f->x[201] = 1.30e-06;
        f->y[201] = 346.20;
        f->x[202] = 1.31e-06;
        f->y[202] = 314.55;
        f->x[203] = 1.31e-06;
        f->y[203] = 267.40;
        f->x[204] = 1.32e-06;
        f->y[204] = 216.79;
        f->x[205] = 1.32e-06;
        f->y[205] = 190.13;
        f->x[206] = 1.33e-06;
        f->y[206] = 155.30;
        f->x[207] = 1.33e-06;
        f->y[207] = 110.43;
        f->x[208] = 1.33e-06;
        f->y[208] = 66.92;
        f->x[209] = 1.33e-06;
        f->y[209] = 0.00;
        f->x[210] = 1.35e-06;
        f->y[210] = 0.00;
        f->x[211] = 1.36e-06;
        f->y[211] = 0.00;
        f->x[212] = 1.38e-06;
        f->y[212] = 0.00;
        f->x[213] = 1.39e-06;
        f->y[213] = 0.00;
        f->x[214] = 1.41e-06;
        f->y[214] = 0.00;
        f->x[215] = 1.42e-06;
        f->y[215] = 9.77;
        f->x[216] = 1.44e-06;
        f->y[216] = 50.60;
        f->x[217] = 1.45e-06;
        f->y[217] = 113.52;
        f->x[218] = 1.45e-06;
        f->y[218] = 82.46;
        f->x[219] = 1.45e-06;
        f->y[219] = 55.15;
        f->x[220] = 1.46e-06;
        f->y[220] = 65.87;
        f->x[221] = 1.47e-06;
        f->y[221] = 104.68;
        f->x[222] = 1.48e-06;
        f->y[222] = 140.77;
        f->x[223] = 1.48e-06;
        f->y[223] = 191.50;
        f->x[224] = 1.49e-06;
        f->y[224] = 244.79;
        f->x[225] = 1.49e-06;
        f->y[225] = 278.41;
        f->x[226] = 1.50e-06;
        f->y[226] = 323.91;
        f->x[227] = 1.52e-06;
        f->y[227] = 355.80;
        f->x[228] = 1.53e-06;
        f->y[228] = 373.53;
        f->x[229] = 1.55e-06;
        f->y[229] = 362.12;
        f->x[230] = 1.56e-06;
        f->y[230] = 335.79;
        f->x[231] = 1.58e-06;
        f->y[231] = 343.62;
        f->x[232] = 1.60e-06;
        f->y[232] = 323.24;
        f->x[233] = 1.61e-06;
        f->y[233] = 333.82;
        f->x[234] = 1.62e-06;
        f->y[234] = 324.97;
        f->x[235] = 1.65e-06;
        f->y[235] = 313.07;
        f->x[236] = 1.67e-06;
        f->y[236] = 301.44;
        f->x[237] = 1.69e-06;
        f->y[237] = 284.80;
        f->x[238] = 1.72e-06;
        f->y[238] = 255.25;
        f->x[239] = 1.73e-06;
        f->y[239] = 235.35;
        f->x[240] = 1.75e-06;
        f->y[240] = 206.40;
        f->x[241] = 1.76e-06;
        f->y[241] = 164.68;
        f->x[242] = 1.77e-06;
        f->y[242] = 118.72;
        f->x[243] = 1.78e-06;
        f->y[243] = 78.42;
        f->x[244] = 1.78e-06;
        f->y[244] = 45.90;
        f->x[245] = 1.80e-06;
        f->y[245] = 0.00;
        f->x[246] = 1.82e-06;
        f->y[246] = 0.00;
        f->x[247] = 1.83e-06;
        f->y[247] = 0.00;
        f->x[248] = 1.85e-06;
        f->y[248] = 0.00;
        f->x[249] = 1.86e-06;
        f->y[249] = 0.00;
        f->x[250] = 1.88e-06;
        f->y[250] = 0.00;
        f->x[251] = 1.90e-06;
        f->y[251] = 0.00;
        f->x[252] = 1.91e-06;
        f->y[252] = 0.00;
        f->x[253] = 1.93e-06;
        f->y[253] = 0.00;
        f->x[254] = 1.94e-06;
        f->y[254] = 0.00;
        f->x[255] = 1.96e-06;
        f->y[255] = 18.07;
        f->x[256] = 1.96e-06;
        f->y[256] = 70.77;
        f->x[257] = 1.98e-06;
        f->y[257] = 102.43;
        f->x[258] = 1.98e-06;
        f->y[258] = 45.83;
        f->x[259] = 2.00e-06;
        f->y[259] = 5.91;
        f->x[260] = 2.01e-06;
        f->y[260] = 24.65;
        f->x[261] = 2.01e-06;
        f->y[261] = 81.82;
        f->x[262] = 2.02e-06;
        f->y[262] = 126.87;
        f->x[263] = 2.03e-06;
        f->y[263] = 119.82;
        f->x[264] = 2.04e-06;
        f->y[264] = 93.14;
        f->x[265] = 2.06e-06;
        f->y[265] = 95.95;
        f->x[266] = 2.08e-06;
        f->y[266] = 129.35;
        f->x[267] = 2.10e-06;
        f->y[267] = 141.37;
        f->x[268] = 2.12e-06;
        f->y[268] = 134.21;
        f->x[269] = 2.13e-06;
        f->y[269] = 136.23;
        f->x[270] = 2.14e-06;
        f->y[270] = 128.31;
        f->x[271] = 2.16e-06;
        f->y[271] = 124.07;
        f->x[272] = 2.18e-06;
        f->y[272] = 119.75;
        f->x[273] = 2.20e-06;
        f->y[273] = 115.65;
        f->x[274] = 2.22e-06;
        f->y[274] = 111.38;
        f->x[275] = 2.24e-06;
        f->y[275] = 106.08;
        f->x[276] = 2.25e-06;
        f->y[276] = 99.96;
        f->x[277] = 2.27e-06;
        f->y[277] = 93.68;
        f->x[278] = 2.29e-06;
        f->y[278] = 85.03;
        f->x[279] = 2.30e-06;
        f->y[279] = 76.94;
        f->x[280] = 2.32e-06;
        f->y[280] = 67.28;
        f->x[281] = 2.34e-06;
        f->y[281] = 56.19;
        f->x[282] = 2.35e-06;
        f->y[282] = 35.69;
        f->x[283] = 2.37e-06;
        f->y[283] = 35.26;
        f->x[284] = 2.39e-06;
        f->y[284] = 35.46;
        f->x[285] = 2.40e-06;
        f->y[285] = 13.24;
        f->x[286] = 2.42e-06;
        f->y[286] = 13.28;
        f->x[287] = 2.43e-06;
        f->y[287] = 0.00;
        f->x[288] = 2.45e-06;
        f->y[288] = 5.72;
        f->x[289] = 2.46e-06;
        f->y[289] = 0.00;
        f->x[290] = 2.52e-06;
        f->y[290] = 0.00;
        f->x[291] = 2.59e-06;
        f->y[291] = 0.00;
        return 1;
error:
        return 0;
}

struct mliColorMap {
        struct mliRgbEfficiency sensor;

        uint64_t num_sources;
        uint64_t num_functions;

        struct mliLightSource *sources;
        struct mliImage mat;
};

struct mliColorMap mliColorMap_init(void)
{
        struct mliColorMap cmap;
        cmap.sensor = mliRgbEfficiency_init();
        cmap.num_sources = 0u;
        cmap.sources = NULL;
        cmap.num_functions = 0u;
        cmap.mat = mliImage_init();
        return cmap;
}

void mliColorMap_free(struct mliColorMap *cmap)
{
        uint64_t i;
        for (i = 0; i < cmap->num_sources; i++) {
                mliLightSource_free(&cmap->sources[i]);
        }
        free(cmap->sources);
        mliImage_free(&cmap->mat);
        *cmap = mliColorMap_init();
}

int mliColorMap_malloc(
        struct mliColorMap *cmap,
        const uint64_t num_sources,
        const uint64_t num_functions)
{
        uint64_t s, f;
        mliColorMap_free(cmap);
        cmap->num_sources = num_sources;
        cmap->num_functions = num_functions;
        mli_malloc(cmap->sources, struct mliLightSource, num_sources);
        for (s = 0; s < cmap->num_sources; s++) {
                cmap->sources[s] = mliLightSource_init();
        }
        mli_check(
                mliImage_malloc(&cmap->mat, num_sources, num_functions),
                "Failed to malloc color_function_source_matrix");
        for (s = 0; s < cmap->num_sources; s++) {
                for (f = 0; f < cmap->num_functions; f++) {
                        mliImage_set(&cmap->mat, s, f, mliColor_set(0, 0, 0));
                }
        }
        return 1;
error:
        return 0;
}

void mliScenery_histogram_optical_function_references(
        const struct mliScenery *scenery,
        uint64_t *num_function_references)
{
        uint64_t i = 0;
        for (i = 0; i < scenery->resources.num_functions; i++) {
                num_function_references[i] = 0u;
        }

        for (i = 0; i < scenery->num_primitives; i++) {
                struct mliBoundaryLayer layer = mliScenery_object_surfaces(
                        scenery,
                        i);

                struct mliSurface outer_surface = scenery->resources.surfaces[
                        layer.outer.surface];
                struct mliSurface inner_surface = scenery->resources.surfaces[
                        layer.inner.surface];

                struct mliMedium outer_medium = scenery->resources.media[
                        layer.outer.medium];
                struct mliMedium inner_medium = scenery->resources.media[
                        layer.inner.medium];

                num_function_references[inner_surface.specular_reflection]++;
                num_function_references[inner_surface.diffuse_reflection]++;

                num_function_references[outer_surface.specular_reflection]++;
                num_function_references[outer_surface.diffuse_reflection]++;

                num_function_references[outer_medium.refraction]++;
                num_function_references[outer_medium.absorbtion]++;

                num_function_references[inner_medium.refraction]++;
                num_function_references[inner_medium.absorbtion]++;
        }
}

int mliScenery_num_optical_functions(
        const struct mliScenery *scenery,
        uint64_t *num_optical_functions)
{
        uint64_t i = 0u;
        uint64_t *num_function_references = NULL;
        mli_malloc(
                num_function_references,
                uint64_t,
                scenery->resources.num_functions);
        mliScenery_histogram_optical_function_references(
                scenery,
                num_function_references);

        (*num_optical_functions) = 0u;
        for (i = 0; i < scenery->resources.num_functions; i++) {
                if (num_function_references[i] > 0u) {
                        (*num_optical_functions)++;
                }
        }

        free(num_function_references);
        return 1;
error:
        free(num_function_references);
        return 0;
}

#endif

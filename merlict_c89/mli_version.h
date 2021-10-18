/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_VERSION_H_
#define MERLICT_C89_MLI_VERSION_H_

#include <stdio.h>

#define MLI_VERSION_MAYOR 1
#define MLI_VERSION_MINOR 5
#define MLI_VERSION_PATCH 2

void mli_logo_fprint(FILE *f);
void mli_authors_and_affiliations_fprint(FILE *f);
#endif

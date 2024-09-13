/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VERSION_H_
#define MLI_VERSION_H_

#include <stdio.h>

#define MLI_VERSION_MAYOR 1
#define MLI_VERSION_MINOR 10
#define MLI_VERSION_PATCH 0

void mli_logo_fprint(FILE *f);
void mli_authors_and_affiliations_fprint(FILE *f);
#endif

/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VERSION_H_
#define MLI_VERSION_H_

#include <stdio.h>

#define MLI_VERSION_MAYOR 2
#define MLI_VERSION_MINOR 3
#define MLI_VERSION_PATCH 1

void mli_version_logo_fprint(FILE *f);
void mli_version_authors_and_affiliations_fprint(FILE *f);
#endif

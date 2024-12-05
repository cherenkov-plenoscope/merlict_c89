/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "version.h"

void mli_version_logo_fprint(FILE *f)
{
        fprintf(f,
                "\n  "
                "                                        _/  _/              "
                "_/\n  "
                "   _/_/_/  _/_/      _/_/    _/  _/_/  _/        _/_/_/  "
                "_/_/_/_/\n  "
                "  _/    _/    _/  _/_/_/_/  _/_/      _/  _/  _/          "
                "_/\n  "
                " _/    _/    _/  _/        _/        _/  _/  _/          _/\n "
                " "
                "_/    _/    _/    _/_/_/  _/        _/  _/    _/_/_/      "
                "_/_/\n  "
                "\n");
}

void mli_version_authors_and_affiliations_fprint(FILE *f)
{
        fprintf(f,
                "  Sebastian Achim Mueller (1,2*,3^)\n"
                "\n"
                "  [1] Max-Planck-Institute for Nuclear Physics, \n"
                "      Saupfercheckweg 1, 69117 Heidelberg, Germany\n"
                "\n"
                "  [2] Institute for Particle Physics and Astrophysics,\n"
                "      ETH-Zurich, Otto-Stern-Weg 5, 8093 Zurich, Switzerland\n"
                "\n"
                "  [3] Experimental Physics Vb, Astroparticle Physics,\n"
                "      TU-Dortmund, Otto-Hahn-Str. 4a, 44227 Dortmund, "
                "Germany\n"
                "\n"
                "   *  (2015 - 2019)\n"
                "   ^  (2013 - 2015)\n");
}

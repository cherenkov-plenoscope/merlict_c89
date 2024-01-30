import os
import glob
import argparse

parser = argparse.ArgumentParser(
    prog="identify_missing_includes.py",
    description=(
        "Raise awareness of missing includes in '.c' and '.h' files."
    ),
)
parser.add_argument(
    "lib",
    metavar="PATH",
    type=str,
    help=(
        "e.g. 'libs/mli' or 'libs/mli_corsika'."
    ),
)

args = parser.parse_args()
libpaths = args.lib

cpaths = glob.glob(os.path.join(libpaths, "src", "*.c"))
cpaths = sorted(cpaths)
ccode = {}
for cpath in cpaths:
    with open(cpath) as f:
        ccode[cpath] = f.read()

hpaths = glob.glob(os.path.join(libpaths, "src", "*.h"))
hpaths = sorted(hpaths)
hcode = {}
for hpath in hpaths:
    with open(hpath) as f:
        hcode[hpath] = f.read()


# assert
for cpath in ccode:
    if "assert(" in ccode[cpath]:
        if not "#include <assert.h>" in ccode[cpath]:
            print("Expected #include <assert.h> in {:s}.".format(cpath))

stdint_keys = [
    "int8_t",
    "int16_t",
    "int32_t",
    "int64_t",
]


# stdint
for p in hcode:
    for key in stdint_keys:
        if key in hcode[p]:
            if (
                not "#include <stdint.h>" in hcode[p]
                and not "#include <inttypes.h>" in hcode[p]
            ):
                print(
                    "Expected #include <stdint.h> / <inttypes.h> in {:s}.".format(
                        p
                    )
                )


# chk
for cpath in ccode:
    if "error:" in ccode[cpath]:
        if not '#include "chk.h"' in ccode[cpath]:
            print('Expected #include "chk.h" in {:s}.'.format(cpath))

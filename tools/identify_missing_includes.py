#!/usr/bin/env python3
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
    "src_directory",
    metavar="SRC_DIRECTORY",
    nargs="?",
    type=str,
    default="src",
    help=("Path to the 'src' directory."),
)

args = parser.parse_args()
src_directory = args.src_directory

cpaths = []
hpaths = []
for subdir in os.walk(src_directory):
    for filename in subdir[2]:
        filepath = os.path.join(subdir[0], filename)
        if filename.endswith(".h"):
            hpaths.append(filepath)
        elif filename.endswith(".c"):
            cpaths.append(filepath)
        else:
            print(f"Ignoring '{filepath:s}'")

hpaths = sorted(hpaths)
cpaths = sorted(cpaths)

ccode = {}
for cpath in cpaths:
    with open(cpath) as f:
        ccode[cpath] = f.read()

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
                print("Expected '#include <stdint.h>' in {:s}.".format(p))

# chk
chk_include_path = "../chk/chk.h"

for cpath in ccode:
    if "chk_error:" in ccode[cpath]:
        if not '#include "{:s}"'.format(chk_include_path) in ccode[cpath]:
            print(
                'Expected #include "{:s}" in {:s}.'.format(
                    chk_include_path, cpath
                )
            )

# Check the copyright notice
# --------------------------

def warn_about_copyright(path, code):
    lines = code.splitlines()
    if len(lines) > 0:
        firts_line = lines[0]
        if not firts_line.startswith("/* Copyright"):
            print(f"Check copyright in: {path:s}")

for hpath in hcode:
    warn_about_copyright(hpath, hcode[hpath])

for cpath in ccode:
    warn_about_copyright(cpath, hcode[hpath])

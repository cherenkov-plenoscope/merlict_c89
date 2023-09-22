import os
import glob

cpaths = glob.glob(os.path.join("src", "*.c"))
cpaths = sorted(cpaths)
ccode = {}
for cpath in cpaths:
    with open(cpath) as f:
        ccode[cpath] = f.read()

hpaths = glob.glob(os.path.join("src", "*.h"))
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
    "int8_t", "int16_t", "int32_t", "int64_t",
]


# stdint
for p in hcode:
    for key in stdint_keys:
        if key in hcode[p]:
            if not "#include <stdint.h>" in hcode[p] and not "#include <inttypes.h>" in hcode[p]:
                print("Expected #include <stdint.h> / <inttypes.h> in {:s}.".format(p))


# chk_debug
for cpath in ccode:
    if "error:" in ccode[cpath]:
        if not '#include "chk_debug.h"' in ccode[cpath]:
            print('Expected #include "chk_debug.h" in {:s}.'.format(cpath))

            lines = str.splitlines(ccode[cpath])

            armed = False
            for l in range(len(lines)):
                if "include" in lines[l]:
                    armed = True
                if armed and not "include" in lines[l]:
                    break

            lines.insert(l, '#include "chk_debug.h"')

            ccode[cpath] = str.join("\n", lines)

            with open(cpath, "wt") as f:
                f.write(ccode[cpath])
                f.write("\n")

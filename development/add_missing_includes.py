import os
import glob

cpaths = glob.glob(os.path.join("merlict", "*.c"))
cpaths = sorted(cpaths)

ccode = {}
for cpath in cpaths:
    with open(cpath) as f:
        ccode[cpath] = f.read()

# assert
for cpath in ccode:
    if "assert(" in ccode[cpath]:
        if not "#include <assert.h>" in ccode[cpath]:
            print("Expected #include <assert.h> in {:s}.".format(cpath))


# error
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
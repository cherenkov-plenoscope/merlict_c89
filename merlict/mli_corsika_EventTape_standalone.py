# make minimal header-only EventTape lib.
import os
import io

headers = [
    "chk_debug.h",
    "mli_version.h",
    "mli_math.h",
    "mli_cstr.h",
    "mli_cstr_numbers.h",
    "mliTar.h",
    "mliDynArray.h",
    "mliDynFloat.h",
    "mli_corsika_version.h",
    "mli_corsika_utils.h",
    "mli_corsika_EventTape.h",
]


def line_is_include_std(line):
    return line[0:10] == "#include <"


def line_is_include(line):
    return line[0:10] == '#include "'


def get_include_path_from_line(line):
    return line[10:-2]


std_includes = []

sources = []
for header in headers:
    source = str.replace(header, ".h", ".c")
    if os.path.exists(source):
        sources.append(source)

fheader = io.StringIO()
for header in headers:
    with open(header, "rt") as fin:
        fheader.write("/* {:s} */\n".format(header))
        for line in fin.readlines():
            if line_is_include_std(line):
                include_path = get_include_path_from_line(line)
                std_includes.append(include_path)
            elif line_is_include(line):
                include_path = get_include_path_from_line(line)
                if include_path not in headers:
                    fheader.write(line)
            else:
                fheader.write(line)
        fheader.write("\n\n\n")
fheader.seek(0)
header_str = fheader.read()

fsource = io.StringIO()
for source in sources:
    with open(source, "rt") as fin:
        fsource.write("/* {:s} */\n".format(source))
        for line in fin.readlines():
            if line_is_include_std(line):
                include_path = get_include_path_from_line(line)
                std_includes.append(include_path)
            elif line_is_include(line):
                include_path = get_include_path_from_line(line)
                if include_path not in headers:
                    fsource.write(line)
            else:
                fsource.write(line)
        fsource.write("\n\n\n")
fsource.seek(0)
source_str = fsource.read()

std_includes = list(set(std_includes))

# headeronly
# ----------
headeronly_path = "mli_corsika_EventTape_standalone_headeronly.h"

with open(headeronly_path, "wt") as f:
    for std_include in std_includes:
        f.write("#include <{:s}>\n".format(std_include))
    f.write("\n\n\n")
    f.write(header_str)
    f.write(source_str)

# header and source
# -----------------
header_path = "mli_corsika_EventTape_standalone.h"
source_path = "mli_corsika_EventTape_standalone.c"
with open(header_path, "wt") as f:
    for std_include in std_includes:
        f.write("#include <{:s}>\n".format(std_include))
    f.write("\n\n\n")
    f.write(header_str)

with open(source_path, "wt") as f:
    f.write('#include "{:s}"\n'.format(header_path))
    f.write("\n\n\n")
    f.write(source_str)

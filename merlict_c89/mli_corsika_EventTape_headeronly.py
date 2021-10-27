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
    "mliDynArray_template.h",
    "mliPhoton.h",
    "mli_corsika_version.h",
    "mli_corsika_utils.h",
    "mli_corsika_CorsikaPhotonBunch.h",
    "mli_corsika_EventTape.h",
]

def line_is_include_std(line):
    return line[0:10] == '#include <'

def line_is_include(line):
    return line[0:10] == '#include "'

def get_include_path_from_line(line):
    return line[10: -2]

std_includes = []

sources = []
for header in headers:
    source = str.replace(header, ".h", ".c")
    if os.path.exists(source):
        sources.append(source)

fout = io.StringIO()

for header in headers:
    with open(header, "rt") as fin:
        fout.write("/* {:s} */\n".format(header))
        for line in fin.readlines():
            if line_is_include_std(line):
                include_path = get_include_path_from_line(line)
                std_includes.append(include_path)
            elif line_is_include(line):
                include_path = get_include_path_from_line(line)
                if include_path not in headers:
                    fout.write(line)
            else:
                fout.write(line)
        fout.write("\n\n\n")

for source in sources:
    with open(source, "rt") as fin:
        fout.write("/* {:s} */\n".format(source))
        for line in fin.readlines():
            if line_is_include_std(line):
                include_path = get_include_path_from_line(line)
                std_includes.append(include_path)
            elif line_is_include(line):
                include_path = get_include_path_from_line(line)
                if include_path not in headers:
                    fout.write(line)
            else:
                fout.write(line)
        fout.write("\n\n\n")


std_includes = list(set(std_includes))
out_path = "mli_corsika_EventTape_headeronly.h"
fout.seek(0)

with open(out_path, "wt") as f:
    for std_include in std_includes:
        f.write("#include <{:s}>\n".format(std_include))
    f.write("\n\n\n")
    f.write(fout.read())

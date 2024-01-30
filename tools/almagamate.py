import argparse
import os
import io
import numpy as np


def line_is_include(line):
    return line[0:10] == '#include "'


def line_is_include_std(line):
    return line[0:10] == "#include <"


def get_include_path_from_line(line, include_type):
    if include_type == "std":
        start = str.find(line, "<") + 1
        stop = str.find(line, ">")
        return line[start:stop]

    elif include_type == "reg":
        start = str.find(line, '"') + 1
        return line[start:-1]


SOURCE_TYPES = ["c", "h", "test.c"]


def read_lib(path, source_types=SOURCE_TYPES):
    out = {}
    for key in source_types:
        out[key] = {}
    for ooo in os.walk(path):
        for filename in ooo[2]:
            source_path = os.path.join(ooo[0], filename)
            with open(source_path, "rt") as fin:
                source_text = fin.read()
            matching_keys = []
            for key in source_types:
                ext = "." + key
                if ext in filename:
                    matching_keys.append(key)
            matching_key = matching_keys[
                np.argmax([len(i) for i in matching_keys])
            ]
            matching_ext = "." + matching_key
            mfilename = filename.replace(matching_ext, "")

            out[matching_key][mfilename] = {
                "source": source_text,
                "path": source_path,
            }
    return out


def strip_non_std_includes(source):
    source_out_lines = []
    for line in str.splitlines(source):
        if not line_is_include(line) and not line_is_include_std(line):
            source_out_lines.append(line)
    return str.join("\n", source_out_lines)


def gather_includes(source, include_type):
    if include_type == "std":
        line_match = line_is_include_std
    elif include_type == "reg":
        line_match = line_is_include
    else:
        raise AssertionError()

    includes = []
    for line in str.splitlines(source):
        if line_match(line):
            include = get_include_path_from_line(
                line, include_type=include_type
            )
            includes.append(include)
    return list(set(includes))


# def main():
parser = argparse.ArgumentParser(
    prog="almagamate.py",
    description=(
        "Makes a single header-file and a single source-file "
        "out of the requested submodules from merlict_c89/libs/."
    ),
)
parser.add_argument(
    "outdir",
    metavar="PATHS",
    type=str,
    help="Output directory for header and source.",
)
parser.add_argument(
    "libs",
    metavar="PATHS",
    nargs="+",
    type=str,
    help=(
        "A list of the libs to be almagamated e.g. "
        "'libs/mli', 'libs/mli_corsika'."
    ),
)
parser.add_argument(
    "-t",
    "--test",
    help="make test.main.c",
    action="store_true",
)

args = parser.parse_args()
libpaths = args.libs
outdir = args.outdir
dotest = args.test

if dotest:
    if "libs/mli_testing" not in libpaths:
        libpaths.append("libs/mli_testing")

os.makedirs(outdir, exist_ok=True)
libnames = str.join("-", [os.path.basename(lp) for lp in libpaths])
header_path = os.path.join(outdir, libnames + ".h")
source_path = os.path.join(outdir, libnames + ".c")
test_path = os.path.join(outdir, libnames + ".test.c")

sources = {}
for source_type in SOURCE_TYPES:
    sources[source_type] = {}

for libpath in libpaths:
    libname = os.path.basename(libpath)
    lib = read_lib(os.path.join(libpath, "src"))
    for source_type in SOURCE_TYPES:
        for filename in lib[source_type]:
            assert filename not in sources[source_type]
            sources[source_type][filename] = lib[source_type][filename]

for source_type in SOURCE_TYPES:
    for filename in sources[source_type]:
        source = sources[source_type][filename]["source"]
        istd = gather_includes(source, include_type="std")
        ireg = gather_includes(source, include_type="reg")
        ireg = [os.path.basename(p) for p in ireg]
        ireg = [os.path.splitext(p)[0] for p in ireg]
        sources[source_type][filename]["includes"] = {
            "std": istd,
            "reg": ireg,
        }

all_includes_from_std = {}
for source_type in SOURCE_TYPES:
    all_includes_from_std[source_type] = []
    for filename in sources[source_type]:
        for incl in sources[source_type][filename]["includes"]["std"]:
            all_includes_from_std[source_type].append(incl)
    all_includes_from_std[source_type] = list(
        set(all_includes_from_std[source_type])
    )

all_includes_from_std["c"] = list(
    set(all_includes_from_std["c"]).difference(set(all_includes_from_std["h"]))
)

for source_type in SOURCE_TYPES:
    all_includes_from_std[source_type] = sorted(
        all_includes_from_std[source_type]
    )

# write header
# ------------
list_headers = []
so = io.StringIO()
for incl in all_includes_from_std["h"]:
    so.write("#include <{:s}>\n".format(incl))
so.write("\n")

ii = 0
initial_num_sources = len(sources["h"])

insources = set(sources["h"].keys())
inheader = set()

while True:
    filenames_not_yet_in_header = list(insources.difference(inheader))
    filenames_not_yet_in_header = sorted(filenames_not_yet_in_header)

    if len(filenames_not_yet_in_header) == 0:
        break

    ii += 1
    assert (
        ii <= initial_num_sources**2
    ), "Dependencies can not be resolved in {:s}".format(
        str(filenames_not_yet_in_header)
    )

    for filename in filenames_not_yet_in_header:
        depends = False
        for dep in sources["h"][filename]["includes"]["reg"]:
            if dep not in inheader:
                depends = True

        if not depends:
            so.write("/* {:s} */\n".format(filename))
            so.write("/* " + "-" * len(filename) + " */\n\n")
            so.write(strip_non_std_includes(sources["h"][filename]["source"]))
            so.write("\n")
            so.write("\n")
            inheader.add(filename)
            list_headers.append(sources["h"][filename]["path"])

so.seek(0)
with open(header_path, "wt") as fout:
    fout.write(so.read())


list_sources = []

# write source
# ------------
so = io.StringIO()
header_filename = os.path.basename(header_path)
so.write('#include "{:s}"\n'.format(header_filename))
so.write("\n")

for incl in all_includes_from_std["c"]:
    so.write("#include <{:s}>\n".format(incl))
so.write("\n")

sorted_c_sources_filenames = sorted(list(sources["c"].keys()))
for filename in sorted_c_sources_filenames:
    so.write("/* {:s} */\n".format(filename))
    so.write("/* " + "-" * len(filename) + " */\n\n")
    so.write(strip_non_std_includes(sources["c"][filename]["source"]))
    so.write("\n")
    so.write("\n")
    inheader.add(filename)
    list_sources.append(sources["c"][filename]["path"])

so.seek(0)
with open(source_path, "wt") as fout:
    fout.write(so.read())


list_tests = []
sorted_c_sources_filenames = sorted(list(sources["test.c"].keys()))
for filename in sorted_c_sources_filenames:
    list_tests.append(sources["test.c"][filename]["path"])

relpath_outdir_to_libs = os.path.relpath(".", start=outdir)
ro2l = relpath_outdir_to_libs

if dotest:
    o = ""
    o += "#include <stdlib.h>\n"
    o += "\n"
    for hpath in list_headers:
        o += '#include "{:s}"\n'.format(os.path.join(ro2l, hpath))
    o += "\n"
    for cpath in list_sources:
        o += '#include "{:s}"\n'.format(os.path.join(ro2l, cpath))
    o += "\n"
    o += "int main(void)\n"
    o += "{\n"
    o += '        printf("MLI_VERSION %d.%d.%d\\n",\n'
    o += "               MLI_VERSION_MAYOR,\n"
    o += "               MLI_VERSION_MINOR,\n"
    o += "               MLI_VERSION_PATCH);\n"
    o += "\n"
    for path in list_tests:
        o += '#include "{:s}"\n'.format(os.path.join(ro2l, path))
    o += "\n"
    o += '        printf("__SUCCESS__\\n");\n'
    o += "        return EXIT_SUCCESS;\n"
    o += "test_failure:\n"
    o += '        printf("__FAILURE__\\n");\n'
    o += "        return EXIT_FAILURE;\n"
    o += "}\n"
    test_main_path = os.path.join(outdir, libnames + ".test.main.c")
    with open(test_main_path, "wt") as f:
        f.write(o)


"""
if __name__ == "__main__":
    main()
"""

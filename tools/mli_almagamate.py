import argparse
import os
import io


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


def read_lib(path):
    cs = {}
    hs = {}
    for ooo in os.walk(path):
        for filename in ooo[2]:
            source_path = os.path.join(ooo[0], filename)
            source_name, source_ext = os.path.splitext(filename)
            with open(source_path, "rt") as fin:
                source_text = fin.read()
            if source_ext == ".h":
                hs[source_name] = {"source": source_text}
            elif source_ext == ".c":
                cs[source_name] = {"source": source_text}

    return {"h": hs, "c": cs}


def strip_non_std_includes(source):
    source_out_lines = []
    for line in str.splitlines(source):
        if not line_is_include(line):
            source_out_lines.append(line)
    return str.join("\n", source_out_lines)


def write_single_source(path, libs, source_type="h", header=""):
    so = io.StringIO()
    so.write(header)
    so.write("\n")
    for libname in libs:
        for spath in libs[libname][source_type]:
            so.write("/* {:s} */\n".format(spath))
            so.write("/* " + "-" * len(spath) + " */\n\n")
            so.write(strip_non_std_includes(libs[libname][source_type][spath]))
            so.write("\n")
            so.write("\n")
    so.seek(0)
    with open(path, "wt") as fout:
        fout.write(so.read())


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


def all_includes_from(libs, source_type, include_type):
    all_includes = []
    for libname in libs:
        for hpath in libs[libname][source_type]:
            all_includes += libs[libname][source_type][hpath]["includes"][
                include_type
            ]
    return list(set(all_includes))


"""
def main():
    parser = argparse.ArgumentParser(
        prog="mli_almagamate.py",
        description=(
            "Makes a single header-file and a single source-file "
            "out of the requested libraries."
        ),
    )
    parser.add_argument(
        "outdir",
        metavar="PATH",
        type=str,
        help=("The directory to write the source-files to."),
    )
    parser.add_argument(
        "libpaths",
        metavar="PATH",
        nargs="+",
        type=str,
        help=("A list of the required libraries e.g.: chk_debug mli_core."),
    )
    args = parser.parse_args()
    outdir = args.outdir
    libpaths = args.libpaths
"""

outdir = "."
libpaths = ["chk_debug", "mli_core", "mli_viewer"]

sources = {"c": {}, "h": {}}
for libpath in libpaths:
    libname = os.path.basename(libpath)
    lib = read_lib(os.path.join(libpath, "src"))
    for source_type in ["h", "c"]:
        for filename in lib[source_type]:
            assert filename not in sources[source_type]
            sources[source_type][filename] = lib[source_type][filename]

for source_type in ["h", "c"]:
    for filename in sources[source_type]:
        source = sources[source_type][filename]["source"]
        istd = gather_includes(source, include_type="std")
        ireg = gather_includes(source, include_type="reg")
        #ireg = [os.path.basename(p) for p in ireg]
        sources[source_type][filename]["includes"] = {"std": istd, "reg": ireg}

for filename in sources["h"]:
    print(sources[source_type][filename]["includes"]["reg"])

"""
libs = {}
for libpath in libpaths:
    libname = os.path.basename(libpath)
    libs[libname] = read_lib(os.path.join(libpath, "src"))
libnames = list(libs.keys())

for libname in libs:
    for hpath in libs[libname]["h"]:
        source = libs[libname]["h"][hpath]["source"]
        libs[libname]["h"][hpath]["includes"] = {}
        libs[libname]["h"][hpath]["includes"]["std"] = gather_includes(
            source, include_type="std"
        )
        libs[libname]["h"][hpath]["includes"]["reg"] = gather_includes(
            source, include_type="reg"
        )

    for cpath in libs[libname]["c"]:
        source = libs[libname]["c"][cpath]["source"]
        libs[libname]["c"][cpath]["includes"] = {}
        libs[libname]["c"][cpath]["includes"]["std"] = gather_includes(
            source, include_type="std"
        )
        libs[libname]["c"][cpath]["includes"]["reg"] = gather_includes(
            source, include_type="reg"
        )


all_includes_from_std_in_h = all_includes_from(
    libs=libs, source_type="h", include_type="std"
)
all_includes_from_std_in_c = all_includes_from(
    libs=libs, source_type="c", include_type="std"
)


name = str.join("_", libnames)
name_h = name + ".h"
name_c = name + ".c"



so = io.StringIO()
for std_include in all_includes_from_std_in_h:
    so.write("#include <{:s}>\n".format(std_include))
so.write("\n")

for libname in libs:
    for spath in libs[libname][source_type]:
        so.write("/* {:s} */\n".format(spath))
        so.write("/* " + "-" * len(spath) + " */\n\n")
        so.write(strip_non_std_includes(libs[libname][source_type][spath]))
        so.write("\n")
        so.write("\n")
so.seek(0)
with open(path, "wt") as fout:
    fout.write(so.read())


write_single_source(path=os.path.join(outdir, name_h), libs=libs, source_type="h")
write_single_source(
    path=os.path.join(outdir, name_c),
    libs=libs,
    source_type="c",
    header='#include "{:s}"'.format(name_h),
)


if __name__ == "__main__":
    main()
"""

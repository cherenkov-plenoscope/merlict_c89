import argparse
import os
import io


def line_is_include(line):
    return line[0:10] == '#include "'


def read_lib(path):
    cs = {}
    hs = {}
    for ooo in os.walk(path):
        for filename in ooo[2]:
            source_path = os.path.join(ooo[0], filename)
            with open(source_path, "rt") as fin:
                source_text = fin.read()
            source_ext = os.path.splitext(filename)[-1]
            if source_ext == ".h":
                hs[source_path] = source_text
            elif source_ext == ".c":
                cs[source_path] = source_text

    return {"h": hs, "c": cs}


def strip_non_std_includes(source):
    source_out_lines = []
    for line in str.splitlines(source):
        if not line_is_include(line):
            source_out_lines.append(line)
    return str.join("\n", source_out_lines)


def write_single_source(path, libs, key="h", header=""):
    so = io.StringIO()
    so.write(header)
    so.write("\n")
    for libname in libs:
        for spath in libs[libname][key]:
            so.write("/* {:s} */\n".format(spath))
            so.write("/* " + "-" * len(spath) + " */\n\n")
            so.write(strip_non_std_includes(libs[libname][key][spath]))
            so.write("\n")
            so.write("\n")
    so.seek(0)
    with open(path, "wt") as fout:
        fout.write(so.read())


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
        "libnames",
        metavar="LIBS",
        nargs="+",
        type=str,
        help=("A list of the required libraries e.g.: chk_debug mli_core."),
    )
    args = parser.parse_args()
    outdir = args.outdir
    libnames = args.libnames

    libs = {}
    for libname in libnames:
        libs[libname] = read_lib(os.path.join(libname, "src"))

    name = str.join("_", libnames)
    name_h = name + ".h"
    name_c = name + ".c"

    write_single_source(path=os.path.join(outdir, name_h), libs=libs, key="h")
    write_single_source(
        path=os.path.join(outdir, name_c),
        libs=libs,
        key="c",
        header='#include "{:s}"'.format(name_h),
    )


if __name__ == "__main__":
    main()

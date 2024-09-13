#!/usr/bin/env python3
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


SOURCE_TYPES = ["c", "h", "test.c"]


def argmax(iterable):
    return max(enumerate(iterable), key=lambda x: x[1])[0]


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
                argmax([len(i) for i in matching_keys])
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


def limit_sources(sources, source_paths):
    spaths = set([os.path.splitext(p)[0] for p in source_paths])
    out = {}
    for source_type in SOURCE_TYPES:
        out[source_type] = {}
        for filename in sources[source_type]:
            srcpath = os.path.splitext(sources[source_type][filename]["path"])[
                0
            ]
            if srcpath in spaths:
                out[source_type][filename] = sources[source_type][filename]
    return out


def make_test_main(
    relpath_outdir_to_libs, header_paths, source_paths, test_paths
):
    ro2l = relpath_outdir_to_libs
    o = ""
    o += "#include <stdlib.h>\n"
    o += "\n"
    for hpath in header_paths:
        o += '#include "{:s}"\n'.format(os.path.join(ro2l, hpath))
    o += "\n"
    for cpath in source_paths:
        o += '#include "{:s}"\n'.format(os.path.join(ro2l, cpath))
    o += "\n"
    o += "int main(void)\n"
    o += "{\n"
    o += '        printf("MLI_VERSION %d.%d.%d\\n",\n'
    o += "               MLI_VERSION_MAYOR,\n"
    o += "               MLI_VERSION_MINOR,\n"
    o += "               MLI_VERSION_PATCH);\n"
    o += "\n"
    for tpath in test_paths:
        o += '#include "{:s}"\n'.format(os.path.join(ro2l, tpath))
    o += "\n"
    o += '        printf("__SUCCESS__\\n");\n'
    o += "        return EXIT_SUCCESS;\n"
    o += "test_failure:\n"
    o += '        printf("__FAILURE__\\n");\n'
    o += "        return EXIT_FAILURE;\n"
    o += "}\n"
    return o


def gather_sources(libpaths, source_types=SOURCE_TYPES):
    sources = {}
    for source_type in source_types:
        sources[source_type] = {}

    for libpath in libpaths:
        libname = os.path.basename(libpath)
        lib = read_lib(os.path.join(libpath, "src"))
        for source_type in source_types:
            for filename in lib[source_type]:
                assert filename not in sources[source_type]
                sources[source_type][filename] = lib[source_type][filename]

    for source_type in source_types:
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

    includes_from_std = {}
    for source_type in source_types:
        includes_from_std[source_type] = []
        for filename in sources[source_type]:
            for incl in sources[source_type][filename]["includes"]["std"]:
                includes_from_std[source_type].append(incl)
        includes_from_std[source_type] = list(
            set(includes_from_std[source_type])
        )

    includes_from_std["c"] = list(
        set(includes_from_std["c"]).difference(set(includes_from_std["h"]))
    )

    for source_type in source_types:
        includes_from_std[source_type] = sorted(includes_from_std[source_type])

    return sources, includes_from_std


def make_source_txt(c_sources, c_includes_from_std):
    list_sources = []
    so = io.StringIO()

    for incl in c_includes_from_std:
        so.write("#include <{:s}>\n".format(incl))
    so.write("\n")

    sorted_c_sources_filenames = sorted(list(c_sources.keys()))
    for filename in sorted_c_sources_filenames:
        so.write("/* {:s} */\n".format(filename))
        so.write("/* " + "-" * len(filename) + " */\n\n")
        so.write(strip_non_std_includes(c_sources[filename]["source"]))
        so.write("\n")
        so.write("\n")
        list_sources.append(c_sources[filename]["path"])
    so.seek(0)
    return so.read(), list_sources


def make_header_txt(h_source, h_includes_from_std):
    list_headers = []
    so = io.StringIO()
    for incl in h_includes_from_std:
        so.write("#include <{:s}>\n".format(incl))
    so.write("\n")

    ii = 0
    initial_num_sources = len(h_source)

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
            for dep in h_source[filename]["includes"]["reg"]:
                if dep not in inheader:
                    depends = True

            if not depends:
                so.write("/* {:s} */\n".format(filename))
                so.write("/* " + "-" * len(filename) + " */\n\n")
                so.write(strip_non_std_includes(h_source[filename]["source"]))
                so.write("\n")
                so.write("\n")
                inheader.add(filename)
                list_headers.append(h_source[filename]["path"])

    so.seek(0)

    return so.read(), list_headers


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
    "--header_path",
    metavar="PATH",
    type=str,
    help="Path of header.",
)
parser.add_argument(
    "--source_path",
    metavar="PATH",
    type=str,
    help="Path of source.",
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
parser.add_argument(
    "-c",
    "--cherry_pick",
    metavar="PATH",
    type=str,
    help="Cherry pick from a limited list of source files.",
)
parser.add_argument(
    "--header_only",
    help="no seperate source '.c', will append source to header.",
    action="store_true",
)

args = parser.parse_args()
libpaths = args.libs
outdir = args.outdir
cherrypickpath = args.cherry_pick

if args.test:
    if os.path.join("libs", "mli_testing") not in libpaths:
        libpaths.append(os.path.join("libs", "mli_testing"))

os.makedirs(outdir, exist_ok=True)
libnames = str.join("-", [os.path.basename(lp) for lp in libpaths])

sources, includes_from_std = gather_sources(
    libpaths=libpaths, source_types=SOURCE_TYPES
)

if cherrypickpath:
    cherrypick = {
        "path": cherrypickpath,
        "src": [],
        "name": os.path.splitext(os.path.basename(cherrypickpath))[0],
    }
    with open(cherrypickpath, "rt") as f:
        cherrypick["src"] = f.read().splitlines()
    sources = limit_sources(sources=sources, source_paths=cherrypick["src"])
    libnames = libnames + "-" + cherrypick["name"]

if args.header_only:
    libnames = libnames + "-headeronly"

header_path = os.path.join(outdir, libnames + ".h")
source_path = os.path.join(outdir, libnames + ".c")
test_path = os.path.join(outdir, libnames + ".test.c")

if args.header_path:
    header_path = args.header_path

if args.source_path:
    source_path = args.source_path

header_txt, list_headers = make_header_txt(
    h_source=sources["h"], h_includes_from_std=includes_from_std["h"]
)

with open(header_path, "wt") as fout:
    fout.write(header_txt)

source_txt, list_sources = make_source_txt(
    c_sources=sources["c"], c_includes_from_std=includes_from_std["c"]
)

if args.header_only:
    with open(header_path, "at") as fout:
        fout.write("#ifndef MLI_SOURCE_H_\n")
        fout.write("#define MLI_SOURCE_H_\n")
        fout.write("\n")
        fout.write(source_txt)
        fout.write("\n")
        fout.write("#endif /* MLI_SOURCE_H_ */\n")
else:
    header_filename = os.path.basename(header_path)
    with open(source_path, "wt") as fout:
        fout.write('#include "{:s}"\n'.format(header_filename))
        fout.write("\n")
        fout.write(source_txt)


if args.test:
    list_tests = []
    sorted_test_filenames = sorted(list(sources["test.c"].keys()))
    for filename in sorted_test_filenames:
        list_tests.append(sources["test.c"][filename]["path"])

    o = make_test_main(
        relpath_outdir_to_libs=os.path.relpath(".", start=outdir),
        header_paths=list_headers,
        source_paths=list_sources,
        test_paths=list_tests,
    )
    test_main_path = os.path.join(outdir, libnames + ".test.main.c")
    with open(test_main_path, "wt") as f:
        f.write(o)


"""
if __name__ == "__main__":
    main()
"""

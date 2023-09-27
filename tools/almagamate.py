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
            ireg = [os.path.basename(p) for p in ireg]
            ireg = [os.path.splitext(p)[0] for p in ireg]
            sources[source_type][filename]["includes"] = {
                "std": istd,
                "reg": ireg,
            }

    all_includes_from_std = {}
    for source_type in ["h", "c"]:
        all_includes_from_std[source_type] = []
        for filename in sources[source_type]:
            for incl in sources[source_type][filename]["includes"]["std"]:
                all_includes_from_std[source_type].append(incl)
        all_includes_from_std[source_type] = list(
            set(all_includes_from_std[source_type])
        )

    all_includes_from_std["c"] = list(
        set(all_includes_from_std["c"]).difference(
            set(all_includes_from_std["h"])
        )
    )

    for source_type in ["h", "c"]:
        all_includes_from_std[source_type] = sorted(
            all_includes_from_std[source_type]
        )

    libnames = [os.path.basename(p) for p in libpaths]
    name = str.join("_", libnames)
    name_h = name + ".h"
    name_c = name + ".c"

    # write header
    # ------------
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
            ii < initial_num_sources**2
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
                so.write(
                    strip_non_std_includes(sources["h"][filename]["source"])
                )
                so.write("\n")
                so.write("\n")
                inheader.add(filename)

    so.seek(0)
    with open(name_h, "wt") as fout:
        fout.write(so.read())

    # write source
    # ------------
    so = io.StringIO()
    so.write('#include "{:s}"\n'.format(name_h))
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

    so.seek(0)
    with open(name_c, "wt") as fout:
        fout.write(so.read())


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
import argparse
import os
import io
import subprocess


com = {}
com["gcc"] = {
    "compiler": "gcc",
    "flags": ["-lm", "-Wall", "-pedantic", "-Wextra", "-Wstrict-prototypes"],
}
com["gcc-c89"] = {
    "compiler": "gcc",
    "flags": ["-std=c89"] + com["gcc"]["flags"],
}
com["g++"] = {
    "compiler": "g++",
    "flags": ["-lm", "-Wall", "-pedantic", "-Wextra"],
}
com["clang-c89"] = {"compiler": "clang", "flags": com["gcc-c89"]["flags"]}


parser = argparse.ArgumentParser(
    prog="compile_and_test.py",
    description=(
        "Makes a single header-file and a single source-file "
        "out of the requested modules from merlict_c89."
    ),
)
args = parser.parse_args()
module_paths = [
    os.path.join("src", "chk"),
    os.path.join("src", "version"),
    os.path.join("src", "magicid"),
    os.path.join("src", "math"),
    os.path.join("src", "vector"),
    os.path.join("src", "array"),
    os.path.join("src", "cstr"),
    os.path.join("src", "string"),
    os.path.join("src", "argv"),
    os.path.join("src", "map"),
    os.path.join("src", "path"),
    os.path.join("src", "prng"),
    os.path.join("src", "io"),
    os.path.join("src", "json"),
    os.path.join("src", "image"),
    os.path.join("src", "fresnel"),
    os.path.join("src", "lambertian"),
    os.path.join("src", "func"),
    os.path.join("src", "tar"),
    os.path.join("src", "avl"),
    os.path.join("src", "color"),
    os.path.join("src", "vec"),
    os.path.join("src", "aabb"),
    os.path.join("src", "ray"),
    os.path.join("src", "mat"),
    os.path.join("src", "cube"),
    os.path.join("src", "quaternion"),
    os.path.join("src", "homtra"),
    os.path.join("src", "object"),
    os.path.join("src", "medium"),
    os.path.join("src", "surface"),
    os.path.join("src", "materials"),
    os.path.join("src", "triangle"),
    os.path.join("src", "octree"),
    os.path.join("src", "boundarylayer"),
    os.path.join("src", "testing"),
    os.path.join("src", "mli"),
    os.path.join("src", "corsika"),
    os.path.join("src", "view"),
    os.path.join("src", "viewer"),
]

def print_file(path):
    with open(path, "rt") as f:
        print(f.read())


def run_and_save_sdtout(call, stdout_path):
    with open(stdout_path, "wt") as f:
        rc = subprocess.call(
            call,
            stderr=subprocess.STDOUT,
            stdout=f,
        )
    return rc


def tar_sceneries(scenery_name):
    in_path = os.path.join(
        "data", "mli", "tests", "resources", "sceneries", scenery_name
    )
    out_path = in_path + ".tar"
    call = ["tar", "-cvf", out_path, "--directory", in_path, "."]
    stdout_path = os.path.join("build", "tar_{:s}.o".format(scenery_name))
    return run_and_save_sdtout(
        call=call,
        stdout_path=stdout_path,
    )


def almagamate_sources(
    outdir,
    module_paths,
):
    stdout_path = os.path.join(outdir, "compile_and_test.o")
    return run_and_save_sdtout(
        call=[
            "python",
            os.path.join("tools", "almagamate.py"),
            "--test",
            outdir,
        ]
        + module_paths,
        stdout_path=stdout_path,
    )


def compile_sources(
    compiler,
    target,
    out_path,
    flags,
    stdout_path,
):
    return run_and_save_sdtout(
        call=[compiler, target, "-o", out_path] + flags,
        stdout_path=stdout_path,
    )


def run_tests(test_executable_path, stdout_path):
    return run_and_save_sdtout(
        call=[test_executable_path], stdout_path=stdout_path
    )


os.makedirs("build", exist_ok=True)
os.makedirs(os.path.join("build", "almagamate"), exist_ok=True)
os.makedirs(os.path.join("build", "tests"), exist_ok=True)

scenery_names = [
    "000",
    "001",
    "002",
    "optics_prism",
    "optics_focussing_mirror",
]

for scenery_name in scenery_names:
    rc = tar_sceneries(scenery_name=scenery_name)


almagamate_sources(
    outdir=os.path.join("build", "almagamate"),
    module_paths=module_paths,
)


os.makedirs(os.path.join("build", "tests", "mli"), exist_ok=True)

for comkey in com:
    print(comkey)
    out_path = os.path.join(
        "build", "tests", "mli", "test_{:s}".format(comkey)
    )
    rc = compile_sources(
        compiler=com[comkey]["compiler"],
        target=os.path.join(
            "build",
            "almagamate",
            "mli.test.main.c",
        ),
        out_path=out_path + ".exe",
        flags=com[comkey]["flags"],
        stdout_path=out_path + ".o",
    )
    print_file(path=out_path + ".o")

    rc = run_tests(
        test_executable_path=out_path + ".exe", stdout_path=out_path + ".exe.o"
    )
    if rc != 0:
        print_file(path=out_path + ".exe.o")

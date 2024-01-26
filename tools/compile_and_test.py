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
        "out of the requested libraries from merlict_c89."
    ),
)
args = parser.parse_args()
submodule_name = "mli"

def print_file(path):
    with open(path, "rt") as f:
        print(f.read())


def run_and_save_sdtout(call, stdout_path):
    p = subprocess.Popen(
        call,
        stderr=subprocess.STDOUT,
        stdout=subprocess.PIPE,
    )
    p.wait()
    o = p.stdout.read().decode()
    rc = int(p.returncode)

    with open(stdout_path, "wt") as f:
        f.write(o)
    return rc


def tar_sceneries(scenery_name):
    in_path = os.path.join(
        "libs", "mli", "tests", "resources", "sceneries", scenery_name
    )
    out_path = in_path + ".tar"
    call = ["tar", "-cvf", out_path, "--directory", in_path, "."]
    stdout_path = os.path.join("build", "tar_{:s}.o".format(scenery_name))
    return run_and_save_sdtout(
        call=call,
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


scenery_names = ["000", "001", "002", "optics_prism optics_focussing_mirror"]


for scenery_name in scenery_names:
    rc = tar_sceneries(scenery_name=scenery_name)


for comkey in com:
    print(comkey)
    out_path = os.path.join(
        "build", "tests", submodule_name, "test_{:s}".format(comkey)
    )
    os.makedirs(os.path.join("build", "tests", submodule_name), exist_ok=True)
    rc = compile_sources(
        compiler=com[comkey]["compiler"],
        target=os.path.join("libs", submodule_name, "tests", "main_entry_point.c"),
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

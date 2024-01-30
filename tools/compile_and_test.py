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
    print(o)

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


def almagamate_sources(
    outdir,
    libpath,
):
    stdout_path = os.path.join(outdir, "compile_and_test.o")
    return run_and_save_sdtout(
        call=[
            "python",
            os.path.join("tools", "almagamate.py"),
            "--test",
            outdir,
            libpath,
        ],
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

scenery_names = ["000", "001", "002", "optics_prism optics_focussing_mirror"]


almagamate_sources(
    outdir=os.path.join("build", "almagamate"),
    libpath=os.path.join("libs", submodule_name),
)


for scenery_name in scenery_names:
    rc = tar_sceneries(scenery_name=scenery_name)

os.makedirs(os.path.join("build", "tests", submodule_name), exist_ok=True)

for comkey in com:
    print(comkey)
    out_path = os.path.join(
        "build", "tests", submodule_name, "test_{:s}".format(comkey)
    )
    rc = compile_sources(
        compiler=com[comkey]["compiler"],
        target=os.path.join(
            "build",
            "almagamate",
            "{:s}-mli_testing.test.main.c".format(submodule_name),
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

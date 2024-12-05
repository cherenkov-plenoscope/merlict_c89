#!/usr/bin/env python3
import os
import subprocess
import glob

libs_dir = os.path.join("libs")
clang_format_file = os.path.join("tools", ".clang-format")

for namespace_dir in glob.glob(os.path.join(libs_dir, "*")):
    if os.path.isdir(namespace_dir):
        paths = []
        for wild in ["*.h", "*.c"]:
            wild_path = os.path.join(namespace_dir, wild)
            for pwp in glob.glob(wild_path):
                paths.append(pwp)
        subprocess.call(
            ["clang-format", "-i", f"--style=file:{clang_format_file:s}"]
            + paths
        )

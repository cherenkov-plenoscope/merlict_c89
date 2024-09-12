#!/usr/bin/env python3
import os
import subprocess
import glob

libs_dir = os.path.join("..", "libs")

for ppp in glob.glob(os.path.join(libs_dir, "*")):
    if os.path.isdir(ppp):
        possible_src_dir = os.path.join(ppp, "src")
        if os.path.exists(possible_src_dir):
            paths = []
            for wild in ["*.h", "*.c"]:
                wild_path = os.path.join(possible_src_dir, wild)
                for pwp in glob.glob(wild_path):
                    paths.append(pwp)
            subprocess.call(["clang-format", "-i"] + paths)

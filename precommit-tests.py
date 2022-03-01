#! /usr/bin/env python3

import os
import sys


def find_path_to_build(cur_path):
    if os.path.basename(cur_path) == "build":
        return cur_path
    if "build" in cur_path.split("/"):
        return find_path_to_build(os.path.split()[0])
    else:
        for next_dir in os.listdir(cur_path):
            if next_dir == ".git":
                continue
            next_path = os.path.join(cur_path, next_dir)
            if not os.path.isdir(next_path):
                continue
            next_path = find_path_to_build(next_path)
            if next_path is not None:
                return next_path
        return None


def verify_cmd(path, cmd):
    print("$> {}".format(cmd))
    exec_cmd = "(cd {}; {} > /dev/null 2>&1)".format(path, cmd)
    assert os.system(exec_cmd) == 0, "Error Running: \"{}\"".format(exec_cmd)


path = os.getcwd()
if len(sys.argv) > 1:
    path = sys.argv[1]

path = find_path_to_build(path)

for lang in ["C", "CXX"]:
    tst_cmds = [
        "cmake -DLANG={} ..".format(lang), "make clean", "make full",
        "./bench --all", "./tests --all", "make static-analysis"
    ]
    for tst_cmd in tst_cmds:
        verify_cmd(path, tst_cmd)

import os
import glob
from common_util import is_int, as_int

S_files = glob.glob("**/*-glibc*.S", recursive=True)

for f in S_files:
    lines = []
    for line in open(f):
        line = line.replace(
            "\tSection: .section .text.evex512, \"ax\", @progbits\n",
            "\t.section .text.evex512, \"ax\", @progbits\n")
        lines.append(line)
    f_out = open(f, "w+")
    f_out.write("".join(lines))
    f_out.close()

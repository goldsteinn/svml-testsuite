import os
for f in open("tmp.txt"):
    f = f.lstrip().rstrip()

    assert os.path.isfile(f)

    has = 0
    lines = []
    for line in open(f):
        if "exex512" in line:
            has += 1
        lines.append(line.replace("exex512", "evex512"))
    assert has == 1
#    f_out = open(f, "w+")
#    f_out.write("".join(lines))
#    f_out.close()

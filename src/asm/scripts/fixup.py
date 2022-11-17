import os
for line in open("errs.txt"):
    line = line.lstrip().rstrip()
    fname = line.replace("_", "-") + ".S"
    if not os.path.isfile(fname):
        continue

    pieces = line.split("_")
    func = pieces[0]
    n = pieces[1]
    ext = pieces[2]
    owner = pieces[-1]
    res = "_".join(pieces[3:-1])
    eext = ext

    expec = ""
    correct = ""
    dead = False
    if int(n) != 2:
        if ext == "sse2":
            eext = "sse"
        expec = "{}_{}_wrapper_{}_{}_wrapper".format(func, n, owner, eext)
        correct = line

    else:
        expec = "{}_{}_wrapper_{}_{}".format(func, n, owner, ext)
        correct = line
        dead = True

    lines = []
    for ol in open(fname):
        lines.append(ol.replace(expec, correct))

    fname = open(fname, "w+")
    fname.write("".join(lines) + "\n")
    fname.close()

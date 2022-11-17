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
    if "_vv_" in line:
        continue
    expec = ""
    correct = ""
    dead = False
    if False:
        if ext == "sse2":
            eext = "sse2"
        expec = "{}_{}_wrapper_vv_{}_{}".format(func, n, owner, ext)
        correct = line
    elif False:
        if ext == "sse2":
            eext = "sse"
        expec = "{}_{}_wrapper_vv_{}_{}_wrapper".format(func, n, owner, eext)
        correct = line
    elif True:
        if ext == "sse2":
            eext = "sse"
        expec = "{}_{}_{}_wrapped_{}_wrapper".format(func, n, ext, owner)
        correct = line                

    lines = []
    for ol in open(fname):
        lines.append(ol.replace(expec, correct))

    fname = open(fname, "w+")
    fname.write("".join(lines) + "\n")
    fname.close()

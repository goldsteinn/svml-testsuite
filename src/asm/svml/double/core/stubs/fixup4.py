import os
for line in open("errs.txt"):
    line = line.lstrip().rstrip()
    pieces = line.split("_")
    func = pieces[0]
    n = pieces[1]
    ext = pieces[2]
    
    fname = line.replace("_", "-").replace("-vv", "").replace(ext + "-", "") + ".S"
    print(fname)
    if not os.path.isfile(fname):
        continue

    owner = pieces[-1]
    res = "_".join(pieces[3:-1])
    eext = ext
    expec = ""
    correct = ""
    dead = False
    expec = "{}_{}_wrapper_vv_{}".format(func, n, owner)
    correct = line                

    lines = []
    for ol in open(fname):
        lines.append(ol.replace(correct, expec))

    fname = open(fname, "w+")
    fname.write("".join(lines) + "\n")
    fname.close()

import os
for line in open("errs.txt"):
    line = line.lstrip().rstrip()
    piece = line.split("_")[0]
    if piece[-1] != "f":
        continue

    fname = line.replace("_", "-").replace("wrapped", "wrapper").replace(
        piece, piece[:-1] + "-f") + ".S"
    for ext in ["avx2", "sse2", "avx512", "avx", "sse4"]:
        fname = fname.replace(ext + "-", "")

    if not os.path.isfile(fname):
        continue

    lines = []
    for ol in open(fname):
        lines.append(ol.replace(piece[:-1] + "_f_", piece + "_"))

    fname = open(fname, "w+")
    fname.write("".join(lines) + "\n")
    fname.close()

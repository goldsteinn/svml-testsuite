lines = []
for line in open("tmp.txt"):
    line = line.lstrip().rstrip()
    for cline in open(line):
        lines.append(cline.replace("glibc", "dev"))
    f = line.replace("glibc", "dev")
    f = open(f, "w+")
    f.write("".join(lines) + "\n")

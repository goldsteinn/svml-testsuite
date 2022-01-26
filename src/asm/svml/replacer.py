import os

lines = []

glibc_files = []
dev_files = []
for line in open("tmp-dev.txt"):
    line = line.lstrip().rstrip()
    assert os.path.isfile(line)
    dev_files.append(line)
for line in open("tmp-glibc.txt"):
    line = line.lstrip().rstrip()
    assert os.path.isfile(line)
    glibc_files.append(line)
        
for line in open("tmp.txt"):
    incl = "#include \"/home/noah/programs/projects/svml-new/src/asm/svml/{}\""
    line = line.lstrip().rstrip()
    for cline in open(line):
        lines.append(cline.replace("glibc", "dev"))
    f = line.replace("glibc", "dev")
    f = open(f, "w+")
    f.write("".join(lines) + "\n")
    f.close()
    old_incl = incl.format(line.replace("glibc-", ""))
    glibc_incl = incl.format(line)
    dev_incl = incl.format(line.replace("glibc", "dev"))
    
    for f in dev_files:
        lines = []
        for line in open(f):
            line = line.lstrip().rstrip()
            if line == "":
                continue            
            lines.append(line.replace(old_incl, dev_incl))
        f = open(f, "w+")
        f.write("\n".join(lines) + "\n")
        f.close()
    for f in glibc_files:
        lines = []
        for line in open(f):
            line = line.lstrip().rstrip()
            if line == "":
                continue
            lines.append(line.replace(old_incl, glibc_incl))
        f = open(f, "w+")
        f.write("\n".join(lines) + "\n")        
        f.close()

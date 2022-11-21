import os
import glob
from common_util import is_int, as_int

S_files = glob.glob("**/*-glibc*.S", recursive=True)
H_files = glob.glob("**/*-dev*.h", recursive=True)


def is_data(line):
    line = line.lstrip().rstrip()
    if line.startswith(".quad"):
        return True, 8
    if line.startswith(".long"):
        return True, 4
    return False, None


def is_align(line):
    line = line.lstrip().rstrip()
    pieces = line.split()
    if line.startswith(".align"):
        assert len(pieces) == 2
        return True, int(pieces[-1])
    if line.startswith(".p2align"):
        assert len(pieces) == 2
        return True, 1 << int(pieces[-1])
    return False, None


def is_data_start(line):
    line = line.lstrip().rstrip()
    return "data_internal" in line


def is_section(line):
    line = line.lstrip().rstrip()
    if line.startswith(".section"):
        if ".rodata" in line:
            return False
        return True
    elif line == ".text":
        return True
    return False


S_d_files = []
S_nd_files = []
sections = {}
for S_file in S_files:
    has_data = False
    found_data = False
    has_align = False
    has_section = False
    for line in open(S_file):
        has_data = has_data or is_data_start(line)
        fd, dlen = is_data(line)
        ia, alen = is_align(line)
        if is_section(line):
            assert not has_section
            sections.setdefault(line.lstrip().rstrip(), []).append(S_file)
            has_section = True
        if ia:
            has_align = ia
            assert has_section, S_file
        if fd:
            if not has_align:
                #                print(S_file)
                break
            has_align = False
        found_data = found_data or fd
        if found_data:
            assert has_section, S_file
            assert has_data, S_file

    if found_data:
        S_d_files.append(S_file)
    else:
        S_nd_files.append(S_file)

for section in sections:
    if section != ".text":
        continue
    print("Section: " + section)
    for f in sections[section]:
        Tsz = 0
        if f.startswith("double/"):
            Tsz = 8
        elif f.startswith("float/"):
            Tsz = 4
        assert Tsz != 0

        Nvec = 0
        pieces = f.split("-")
        for p in pieces:
            if is_int(p):
                assert Nvec == 0
                Nvec = int(p)
                assert Nvec != 0
        assert Nvec != 0

        sz = Nvec * Tsz

        assert sz in {16, 32, 64}

        if "wrapper" in f:
            wrap_sz = []
            has_entry = False
            has_section = False
            for line in open(f):
                line = line.lstrip().rstrip()
                if line == ".text":
                    assert not has_section
                    has_section = True
                if line.startswith("ENTRY"):
                    assert not has_entry
                    assert has_section
                    wrap_sz.append(0)
                    has_entry = True
                if line.startswith("WRAPPER_IMPL_"):
                    assert has_entry
                    has_entry = False
                    assert len(wrap_sz) != 0
                    assert wrap_sz[-1] == 0, f
                    assert line.count("WRAPPER_IMPL_") == 1
                    line = line.replace("WRAPPER_IMPL_", "")

                    if line.startswith("AVX512"):
                        wrap_sz[-1] = 64
                    elif line.startswith("AVX"):
                        wrap_sz[-1] = 32
                    elif line.startswith("SCALAR"):
                        wrap_sz[-1] = 16
                    else:
                        assert False
                else:
                    assert line.count("WRAPPER_IMPL_") == 0 or line.startswith(
                        ".macro")
            assert len(wrap_sz) != 0
            for wsz in wrap_sz:
                assert wsz == wrap_sz[0]
                assert wsz in {16, 32, 64}
                assert wsz == sz
#            assert sz in {8, 16, 32}
        else:
            assert "wrap" not in f
            okat = False
            if "-sse4-" in f:
                okay = True
                assert sz == 16
            if "-avx2-" in f or "-avx-" in f:
                okay = True
                assert sz == 32, f
            if "-avx512-" in f:
                okay = True
                assert sz == 64
            assert okay
                
        assert sz in {16, 32, 64}
        has_section = False
        lines = []
        if "-sse4-" in f:
            assert sz == 16
        if "-avx2-" in f or "-avx-" in f:
            assert sz == 32
        if "-avx512-" in f:
            assert sz == 64
        for line in open(f):
            checkln = line.lstrip().rstrip()
            if checkln.startswith(".section"):
                if not checkln.replace(".section", "").lstrip().rstrip().startswith(".rodata"):
                    assert False, f
            if checkln == ".text":
                assert not has_section
                has_section = True
                if sz == 16:
                    if "-sse4-" in f:
                        line = "\t.section .text.sse4, \"ax\", @progbits\n"
                    pass
                elif sz == 32:
                    line = "\t.section .text.avx2, \"ax\", @progbits\n"
                elif sz == 64:
                    line = "\t.section .text.evex512, \"ax\", @progbits\n"
            lines.append(line)
        assert has_section
        f_out = open(f, "w+")
        f_out.write("".join(lines))
        f_out.close()
                    


#print("---- With Data ----")
#print("\n".join(S_d_files))

import sys
from common_util import (strs, remove_ws, is_int)
from copy import (deepcopy)


def get_data_name(data_piece):
    name_map = {
        "3f800000": "_OneF",
        "ff800000": "_NotiOffExpoMask",
        "3f2aaaab": "_IBrkValue",
        "7fffffff": "_AbsMask",
        "80000000": "_SignMask",
        "01000000": "_ILoRange",
        "bf000000": "_Neg5F",
        "3f317218": "_Ln2",
        "fffff000": "_Neg4096",
        "3fc90fdb": "_TanSPI1_FMA",
    }

    assert data_piece in name_map, data_piece

    return name_map[data_piece]


G_offsets = {}
G_nimpls = 1000


class Common_Data():

    def __init__(self, glob, impls):
        self.glob = deepcopy(glob)
        self.impls = impls

        self.sym = get_data_name(self.glob[0])
        self.vec_size = len(self.glob) * len(self.glob[0])
        assert self.vec_size % 2 == 0
        self.vec_size = int(self.vec_size / 2)

    def section(self):
        return {16: "sse4", 32: "avx2", 64: "evex512"}[self.vec_size]

    def add_offset(self):
        global G_offsets
        global G_nimpls

        assert len(self.impls) <= G_nimpls
        G_nimpls = len(self.impls)

        assert self.sym not in G_offsets

        G_offsets[self.sym] = len(G_offsets)

    def get_offsets(self):
        global G_offsets
        assert self.sym in G_offsets
        return "#define {}\t{}".format(self.sym,
                                       self.vec_size * G_offsets[self.sym])

    def get_defs(self):
        out = []
        out.append("\t/* Used By: {}.  */".format(", ".join(sorted(
            self.impls))))
        out.append("\tDATA_VEC(COMMON_DATA_NAME, {}, {})".format(
            self.sym, "0x" + self.glob[0]))
        return "\n".join(out)


def output(cds):
    offsets = "\n".join([cd.get_offsets() for cd in cds])
    defs = "\n".join([cd.get_defs() for cd in cds])

    out = []
    out.append(offsets)
    out.append("")
    out.append("\t.section .rodata.{}, \"a\"".format(cds[0].section()))
    out.append("\t.align\t{}".format(cds[0].vec_size))
    out.append("\t.globl\tCOMMON_DATA_NAME")
    out.append("")
    out.append("COMMON_DATA_NAME:")
    out.append("")
    out.append(defs)
    out.append("")
    out.append("\t.type\tCOMMON_DATA_NAME, @object")
    out.append("\t.size\tCOMMON_DATA_NAME, .-COMMON_DATA_NAME")
    return "\n".join(out)


def invert(glob):
    n1 = "f" * len(glob)
    n1 = int(n1, 16)
    glob = int(glob, 16)

    glob ^= n1

    glob = str(hex(glob))[len("0x"):]
    return glob


def cleanup_name(name):
    assert name.count("-") >= 1
    impl = name.split("-")[0]
    name = "-".join(name.split("-")[1:])

    ends = [".", ":"]
    og_len = len(name)
    while True:
        for end in ends:
            if name.endswith(end):
                name = name[:-len(end)]
        if len(name) == og_len:
            break
        og_len = len(name)
    return name, impl


def parse_explicit_data(line):
    line = remove_ws(line)
    piece_sz = None
    types = [[".long", 4]]
    for T in types:
        if line.startswith(T[0]):
            piece_sz = T[1]
            line = line[len(T[0]):]

    assert piece_sz is not None, "Unknown piece size"

    lc_cnt = line.count("//")
    bcs_cnt = line.count("/*")
    bce_cnt = line.count("*/")

    if lc_cnt == 1:
        assert bcs_cnt == 0
        assert bce_cnt == 0

        lc_pos = line.find("//")
        name = line[lc_pos + len("//"):]
        line = line[:lc_pos]
    else:
        assert lc_cnt == 0
        assert bcs_cnt == 1
        assert bce_cnt == 1

        bcs_pos = line.find("/*")
        bce_pos = line.find("*/")
        name = line[bcs_pos + len("/*"):bce_pos]
        line = line[:bcs_pos]

    name, impl = cleanup_name(name)

    pieces = line.split(",")

    glob = []
    base = None
    for piece in pieces:
        if piece.startswith("0x"):
            if base is None:
                base = 16
            assert base == 16
            piece = piece[len("0x"):]
        else:
            assert False
        assert is_int(piece, base)
        assert piece_sz * 2 == len(piece)
        glob.append(piece)
        assert piece == glob[0], "{} == {}".format(piece, glob[0])

    assert len(glob) % 2 == 0
    return glob, impl, name


def parse_macro_data(line):
    piece_sz = None
    types = [["float_vector", 4]]
    for T in types:
        if line.startswith(T[0]):
            piece_sz = T[1]
            line = line[len(T[0]):]

    line = line.split()

    assert len(line) == 2, line

    name, impl = cleanup_name(line[0])
    data = line[1]

    base = None
    if data.startswith("0x"):
        if base is None:
            base = 16
        assert base == 16
        data = data[len("0x"):]
    else:
        assert False
    assert is_int(data, base)

    assert len(data) == piece_sz * 2
    return data, impl, name


assert len(sys.argv) > 1

f = sys.argv[1]
vsz = None

globs = {}
gsz = None
for line in open(f):
    line = strs(line).lower()
    if line == "":
        continue
    print(line)
    glob = None
    impl = None
    name = None
    if line.startswith(".long"):
        glob, impl, name = parse_explicit_data(line)
        if gsz is None:
            gsz = len(glob)
        assert gsz == len(glob), "{} != {}\n\t{}".format(len(glob), gsz, glob)
    elif line.startswith("float_vector"):
        assert gsz is not None
        data, impl, name = parse_macro_data(line)
        glob = [data for i in range(0, gsz)]
    elif line.startswith(".align"):
        continue
    else:
        assert False, line

    assert glob is not None
    assert impl is not None
    assert name is not None

    if impl == "unused":
        continue

    globs.setdefault("".join(glob), []).append(
        (deepcopy(glob), deepcopy(impl), deepcopy(name)))

common = []
for glob in sorted(globs, reverse=True, key=lambda g: len(globs[g])):
    data_infos = globs[glob]
    impls = []
    for dinfo in data_infos:
        impls.append(dinfo[1])

    if len(impls) > 1:
        common.append(Common_Data(data_infos[0][0], impls))

    has_inverse = invert(glob) in globs
    if has_inverse:
        print("Look Into: {} -> {}".format(glob, ", ".join(impls)))

for cd in common:
    cd.add_offset()

print(output(common))

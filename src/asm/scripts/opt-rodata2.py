import os
import sys
import glob
from copy import deepcopy
from common_util import (is_int, strs, findnth, read_c_no_comments, remove_ws,
                         tern, dict_key)

assert len(sys.argv) > 1
S_files = glob.glob("{}/**/*dev*.S".format(sys.argv[1]), recursive=True)

functions = [
    "logf", "log10f", "log2f", "atanf", "atanhf", "tanhf", "tanf", "dev-tanhf"
]
rodata_files = {"dev-tanhf-rodata.S": ("tanhf", "sse4", "avx2")}
dedup_files = {
    "dev-common-sse4-rodata.S", "dev-common-avx2-rodata.S",
    "dev-common-evex512-rodata.S"
}

kDATA_VEC = 0
kDATA_BLOCK = 1

G_rodata_defs = [("float_block", kDATA_BLOCK), ("DATA_VEC", kDATA_VEC)]

G_all_data = {}
G_all_idata = {}


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


def make_data_def(dglob, repeat, sz, name=None):
    block = None
    if repeat:
        assert name is None
        name = get_data_name(dglob[0])
        return name, "DATA_VEC (COMMON_DATA_NAME, {}, 0x{})".format(
            name, dglob[0])
    elif sz == 4:
        block = "float_block"
    elif sz == 8:
        block = "double_block"
        assert False
    else:
        assert False

    assert False
    assert name is not None
    return name, "{} (COMMON_DATA_NAME, {}, 0x{}".format(
        block, name, ", 0x".join(dglob))


def output(vsize, tsize, isa):
    ddefs = G_all_data[vsize]

    offsets = []
    defs = []
    for dd in sorted(ddefs, key=lambda dd: len(ddefs[dd]), reverse=True):
        if len(ddefs[dd]) < 2:
            break

        name, out = make_data_def(ddefs[dd][0][0], ddefs[dd][0][1], tsize,
                                  ddefs[dd][0][3])

        offsets.append("#define {}\t{}".format(name, len(offsets) * vsize))
        defs.append("\t/* Used By: {}.  */".format(", ".join(
            [x[2] for x in ddefs[dd]])))
        defs.append("\t" + out)

    offsets = "\n".join(offsets)
    defs = "\n".join(defs)

    out = []
    out.append(offsets)
    out.append("")
    out.append("\t.section .rodata.{}, \"a\"".format(isa))
    out.append("\t.align\t{}".format(vsize))
    out.append("\t.globl\tCOMMON_DATA_NAME")
    out.append("")
    out.append("COMMON_DATA_NAME:")
    out.append("")
    out.append(defs)
    out.append("")
    out.append("\t.type\tCOMMON_DATA_NAME, @object")
    out.append("\t.size\tCOMMON_DATA_NAME, .-COMMON_DATA_NAME")
    return "\n".join(out)


def get_isa(fname):
    isas = {
        "sse4": [False, 16],
        "avx2": [False, 32],
        "avx512": [False, 64],
        "evex512": [False, 64]
    }

    for isa in isas:
        if isa in fname:
            isas[isa][0] = True

    f_isa = None
    for isa in isas:
        if isas[isa][0]:
            assert f_isa is None
            f_isa = isa

    assert f_isa is not None, fname
    ret_isa = f_isa
    if f_isa == "evex512":
        ret_isa = "avx512"
    return ret_isa, isas[f_isa][1]


def is_rodata_file(fname):
    return fname.split("/")[-1] in rodata_files


def is_dd_file(fname):
    return fname.split("/")[-1] in dedup_files


def rodata_file_get_isa(fname):
    assert is_rodata_file(fname)
    infos = rodata_files[fname.split("/")[-1]]

    isas = []
    for isa in infos[1:]:
        isas.append(get_isa(isa))

    assert len(isas) > 0
    isa = sorted(isas, reverse=True, key=lambda t: t[1])[0]
    return isa[0], isa[1], infos[0]


class Rodata():

    def __init__(self, lines, vsize, tsize):
        global G_rodata_defs
        line = "".join(lines)
        self.dtype = None
        assert line.endswith(")")

        for rodata_def_tpl in G_rodata_defs:
            if line.startswith(rodata_def_tpl[0]):
                assert self.dtype is None
                self.dtype = rodata_def_tpl[1]
        assert self.dtype is not None
        assert line.count("(") == 1
        assert line.count(")") == 1

        begin = line.find("(")
        end = line.find(")")

        assert begin < end
        assert begin > 0

        line = line[begin + 1:end]
        line = remove_ws(line)

        line = line.split(",")

        assert len(line) >= 3

        assert (self.dtype != kDATA_VEC) or len(line) == 3

        self.sect = None
        self.lbl = None
        self.dglobs = []
        self.repeats = False
        self.itoN = False
        self.vsize = vsize
        self.tsize = tsize
        self.has_ikey = False

        assert vsize % tsize == 0
        assert vsize > tsize

        self.sect = line[0]
        self.lbl = line[1]

        for data in line[2:]:
            assert data.startswith("0x")
            data = data[len("0x"):]
            assert len(data) == 2 * tsize
            assert is_int(data, 16)
            self.dglobs.append(data)

        if self.dtype == kDATA_VEC:
            assert len(self.dglobs) == 1
            self.repeats = True
            for i in range(int(self.vsize / self.tsize) - 1):
                self.dglobs.append(self.dglobs[0])

            assert len(self.dglobs) * self.tsize == self.vsize
            assert len("".join(self.dglobs)) == self.vsize * 2
        else:
            if len(self.dglobs) == 1:
                self.itoN = True
                self.repeats = True
                assert self.vsize == 64
                assert self.sect.endswith("UNALIGNED")
                while len(self.dglobs) * self.tsize < self.vsize:
                    self.dglobs.append(self.dglobs[0])
                assert len(self.dglobs) * self.tsize == self.vsize
            else:
                matches = True
                for dglob in self.dglobs:
                    matches &= dglob == self.dglobs[0]
                assert not matches

        assert len(self.dglobs) * self.tsize >= self.vsize

    def key(self):
        return "".join(self.dglobs)

    def ikey(self):
        k = self.key()
        assert is_int(k, 16)
        k = int(k, 16)

        inv = "f" * (self.vsize * 2)
        assert is_int(inv, 16)
        inv = int(inv, 16)

        k ^= inv

        k = str(hex(k))
        assert k.startswith("0x")
        k = k[len("0x"):]
        return k

    def check_ikey(self, impl):
        global G_all_data
        global G_all_idata
        self.has_ikey = self.ikey() in G_all_data[self.vsize]
        if self.has_ikey:
            G_all_idata.setdefault(self.vsize, {}).setdefault(self.ikey(),
                                                              []).append(impl)

    def add_data(self, impl):
        global G_all_data
        G_all_data.setdefault(self.vsize,
                              {}).setdefault(self.key(), []).append([
                                  self.dglobs, self.repeats, impl,
                                  tern(self.repeats, None, self.lbl)
                              ])


def parse_rodatas(lines, vsize, tsize):
    rodatas = {}
    lc = 0
    in_block = []
    skip = 0
    for line in lines:
        lc += 1
        line = strs(line)

        do_skip = True
        do_skip &= line.startswith("#")
        do_skip &= strs(line[len("#"):]).startswith("if")
        do_skip &= strs(strs(line[len("#"):])[len("if"):]) == "0"
        if do_skip:
            skip += 1

        skip_me = skip > 0
        if line.startswith("#") and strs(line[len("#"):]) == "endif":
            skip -= 1

        if skip_me:
            continue
        if len(in_block) != 0:
            in_block.append(line)
            assert "(" not in line
            if line.endswith(")"):
                rodatas[lc - 1] = Rodata(deepcopy(in_block), vsize, tsize)
                in_block = []
                continue
        for rodata_def_tpl in G_rodata_defs:
            rodata_def = rodata_def_tpl[0]
            if line.startswith(rodata_def):
                line = strs(line)
                if line.endswith(")"):
                    rodatas[lc - 1] = Rodata(deepcopy([line]), vsize, tsize)
                else:
                    assert rodata_def == "float_block"
                    assert len(in_block) == 0
                    in_block = [line]
    assert len(in_block) == 0
    return rodatas


class FInfo():

    def __init__(self, fname):
        self.tsize = None
        assert fname.count("/float/") + fname.count("/double/") == 1
        if fname.count("/float/") == 1:
            self.tsize = 4
        if fname.count("/double/") == 1:
            assert self.tsize is None
            self.tsize = 8
        assert self.tsize is not None

        self.fname = fname
        self.isa = None
        self.vsize = None
        self.impl = None

        if is_rodata_file(self.fname):
            self.isa, self.vsize, self.impl = rodata_file_get_isa(self.fname)
        else:
            self.isa, self.vsize = get_isa(self.fname)
            if not is_dd_file(self.fname):
                self.impl = fname.split("/")[-1].split("-")[0]


class DDFile():

    def __init__(self, fname):
        self.fname = fname
        assert os.path.isfile(fname)

        self.content = []

        finfo = FInfo(fname)
        self.isa = finfo.isa
        self.vsize = finfo.vsize
        self.tsize = finfo.tsize
        self.impl = finfo.impl

        self.rodatas = []

        impl_maps = {}
        lc = 0

        lines = []
        for line in open(fname):
            lines.append(line)
            line = strs(line)
            lc += 1
            if line == "":
                continue
            if line.startswith("/*"):
                assert line.endswith(".  */")
                assert "Used By:" in line
                line = line[line.find(":") + 1:line.find(".")]
                line = remove_ws(line)
                impl_maps[lc - 1] = line.split(",")

        ro_map = parse_rodatas(lines, self.vsize, self.tsize)

        for lc in impl_maps:
            assert lc + 1 in ro_map
            for impl in impl_maps[lc]:
                self.rodatas.append((impl, ro_map[lc + 1]))

    def collect_rodatas(self):
        assert len(self.rodatas) > 0

    def add_data(self):
        for rodata in self.rodatas:
            rodata[1].add_data(rodata[0])

    def check_ikeys(self):
        for rodata in self.rodatas:
            rodata[1].check_ikey(rodata[0])


class File():

    def __init__(self, fname):
        self.fname = fname
        assert os.path.isfile(fname)

        self.content = []
        self.rodatas = []

        finfo = FInfo(fname)
        self.isa = finfo.isa
        self.vsize = finfo.vsize
        self.tsize = finfo.tsize
        self.impl = finfo.impl

        include = None
        for line in open(fname):
            line = strs(line)
            good = True
            good &= line.startswith("#")
            good &= strs(line[len("#"):]).startswith("include")
            good &= strs(strs(
                line[len("#"):])[len("include"):]).startswith("\"")
            good &= line.endswith("\"")
            if good:
                assert include is None
                include = line

        content_f = None
        if is_rodata_file(self.fname):
            content_f = self.fname
        else:
            assert include is not None
            assert not is_rodata_file(self.fname)
            assert include.count("\"") == 2

            begin_f = include.find("\"")
            end_f = findnth(include, "\"", 1)

            assert begin_f > 0 and end_f > 0
            assert end_f > begin_f
            content_f = line[begin_f + 1:end_f]

        assert self.impl is not None

        assert content_f is not None
        assert os.path.isfile(content_f)

        fcontent = read_c_no_comments(content_f)
        assert fcontent is not None
        self.content = fcontent.split("\n")

    def collect_rodatas(self):
        global G_rodata_defs
        ro_map = parse_rodatas(self.content, self.vsize, self.tsize)
        for lc in ro_map:
            self.rodatas.append(ro_map[lc])

    def add_data(self):
        for rodata in self.rodatas:
            rodata.add_data(self.impl)

    def check_ikeys(self):
        for rodata in self.rodatas:
            rodata.check_ikey(self.impl)


uses = []
for sf in S_files:
    if "/core/" in sf:
        continue
    if "wrapped" in sf:
        continue
    if "wrapper" in sf:
        continue
    if "-avx-" in sf:
        continue
    if "dev" not in sf:
        continue

    hit = False
    for func in functions:
        key = "{}-".format(func)
        pieces = sf.split("/")
        for piece in pieces:
            if piece.startswith(key):
                uses.append(File(sf))
                hit = True
    if is_dd_file(sf):
        assert not hit
        uses.append(DDFile(sf))

for use in uses:
    use.collect_rodatas()
for use in uses:
    use.add_data()
for use in uses:
    use.check_ikeys()

for vsize in G_all_data:
    print("{}".format(vsize))
    for dglob in G_all_data[vsize]:
        if len(G_all_data[vsize][dglob]) > 1:
            print("\n{} -> {}".format(
                dglob, ", ".join([x[2] for x in G_all_data[vsize][dglob]])))
uniques = {}
for use in uses:
    tup = (use.vsize, use.tsize, use.isa)
    uniques.setdefault(dict_key(tup), tup)

for tup in sorted(uniques, key=lambda t: uniques[t][0] * 1000 + uniques[t][1]):
    print(output(uniques[tup][0], uniques[tup][1], uniques[tup][2]))

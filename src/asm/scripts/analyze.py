import os
import sys
import copy
from common_util import (strs, is_int, vprint, set_verbosity, tern,
                         push_verbosity, pop_verbosity, vvprint, vvvprint)
from enum import Enum, IntEnum

G_all_insns = {}
G_all_regs = {}
G_reg_map = None
G_isa_max = None
Gk_agu_ops = {"+", "-"}
G_ruses = None
G_ret_reg = "mm0"

class ISAS(IntEnum):
    GENERIC = 0,
    SSE = 1,
    SSE2 = 2,
    SSE4 = 3,
    SSE41 = 4,
    SSE42 = 5,
    AVX = 6,
    AVX2 = 7
    AVX512 = 8,
    ANY = 9,


def isa_str_to_enum(s):
    s = s.lower()
    if s == "generic":
        s = ISAS.GENERIC
    elif s == "sse2":
        s = ISAS.SSE2
    elif s == "sse4":
        s = ISAS.SSE4
    elif s == "sse41":
        s = ISAS.SSE41
    elif s == "sse42":
        s = ISAS.SSE42
    elif s == "avx":
        s = ISAS.AVX
    elif s == "avx2":
        s = ISAS.AVX2
    elif s == "avx512":
        s = ISAS.AVX512
    elif s == "any":
        s = ISAS.ANY
    else:
        s = None
    return s


class Op_Types(Enum):
    VEC = 0,
    GPR = 1,
    AGEN = 2,
    IMM = 3,
    MASK = 4,
    LABEL = 5,
    IP = 6,
    FLAG = 7,


class Op_Targets(Enum):
    DST = 0,
    SRC = 1,
    SRCDST = 2,


class RUses():

    def __init__(self, insns, all_deps=None):
        self.uses = {}
        if all_deps is None:
            all_deps = [i for i in range(0, len(insns))]
        for iid in all_deps:
            insn = insns[iid]
            for use in insn.uses:
                self.uses.setdefault(use, [set(), set()])
                self.uses[use][0].add(insn.iid)
        for use in self.uses:
            self.uses[use][0] = sorted(self.uses[use][0])

    def init_tracker(self):
        for use in self.uses:
            self.uses[use][1] = copy.deepcopy(self.uses[use][0])

    def use_insn(self, insn):
        completed = set()
        for use in insn.uses:
            assert use in self.uses
            assert insn.iid in self.uses[use][0]
            assert insn.iid in self.uses[use][1]
            self.uses[use][1].remove(insn.iid)
            if len(self.uses[use][1]) == 0:
                completed.add(use)
        return sorted(completed)

    def query_insn(self, insn):
        completed = set()
        for use in insn.uses:
            assert use in self.uses
            assert insn.iid in self.uses[use][0]
            if insn.iid in self.uses[use][1] and len(self.uses[use][1]) == 1:
                completed.add(use)
        return sorted(completed)


def str_to_type(op_str):
    op_types_str = {
        "vec": Op_Types.VEC,
        "gpr": Op_Types.GPR,
        "flag": Op_Types.FLAG,
        "agen": Op_Types.AGEN,
        "imm": Op_Types.IMM,
        "mask": Op_Types.MASK,
        "label": Op_Types.LABEL,
        "ip": Op_Types.IP,
    }
    assert op_str in op_types_str
    return op_types_str[op_str]


def get_op_info(op_def):
    assert op_def.count("-") in {1, 2, 3}
    assert op_def.count("(") == 0
    assert op_def.count(")") == 0
    assert op_def.count("|") == 0

    op_def = op_def.split("-")
    op_type = op_def[0]
    op_target = op_def[1]
    op_size = None
    op_fixed = None
    op_implicit = False
    op_targets_str = {
        "src": Op_Targets.SRC,
        "dst": Op_Targets.DST,
        "srcdst": Op_Targets.SRCDST,
    }

    op_type = str_to_type(op_type)
    assert op_target in op_targets_str

    if op_type in {Op_Types.AGEN, Op_Types.IMM, Op_Types.LABEL}:
        assert len(op_def) == 2
    else:
        assert len(op_def) in {3, 4}
        op_size = op_def[2]

        if is_int(op_size):
            op_size = int(op_size)
            assert op_size in {4, 8, 16}
        else:
            global G_all_regs
            assert op_size in G_all_regs
            op_fixed = op_size
            op_size = None

            assert G_all_regs[op_fixed].rtype in {
                Op_Types.VEC, Op_Types.GPR, Op_Types.FLAG
            }
        if len(op_def) > 3:
            for info in op_def[3:]:
                if info == "implicit":
                    op_implicit = True

    return op_type, op_targets_str[op_target], op_size, op_fixed, op_implicit


class Sub_Op_Info():

    def __init__(self, op_def):
        op_type, op_target, op_size, op_fixed, op_implicit = get_op_info(
            op_def)

        self.op_type = op_type
        self.op_target = op_target
        self.op_size = op_size
        self.op_fixed = op_fixed
        self.op_implicit = op_implicit

        if self.op_implicit:
            assert self.op_fixed is not None

    def is_write(self):
        return self.op_target in {Op_Targets.DST, Op_Targets.SRCDST}

    def is_write_only(self):
        return self.op_target in {Op_Targets.DST}

    def is_read(self):
        return self.op_target in {Op_Targets.SRC, Op_Targets.SRCDST}

    def is_read_only(self):
        return self.op_target in {Op_Targets.SRC}

    def is_rw(self):
        return self.op_target == Op_Targets.SRCDST

    def is_fixed(self):
        return self.op_fixed is not None

    def get_fixed_reg(self):
        global G_all_regs
        assert self.is_fixed()
        assert self.op_fixed in G_all_regs
        return G_all_regs[self.op_fixed]

    def reg_matches(self, reg_info):
        if not isinstance(reg_info, Reg_Info):
            return False
        if self.op_type != reg_info.rtype:
            return False
        if self.is_fixed():
            return self.op_fixed == reg_info.name
        else:
            assert self.op_size is not None
            return self.op_size == reg_info.size

    def size(self):
        if not self.is_reg():
            assert False
        else:
            if self.is_fixed():
                return self.get_fixed_reg().size
            else:
                return self.op_size

    def is_agen(self):
        return self.op_type == Op_Types.AGEN

    def is_reg(self):
        return self.is_vec() or self.is_gpr() or self.is_mask(
        ) or self.is_flag() or self.is_ip()

    def is_vec(self):
        return self.op_type == Op_Types.VEC

    def is_gpr(self):
        return self.op_type == Op_Types.GPR

    def is_mask(self):
        return self.op_type == Op_Types.MASK

    def is_flag(self):
        return self.op_type == Op_Types.FLAG

    def is_ip(self):
        return self.op_type == Op_Types.IP

    def is_imm(self):
        return self.op_type == Op_Types.IMM

    def is_label(self):
        return self.op_type == Op_Types.LABEL

    def imm_matches(self, T):
        return self.op_type == Op_Types.IMM and self.op_type == T

    def label_matches(self, T):
        return self.op_type == Op_Types.LABEL and self.op_type == T

    def agen_matches(self, T):
        return self.op_type == Op_Types.AGEN and self.op_type == T

    def matches(self, info):
        return self.reg_matches(info) or self.imm_matches(
            info) or self.label_matches(info) or self.agen_matches(info)


class Op_Possibilities():

    def __init__(self, op_info):
        self.mustbe = op_info.mustbe_results()
        self.maybe = op_info.maybe_results()

        assert len(self.mustbe) <= 1
        if len(self.mustbe) != 0:
            assert len(self.maybe) == 0
        else:
            assert len(self.maybe) >= 1, self.maybe

    def is_must(self):
        return len(self.mustbe) != 0

    def check_mustbe(self, li):
        for T in li:
            if T in self.mustbe:
                return True
        return False

    def check_maybe(self, li):
        for T in li:
            if T in self.maybe:
                return True
        return False

    def check(self, li):
        if self.is_must():
            return self.check_mustbe(li)
        return self.check_maybe(li)

    def reg_valid(self):
        return self.check([
            Op_Types.VEC, Op_Types.GPR, Op_Types.MASK, Op_Types.IP,
            Op_Types.FLAG
        ])

    def imm_valid(self):
        return self.check([Op_Types.IMM])

    def agen_valid(self):
        return self.check([Op_Types.AGEN])

    def label_valid(self):
        return self.check([Op_Types.LABEL])


class Op_Info():

    def __init__(self, op_defs):
        assert op_defs[0] == "("
        assert op_defs[-1] == ")"
        op_defs = op_defs[1:-1]

        op_defs = op_defs.split("|")

        self.sub_op_infos = []
        for op_def in op_defs:
            self.sub_op_infos.append(Sub_Op_Info(op_def))
        has_fixed = None
        fixed_info = None
        for sub_op in self.sub_op_infos:
            if has_fixed is None:
                has_fixed = sub_op.is_fixed()
                fixed_info = sub_op.op_fixed
            assert has_fixed == sub_op.is_fixed()
            assert fixed_info == sub_op.op_fixed

        assert has_fixed != (fixed_info is None)
        self.fixed = fixed_info

        assert len(self.sub_op_infos) in {1, 2, 3}

        assert self.sub_op_infos[0].op_target == self.sub_op_infos[
            -1].op_target

        assert (self.sub_op_infos[0].op_type != self.sub_op_infos[-1].op_type
                ) or (self.sub_op_infos[0].op_size !=
                      self.sub_op_infos[-1].op_size) or (len(self.sub_op_infos)
                                                         == 1)

    def is_fixed(self):
        return self.fixed is not None

    def find_match(self, op):
        for sub_op in self.sub_op_infos:
            if sub_op.matches(op):
                return sub_op
        return None

    def agen_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_agen():
                return len(self.sub_op_infos)
        return 0

    def reg_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_reg():
                return len(self.sub_op_infos)
        return 0

    def vec_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_vec():
                return len(self.sub_op_infos)
        return 0

    def gpr_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_gpr():
                return len(self.sub_op_infos)
        return 0

    def mask_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_mask():
                return len(self.sub_op_infos)
        return 0

    def flag_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_flag():
                return len(self.sub_op_infos)
        return 0

    def ip_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_ip():
                return len(self.sub_op_infos)
        return 0

    def imm_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_imm():
                return len(self.sub_op_infos)
        return 0

    def label_info(self):
        for sub_op in self.sub_op_infos:
            if sub_op.is_label():
                return len(self.sub_op_infos)
        return 0

    def mustbe_agen(self):
        return self.agen_info() == 1

    def mustbe_reg(self):
        return self.reg_info() == 1

    def mustbe_vec(self):
        return self.vec_info() == 1

    def mustbe_gpr(self):
        return self.gpr_info() == 1

    def mustbe_mask(self):
        return self.mask_info() == 1

    def mustbe_flag(self):
        return self.flag_info() == 1

    def mustbe_ip(self):
        return self.ip_info() == 1

    def mustbe_imm(self):
        return self.imm_info() == 1

    def mustbe_label(self):
        return self.label_info() == 1

    def maybe_agen(self):
        return self.agen_info() > 1

    def maybe_reg(self):
        return self.reg_info() > 1

    def maybe_vec(self):
        return self.vec_info() > 1

    def maybe_gpr(self):
        return self.gpr_info() > 1

    def maybe_mask(self):
        return self.mask_info() > 1

    def maybe_flag(self):
        return self.flag_info() > 1

    def maybe_ip(self):
        return self.ip_info() > 1

    def maybe_imm(self):
        return self.imm_info() > 1

    def maybe_label(self):
        return self.label_info() > 1

    def mustbe_results(self):
        res = set()
        if self.mustbe_agen():
            assert not self.mustbe_reg()
            res.add(Op_Types.AGEN)
        if self.mustbe_vec():
            assert self.mustbe_reg()
            res.add(Op_Types.VEC)
        if self.mustbe_gpr():
            assert self.mustbe_reg()
            res.add(Op_Types.GPR)
        if self.mustbe_mask():
            assert self.mustbe_reg()
            res.add(Op_Types.MASK)
        if self.mustbe_flag():
            assert self.mustbe_reg()
            res.add(Op_Types.FLAG)
        if self.mustbe_ip():
            assert self.mustbe_reg()
            res.add(Op_Types.IP)
        if self.mustbe_imm():
            assert not self.mustbe_reg()
            res.add(Op_Types.IMM)
        if self.mustbe_label():
            assert not self.mustbe_reg()
            res.add(Op_Types.LABEL)
        return res

    def maybe_results(self):
        res = set()
        if self.maybe_agen():
            res.add(Op_Types.AGEN)
        if self.maybe_vec():
            res.add(Op_Types.VEC)
        if self.maybe_gpr():
            assert self.maybe_reg()
            res.add(Op_Types.GPR)
        if self.maybe_mask():
            assert self.maybe_reg()
            res.add(Op_Types.MASK)
        if self.maybe_flag():
            assert self.maybe_reg()
            res.add(Op_Types.FLAG)
        if self.maybe_ip():
            assert self.maybe_reg()
            res.add(Op_Types.IP)
        if self.maybe_imm():
            res.add(Op_Types.IMM)
        if self.maybe_label():
            res.add(Op_Types.LABEL)
        return res

    def possiblities(self):
        return Op_Possibilities(self)


class Insn_Info():

    def __init__(self, idef_ln):
        idef_ln = idef_ln.replace(" ", "")
        idef_ln = idef_ln.lower()
        assert idef_ln.startswith("idef:")
        idef_ln = idef_ln[len("idef:"):]

        idef_ln = idef_ln.split(",")
        assert len(idef_ln) >= 1

        self.name = idef_ln[0]
        self.nop = False
        self.commute = False
        self.commute_custom = False
        assert "|" in self.name
        name_info = self.name.split("|")
        self.name = name_info[0]
        self.isa = None
        self.needed = False
        for ni in name_info[1:]:
            if ni == "nop":
                self.nop = True
            elif ni == "needed":
                self.needed = True
            elif isa_str_to_enum(ni) is not None:
                self.isa = isa_str_to_enum(ni)
            elif ni.startswith("comm"):
                self.commute = True
                if ni == "comm-spec":
                    self.commute_custom = True
            elif ni == "non-comm":
                pass
            else:
                assert False
        assert self.isa is not None

        self.op_infos = []
        for op_def in idef_ln[1:]:
            self.op_infos.append(Op_Info(op_def))

    def key(self):
        return self.name

    def is_nop(self):
        return self.nop

    def num_ops(self):
        return len(self.op_infos)


class Reg_Cnt():

    def __init__(self):
        self.rcnts = {
            Op_Types.VEC: 0,
            Op_Types.GPR: 0,
            Op_Types.MASK: 0,
            Op_Types.IP: 0,
            Op_Types.FLAG: 0,
        }

    def get_rcnt(self, name):
        global G_all_regs
        assert name in G_all_regs
        reg = G_all_regs[name]
        assert reg.rtype in self.rcnts
        return self.rcnts[reg.rtype]

    def incr_rcnt(self, name):
        global G_all_regs
        assert name in G_all_regs
        reg = G_all_regs[name]
        assert reg.rtype in self.rcnts
        cnt = self.rcnts[reg.rtype] + 1
        self.rcnts[reg.rtype] = cnt
        return cnt


class Reg_Proto():

    def __init__(self, reg_info, ssa_name, fixed=None):
        self.rtype = reg_info.rtype
        self.rsize = reg_info.size
        self.fixed = fixed
        self.ssa_name = ssa_name
        self.color = None
        self.alloc = None

    def name(self):
        assert self.ssa_name is not None
        out = self.ssa_name
        extra = []
        if self.fixed is not None:
            extra = [self.fixed]
        if self.color is not None:
            extra += [str(self.color)]
        if len(extra) > 0:
            out = "{}({})".format(out, "/".join(extra))
        return out


class Reg_Map():

    def __init__(self):
        self.reg_cnt = Reg_Cnt()
        self.arb_cnt = {}
        self.rmap = {}

    def new_reg_cnt(self, reg_info):
        return self.reg_cnt.incr_rcnt(reg_info.parent)

    def new_reg_proto(self, reg_info, fixed=None):
        global G_all_regs
        assert reg_info.name in G_all_regs
        assert reg_info.parent in G_all_regs
        assert (fixed is None) or fixed in G_all_regs
        self.rmap[reg_info.parent] = Reg_Proto(reg_info,
                                               reg_info.new_ssa_name(), fixed)
        return self.rmap[reg_info.parent]

    def new_spill_reg_proto(self, reg_info, name):
        global G_all_regs
        assert reg_info.name in G_all_regs
        assert reg_info.parent in G_all_regs
        assert name is not None
        assert name not in G_all_regs
        self.rmap[name] = Reg_Proto(reg_info, reg_info.new_ssa_name())
        return self.rmap[name]

    def has_reg_proto(self, reg_info):
        global G_all_regs
        assert reg_info.name in G_all_regs
        assert reg_info.parent in G_all_regs

        return reg_info.parent in self.rmap

    def get_reg_proto(self, reg_info, fixed=None):
        global G_all_regs
        assert reg_info.name in G_all_regs
        assert reg_info.parent in G_all_regs
        assert (fixed is None) or (fixed in G_all_regs)
        if self.has_reg_proto(reg_info):
            return copy.deepcopy(self.rmap[reg_info.parent])
        else:
            return None

    def get_spill_reg_proto(self, reg_info, name):
        global G_all_regs
        assert reg_info.name in G_all_regs
        assert reg_info.parent in G_all_regs
        assert name not in G_all_regs

        if name in self.rmap:
            return copy.deepcopy(self.rmap[name])
        else:
            return None

    def get_arb_rename(self, name):
        if name in self.arb_cnt:
            return "{}-{}".format(name, self.arb_cnt[name])
        return None

    def has_arb_rename(self, name):
        return name in self.arb_cnt

    def new_arb_rename(self, name):
        cnt = 0
        if name in self.arb_cnt:
            cnt = self.arb_cnt[name] + 1
        self.arb_cnt[name] = cnt
        return "{}-{}".format(name, cnt)


def ssa_name_to_type_name(ssa_name):
    assert ssa_name.count("-") == 1
    tname = ssa_name.split("-")[0]
    assert len(tname) in {2, 3, 4}
    return tname


class Reg_Info():

    def __init__(self, rdef_ln):
        rdef_ln = rdef_ln.replace(" ", "")
        rdef_ln = rdef_ln.lower()
        assert rdef_ln.startswith("rdef:")
        rdef_ln = rdef_ln[len("rdef:"):]

        rdef_ln = rdef_ln.split(",")
        assert len(rdef_ln) >= 5

        self.name = rdef_ln[0]
        self.size = rdef_ln[1]
        self.rtype = rdef_ln[2]
        self.isa = rdef_ln[3]
        self.parent = rdef_ln[4]
        self.may_rename = True
        self.avoid = False
        self.pseudo = False
        self.prio = 10
        self.is_return_reg = False
        for extra in rdef_ln[5:]:
            if extra == "return":
                self.is_return_reg = True
            if extra == "norename":
                self.may_rename = False
            if extra == "avoid":
                self.avoid = True
            if extra.startswith("prio-"):
                assert is_int(extra[len("prio-"):])
                self.prio = int(extra[len("prio-"):])
            if extra == "pseudo":
                self.pseudo = True

        self.siblings = []

        assert is_int(self.size)
        self.size = int(self.size)
        assert self.size in {1, 2, 4, 8, 16, 32, 64} or (self.size == 0
                                                         and self.pseudo)

        self.isa = isa_str_to_enum(self.isa)
        assert self.isa is not None

        self.rtype = str_to_type(self.rtype)

    def is_vec(self):
        return self.rtype == Op_Types.VEC

    def is_sp(self):
        return self.name == "rsp"

    def x86_name(self):
        if self.pseudo:
            return ""
        name = self.name
        if name.endswith("mma"):
            assert name in {"xmma", "ymma", "zmma"}
            name = name[:-1] + name[-1].upper()
        return "%" + name

    def new_ssa_name(self):
        global G_reg_map
        return "{}-{}".format(self.type_str(), G_reg_map.new_reg_cnt(self))

    def type_str(self):
        types_to_str = {
            Op_Types.VEC: "vec",
            Op_Types.GPR: "gpr",
            Op_Types.MASK: "mask",
            Op_Types.IP: "ip",
            Op_Types.FLAG: "flag",
        }
        assert self.rtype in types_to_str, self.rtype
        return types_to_str[self.rtype]

    def is_partial(self):
        return self.parent != self.name

    def get_parent(self):
        global G_all_regs
        assert self.parent in G_all_regs
        preg = G_all_regs[self.parent]
        assert preg.size in {0, 8, 64}
        assert preg.size > 0 or self.pseudo

        assert preg.rtype == self.rtype
        assert (preg.name == self.name) != (self.is_partial())
        return preg

    def key(self):
        return self.name


class StackSpot():

    def __init__(self, size):
        self.size = size
        self.offset = None


class Allocation():

    def __init__(self, reg_or_stack):
        assert isinstance(reg_or_stack, Reg_Info) or isinstance(
            reg_or_stack, StackSpot)
        self.alloc = reg_or_stack

    def is_vec_return(self):
        if isinstance(self.alloc, Reg_Info):
            if self.alloc.is_return_reg and self.alloc.is_vec():
                return True
        return False

    def x86_name(self, opt):
        if isinstance(self.alloc, Reg_Info):
            global G_all_regs
            for sreg in self.alloc.siblings:
                if G_all_regs[sreg].size == opt.rinfo.size:
                    return G_all_regs[sreg].x86_name()
            assert False
        else:
            return "{}(%rsp)".format(self.alloc.offset)


def setup(f):
    global G_all_regs
    global G_all_insns
    global G_reg_map
    assert os.path.isfile(f)
    insns = []
    regs = []
    for i in range(0, 2):
        for line in open(f):
            line = strs(line)
            if line.startswith("idef:"):
                if i == 1:
                    insns.append(Insn_Info(line))
            elif line.startswith("rdef:"):
                if i == 0:
                    regs.append(Reg_Info(line))
            elif line == "":
                continue
            else:
                assert False

        if i == 0:
            for reg in regs:
                assert reg.key() not in G_all_regs, reg.key()
                G_all_regs[reg.key()] = reg
            for reg in regs:
                for reg2 in regs:
                    if reg.get_parent() == reg2.get_parent():
                        reg.siblings.append(reg2.name)

        else:
            for insn in insns:
                assert insn.key() not in G_all_insns
                G_all_insns[insn.key()] = insn

    G_reg_map = Reg_Map()


class Reg():

    def __init__(self, rname, rinfo, from_spill=False, spill_rnamein=None):
        self.rname = rname
        self.rinfo = rinfo
        self.proto_in = None
        self.proto_out = None
        self.from_spill = from_spill
        self.spill_rnamein = spill_rnamein

    def uses(self):
        return {self.proto_in.ssa_name}

    def defs(self):
        return {self.proto_out.ssa_name}

    def T(self):
        return self.rinfo.rtype

    def complete(self, op_info=None):
        global G_reg_map

        assert (self.rname != self.rinfo.name) == self.from_spill

        if self.from_spill:
            assert op_info is not None
            assert op_info.op_type == self.T()

            if op_info.is_read() and op_info.is_write():
                assert self.spill_rnamein is not None
                self.proto_in = G_reg_map.get_spill_reg_proto(
                    self.rinfo, self.spill_rnamein)
                if self.proto_in is None:
                    self.proto_in = G_reg_map.new_spill_reg_proto(
                        self.rinfo, self.spill_rnamein)
                self.proto_out = G_reg_map.new_spill_reg_proto(
                    self.rinfo, self.rname)
                assert self.proto_in is not None
                assert self.proto_out is not None

            elif op_info.is_read():
                assert op_info.is_read_only()
                assert self.spill_rnamein is None
                self.proto_in = G_reg_map.get_spill_reg_proto(
                    self.rinfo, self.rname)
                if self.proto_in is None:
                    self.proto_in = G_reg_map.new_spill_reg_proto(
                        self.rinfo, self.rname)
                assert self.proto_in is not None
            else:
                assert op_info.is_write_only()
                assert self.spill_rnamein is None
                self.proto_out = G_reg_map.new_spill_reg_proto(
                    self.rinfo, self.rname)
                assert self.proto_out is not None
        else:
            assert (op_info is None) or (op_info.op_type == self.T())
            if (op_info is None) or op_info.is_read():
                self.proto_in = G_reg_map.get_reg_proto(self.rinfo)
                if self.proto_in is None:
                    self.proto_in = G_reg_map.new_reg_proto(
                        self.rinfo, self.rname)
            if (op_info is not None) and op_info.is_write():
                self.proto_out = G_reg_map.new_reg_proto(self.rinfo)

    def name_in(self):
        assert self.proto_in.name() is not None
        out = "%" + self.proto_in.name()
        if self.from_spill and False:
            name = self.rname
            if self.spill_rnamein is not None:
                name = self.spill_rnamein
            out += "/{}".format(name)
        return out

    def name_out(self):
        assert self.proto_out.name() is not None
        out = "%" + self.proto_out.name()
        if self.from_spill and False:
            out += "/{}".format(self.rname)
        return out

    def x86_name(self):
        if self.proto_in is not None:
            return self.proto_in.alloc.x86_name(self)
        else:
            return self.proto_out.alloc.x86_name(self)


class Imm():

    def __init__(self, val):
        self.val = val

    def T(self):
        return Op_Types.IMM

    def complete(self, op_info):
        assert op_info.op_type == self.T()
        base = 10
        if self.val.startswith("0x"):
            base = 16
        assert is_int(self.val, base)
        self.val = int(self.val, base)

    def uses(self):
        return set()

    def defs(self):
        return set()

    def name_out(self):
        assert False

    def name_in(self):
        return "$" + str(hex(self.val))

    def x86_name(self):
        return self.name_in()


class Label():

    def __init__(self, lbl):
        self.lbl = lbl

    def T(self):
        return Op_Types.LABEL

    def uses(self):
        return set()

    def defs(self):
        return set()

    def complete(self, op_info):
        assert op_info.op_type == self.T()

    def name_out(self):
        assert False

    def name_in(self):
        return self.lbl

    def x86_name(self):
        return self.name_in()


class Agen():

    def __init__(self, offset, base, index, scale):
        self.offset = offset
        self.base = base
        self.index = index
        self.scale = scale

        self.name_out_ = None
        self.name_in_ = None

    def name_out(self):
        assert self.name_out_ is not None
        return self.name_out_

    def name_in(self):
        assert self.name_in_ is not None
        return self.name_in_

    def uses(self):
        assert self.base is not None

        base_uses = self.base.uses()
        index_uses = set()

        assert len(base_uses) == 1

        if self.index is not None:
            index_uses |= self.index.uses()
            assert len(index_uses) == 1

        return base_uses | index_uses

    def x86_name(self):
        components = []
        components.append(self.base.x86_name())
        if self.index is not None:
            components.append(self.index.x86_name())
        if self.scale is not None:
            components.append(str(self.scale))
        return "{}({})".format(self.offset, ",".join(components))

    def defs(self):
        return []

    def T(self):
        return Op_Types.AGEN

    def complete(self, op_info):
        global G_all_regs
        assert self.base is not None
        assert self.base in G_all_regs
        base_reg = G_all_regs[self.base]
        self.base = Reg(self.base, base_reg)
        self.base.complete()
        assert base_reg.rtype in {Op_Types.GPR, Op_Types.IP}

        if self.index is not None:
            assert self.index in G_all_regs
            index_reg = G_all_regs[self.index]
            self.index = Reg(self.index, index_reg)
            assert index_reg.rtype in {Op_Types.GPR, Op_Types.IP}
            self.index.complete()

        if self.scale is not None:
            assert self.scale in {"1", "2", "4", "8"}
            self.scale = int(self.scale)

        index_s = "%riz"
        scale_s = "1"
        if self.index is not None:
            index_s = self.index.name_in()
        if self.scale is not None:
            scale_s = str(self.scale)

        assert self.offset is not None
        if self.offset == "":
            self.offset = "0"

        name = "{}({},{},{})".format(self.offset, self.base.name_in(), index_s,
                                     scale_s)
        if op_info.is_read():
            self.name_in_ = G_reg_map.get_arb_rename(name)
            if self.name_in_ is None:
                self.name_in_ = G_reg_map.new_arb_rename(name)
        if op_info.is_write():
            self.name_out_ = G_reg_map.new_arb_rename(name)


class Op():

    def __init__(self, opt, op_info):
        self.opt = opt
        self.op_info = op_info

    def complete(self):
        assert self.op_info is not None
        self.opt.complete(self.op_info)


def build_op(op_info, pieces, i):
    global G_all_regs
    opp = op_info.possiblities()
    must_info = None
    opt_info = None
    opt = None
    end = None
    if pieces[i][0] == "%":
        vprint("Handling Register")
        assert opp.reg_valid()
        name = pieces[i][1:].lower()
        assert name in G_all_regs
        opt_info = G_all_regs[name]
        opt = Reg(name, opt_info)
        end = i + 1

    elif pieces[i][0] == "$":
        vprint("Handling IMM")
        assert opp.imm_valid()
        opt = Imm(pieces[i][1:])
        opt_info = opt.T()
        end = i + 1
    elif pieces[i].lower().startswith("l(") and pieces[i].endswith(")"):
        vprint("Handling Label")
        assert opp.label_valid()
        opt = Label(pieces[i])
        opt_info = opt.T()
        end = i + 1
    else:
        vprint("Handling Agen")
        assert opp.agen_valid(), pieces
        op_cnt = 0
        cp_cnt = 0
        reg_start = None

        begin = i
        for j in range(i, len(pieces)):
            op_cnt += pieces[j].count("(")
            cp_cnt += pieces[j].count(")")

            if "(%" in pieces[j]:
                reg_start = j

            if pieces[j].endswith(")") and (reg_start is not None) and (
                    op_cnt == cp_cnt) and (op_cnt != 0):
                end = j
                break

        assert end is not None

        offset_pieces = pieces[begin:reg_start]
        offset_last_piece = ""

        assert pieces[reg_start].count("(%") == 1
        offset_last_piece = pieces[reg_start][:pieces[reg_start].find("(%")]

        if offset_last_piece != "":
            offset_pieces.append(offset_last_piece)

        offset = None
        base = None
        index = None
        scale = None
        offset = " ".join(offset_pieces)

        base = pieces[reg_start][pieces[reg_start].find("(%") + 2:].lower()

        assert not offset.endswith("(")
        assert not base.startswith("(%")

        if reg_start != end:
            scale_start = reg_start + 1
            if pieces[reg_start + 1].startswith("%"):
                index = pieces[reg_start + 1][1:].lower()
                scale_start += 1
            if scale_start <= end:
                scale = pieces[scale_start].lower()

        end += 1
        if scale is not None:
            assert index is not None

        if scale is None and index is None:
            assert base.endswith(")")
            base = base[:-1]
        else:
            assert not base.endswith(")"), pieces
            if scale is None:
                assert index.endswith(")")
                index = index[:-1]
            else:
                assert scale.endswith(")")
                scale = scale[:-1]

        assert scale in {None, "1", "2", "4", "8"}, scale

        assert base in G_all_regs, base
        assert (index is None) or (index in G_all_regs)

        opt = Agen(offset, base, index, scale)
        opt_info = opt.T()

    must_info = op_info.find_match(opt_info)
    assert must_info is not None
    return Op(opt, must_info), end


class Insn():

    def __init__(self, iid, lc, pieces):
        vprint(str(pieces))
        global G_all_insns
        global G_all_regs
        global G_reg_map

        assert len(pieces) > 0

        self.strahler = 0
        self.deps = set()
        self.children = set()
        self.vdeps = set()
        self.vchildren = set()

        self.lc = lc
        self.iid = iid
        self.name = pieces[0]
        self.depth = 0

        self.read_spills = None
        self.write_spills = None
        self.rw_spills = None

        self.uses = None
        self.defs = None
        self.xdefs = set()
        self.needed = False
        self.is_ret = False

        self.last_uses = set()

        assert self.name in G_all_insns, self.name

        self.insn_info = G_all_insns[self.name]
        self.needed = self.insn_info.needed
        self.ops = []
        pidx = 1
        for i in range(0, self.insn_info.num_ops()):
            assert pidx < len(pieces)
            op, next_pidx = build_op(self.insn_info.op_infos[i], pieces, pidx)
            assert next_pidx > pidx
            pidx = next_pidx
            op.complete()
            self.ops.append(op)
        assert pidx == len(pieces)

        dsts = 0
        for op in self.ops:
            if op.op_info.is_write():
                dsts += 1
        assert dsts == 1 or self.insn_info.nop, self.name

        if not self.insn_info.nop:
            assert self.insn_info.num_ops() > 0
            assert self.ops[-1].op_info.is_write(), self.name

    def fix_op_registers(self, init_in):
        fixed_ssa_out = []
        fixed_reg_out = []
        for i in range(0, len(self.ops)):
            if self.insn_info.op_infos[i].is_fixed():
                assert isinstance(self.ops[i].opt, Reg)
                if self.ops[i].op_info.is_read():
                    if self.ops[i].opt.proto_in.ssa_name in init_in:
                        assert init_in[
                            self.ops[i].opt.proto_in.
                            ssa_name] == self.insn_info.op_infos[i].fixed
                    self.ops[i].opt.proto_in.fixed = self.insn_info.op_infos[
                        i].fixed
                    fixed_ssa_out.append(self.ops[i].opt.proto_in.ssa_name)
                    fixed_reg_out.append(self.ops[i].opt.proto_in.fixed)
                    assert self.ops[i].opt.proto_in.fixed is not None
                if self.ops[i].op_info.is_write():
                    pass
            elif isinstance(self.ops[i].opt, Reg):
                if self.ops[i].op_info.is_read():
                    if self.ops[i].opt.proto_in.ssa_name in init_in:
                        assert self.ops[i].opt.proto_in.fixed is not None
                        assert self.ops[i].opt.proto_in.fixed == init_in[
                            self.ops[i].opt.proto_in.ssa_name]

                        fixed_ssa_out.append(self.ops[i].opt.proto_in.ssa_name)
                        fixed_reg_out.append(self.ops[i].opt.proto_in.fixed)

                    else:
                        self.ops[i].opt.proto_in.fixed = None
                if self.ops[i].op_info.is_write():
                    assert self.ops[i].opt.proto_out.ssa_name not in init_in
                    if self.is_ret and False:
                        global G_ret_reg
                        assert self.ops[i].opt.proto_out.fixed is not None
                        assert self.ops[i].opt.proto_out.fixed.endswith(G_ret_reg)

                        fixed_ssa_out.append(
                            self.ops[i].opt.proto_out.ssa_name)
                        fixed_reg_out.append(self.ops[i].opt.proto_out.fixed)

                    else:
                        self.ops[i].opt.proto_out.fixed = None

            elif isinstance(self.ops[i].opt, Agen):
                if self.ops[i].opt.base.proto_in.fixed is not None:
                    assert self.ops[i].opt.base.proto_in.fixed in {"rip"}
                    assert self.ops[i].opt.base.proto_in.ssa_name in init_in
                    assert init_in[self.ops[i].opt.base.proto_in.ssa_name] in {
                        "rip"
                    }
                    assert self.ops[i].opt.index is None

                    fixed_ssa_out.append(
                        self.ops[i].opt.base.proto_in.ssa_name)
                    fixed_reg_out.append(self.ops[i].opt.base.proto_in.fixed)

                if self.ops[i].opt.index is not None:
                    assert self.ops[i].opt.index.proto_in.fixed is None
        return fixed_ssa_out, fixed_reg_out

    def get_commutable_proto(self):
        if self.insn_info.commute is False:
            return None
        if self.insn_info.commute_custom:
            if self.name in {"cmpleps", "cmpltps", "cmpnleps", "cmpnltps"}:

                assert len(self.ops) == 2
                assert self.ops[1].op_info.op_target == Op_Targets.SRCDST
                assert self.ops[0].op_info.op_target == Op_Targets.SRC

                if not isinstance(self.ops[0].opt, Reg):
                    return None
                if not isinstance(self.ops[1].opt, Reg):
                    return None

                return self.ops[0].opt.proto_in

            elif self.name == "blendps":
                assert len(self.ops) == 3
                assert self.ops[2].op_info.op_target == Op_Targets.SRCDST
                assert self.ops[1].op_info.op_target == Op_Targets.SRC
                assert self.ops[0].op_info.op_target == Op_Targets.SRC

                assert isinstance(self.ops[0].opt, Imm)

                if not isinstance(self.ops[1].opt, Reg):
                    return None
                if not isinstance(self.ops[2].opt, Reg):
                    return None

                return self.ops[1].opt.proto_in
            return None
        else:
            assert len(self.ops) == 2
            assert self.ops[1].op_info.op_target == Op_Targets.SRCDST
            assert self.ops[0].op_info.op_target == Op_Targets.SRC

            if not isinstance(self.ops[0].opt, Reg):
                return None
            if not isinstance(self.ops[1].opt, Reg):
                return None

            return self.ops[0].opt.proto_in

    def get_commutable_ssa(self):
        proto = self.get_commutable_proto()
        if proto is not None:
            return proto.ssa_name
        return None

    def do_commute(self):
        if self.insn_info.commute is False:
            return False

        if self.insn_info.commute_custom:
            if self.name in {"cmpleps", "cmpltps", "cmpnleps", "cmpnltps"}:
                swaps = {
                    "cmpleps": "cmpnleps",
                    "cmpltps": "cmpnltps",
                    "cmpnleps": "cmpleps",
                    "cmpnltps": "cmpltps"
                }
                new_name = swaps[self.name]
#                assert False
                assert len(self.ops) == 2
                assert self.ops[1].op_info.op_target == Op_Targets.SRCDST
                assert self.ops[0].op_info.op_target == Op_Targets.SRC

                if not isinstance(self.ops[0].opt, Reg):
                    return False
                if not isinstance(self.ops[1].opt, Reg):
                    return False

                assert self.ops[0].op_info.op_type == self.ops[
                    1].op_info.op_type
                assert self.ops[0].op_info.size() == self.ops[1].op_info.size()

                reg0 = copy.deepcopy(self.ops[0].opt.proto_in)
                reg1 = copy.deepcopy(self.ops[1].opt.proto_in)

                self.ops[0].opt.proto_in = reg1
                self.ops[1].opt.proto_in = reg0
                self.name = new_name

            elif self.name == "blendps":
                assert len(self.ops) == 3
                assert self.ops[2].op_info.op_target == Op_Targets.SRCDST
                assert self.ops[1].op_info.op_target == Op_Targets.SRC
                assert self.ops[0].op_info.op_target == Op_Targets.SRC
                assert isinstance(self.ops[0].opt, Imm)

                if not isinstance(self.ops[1].opt, Reg):
                    return False
                if not isinstance(self.ops[2].opt, Reg):
                    return False

                assert self.ops[1].op_info.op_type == self.ops[
                    2].op_info.op_type
                assert self.ops[1].op_info.size() == self.ops[2].op_info.size()

                reg1 = copy.deepcopy(self.ops[1].opt.proto_in)
                reg2 = copy.deepcopy(self.ops[2].opt.proto_in)

                assert isinstance(self.ops[0].opt.val, int)
                assert self.ops[0].opt.val < 256
                assert self.ops[0].opt.val >= 0
                self.ops[0].opt.val ^= 0xff
                self.ops[1].opt.proto_in = reg2
                self.ops[2].opt.proto_in = reg1
                return True

            return False
        else:

            assert len(self.ops) == 2
            assert self.ops[1].op_info.op_target == Op_Targets.SRCDST
            assert self.ops[0].op_info.op_target == Op_Targets.SRC

            if not isinstance(self.ops[0].opt, Reg):
                return False
            if not isinstance(self.ops[1].opt, Reg):
                return False

            assert self.ops[0].op_info.op_type == self.ops[1].op_info.op_type
            assert self.ops[0].op_info.size() == self.ops[1].op_info.size()

            reg0 = copy.deepcopy(self.ops[0].opt.proto_in)
            reg1 = copy.deepcopy(self.ops[1].opt.proto_in)

            self.ops[0].opt.proto_in = reg1
            self.ops[1].opt.proto_in = reg0

            return True

    def is_trivial_gpr_init(self):
        if self.name not in {"movl", "movq"}:
            return None
        assert self.insn_info.num_ops() == 2
        if not (self.ops[0].op_info.is_imm() and self.ops[1].op_info.is_reg()):
            return None
        if not (isinstance(self.ops[0].opt, Imm)
                and isinstance(self.ops[1].opt, Reg)):
            return None
        if self.ops[1].op_info.op_type != Op_Types.GPR:
            return None
        return self.ops[1].opt.proto_out.ssa_name

    def is_trivial_move(self):
        if self.name not in {"movaps", "movdqa", "movups", "movdqu"}:
            return None

        assert self.insn_info.num_ops() == 2
        if not (self.ops[0].op_info.is_reg() and self.ops[1].op_info.is_reg()):
            return None
        if not (isinstance(self.ops[0].opt, Reg)
                and isinstance(self.ops[1].opt, Reg)):
            return None
        if self.ops[0].op_info.op_type != self.ops[1].op_info.op_type:
            return None
        if self.ops[0].op_info.size() != self.ops[1].op_info.size():
            return None
        if self.needed:
            return None

        return [
            copy.deepcopy(self.ops[0].opt.proto_in.ssa_name),
            copy.deepcopy(self.ops[1].opt.proto_out.ssa_name),
            copy.deepcopy(self.ops[0].opt.proto_in.fixed),
            copy.deepcopy(self.ops[1].opt.proto_out.fixed),
            copy.deepcopy(self.ops[0].opt.proto_in),
            copy.deepcopy(self.ops[1].opt.proto_out),
        ]

    def validate_strahler(self, insns):
        assert self.strahler != 0
        if len(self.vchildren) == 0:
            assert self.strahler == 1
        else:
            vals = [insns[child].strahler for child in sorted(self.vchildren)]
            assert self.strahler in {max(vals), max(vals) + 1}
            if len(vals) == 1:
                assert self.strahler == vals[0]
            else:
                vals.sort(reverse=True)
                if vals[0] == vals[1]:
                    assert self.strahler == max(vals) + 1
                else:
                    assert self.strahler == max(vals)

    def compute_strahler(self, insns):
        if self.strahler != 0:
            return self.strahler
        else:
            if len(self.vchildren) == 0:
                self.strahler = 1
            else:
                for child in self.vchildren:
                    insns[child].compute_strahler(insns)

                vals = [
                    insns[child].strahler for child in sorted(self.vchildren)
                ]
                vals.sort(reverse=True)
                if len(vals) == 1 or vals[0] > vals[1]:
                    self.strahler = vals[0]
                else:
                    self.strahler = vals[0] + 1

    def reg_sources(self):
        srcs = set()
        for i in range(0, len(self.ops)):
            if self.ops[i].op_info.is_read():
                srcs |= self.ops[i].opt.uses()
        return srcs

    def recursive_vchildren(self, insns):
        all_children = set()
        for child in self.vchildren:
            all_children |= insns[child].recursive_vchildren(insns)
        return all_children

    def prop_move_elim(self, elim, from_iid):
        ret = False
        for i in range(0, len(self.ops)):
            if self.ops[i].op_info.is_read():
                uses = self.ops[i].opt.uses()
                if elim[1] in uses:
                    if isinstance(self.ops[i].opt, Reg):
                        assert len(uses) == 1
                        self.ops[i].opt.proto_in.ssa_name = copy.deepcopy(
                            elim[0])
                        self.ops[i].opt.proto_in.fixed = copy.deepcopy(elim[2])
                        ret = True
                    elif isinstance(self.ops[i].opt, Agen):
                        assert False, "{} / {}-{}".format(
                            self.to_str(), elim[0], elim[1])
                    else:
                        assert False
            if self.ops[i].op_info.is_write():
                assert elim[1] not in self.ops[i].opt.defs(
                ), "{} - ({}){}/{}".format(self.to_str(), from_iid, elim[0],
                                           elim[1])

        return ret

    def reset_dag_info(self):
        self.uses = None
        self.defs = None
        self.xdefs = set()
        self.strahler = 0
        self.deps = set()
        self.children = set()
        self.vdeps = set()
        self.vchildren = set()

    def collect_use_defs(self):
        assert self.uses is None
        assert self.defs is None

        self.uses = set()
        self.defs = set()
        for op in self.ops:
            if op.op_info.is_read():
                self.uses |= op.opt.uses()
            if op.op_info.is_write():
                self.defs |= op.opt.defs()
            if op.op_info.is_write() and op.op_info.is_read():
                self.xdefs |= op.opt.uses()

    def to_str(self, extra=False):
        out = []
        for op in self.ops:
            if op.op_info.is_read():
                assert op.opt.name_in() is not None
                out.append(op.opt.name_in())
            if op.op_info.is_write():
                assert op.opt.name_out() is not None
                out.append(op.opt.name_out())

        plus = ""
        if extra:
            plus = " -- {}/{} -> {}".format(self.net_vaffect(),
                                            self.net_gaffect(), self.last_uses)

        iid_s = str(self.iid).ljust(4)

        sth_s = str(self.strahler).ljust(2)
        depth_s = str(self.depth).ljust(2)
        name_s = self.name.ljust(10)
        ops_s = ", ".join([x.ljust(8) for x in out])
        return "{}({}/{}): {} {}".format(iid_s, sth_s, depth_s, name_s,
                                         ops_s) + plus

    def pout(self, extra=False):
        print(self.to_str(extra))

    def to_x86_str(self):
        ops_out = []
        for op in self.ops:
            opn = op.opt.x86_name()
            if opn != "":
                ops_out.append(opn)
        extra = " /* N.  */"
        out = "{} {}".format(self.name, ", ".join(ops_out))
        if self.needed:
            out += extra
        return out

    def p_x86out(self):
        print(self.to_x86_str())

    def collect_spills(self):
        read_spills = []
        write_spills = []
        rw_spills = []
        for i in range(0, len(self.ops)):
            op = self.ops[i]
            if isinstance(op.opt, Agen):
                if not op.opt.base.rinfo.is_sp():
                    continue
                if not is_int(op.opt.offset):
                    continue
                if op.opt.index is not None:
                    continue
                if op.opt.scale is not None:
                    continue

                name = None
                if op.op_info.is_write():
                    reg_info = None
                    for j in range(0, i):
                        if not isinstance(self.ops[j].opt, Reg):
                            continue
                        for sub_op in self.insn_info.op_infos[i].sub_op_infos:
                            if (sub_op.op_type == self.ops[j].op_info.op_type
                                ) and (sub_op.size()
                                       == self.ops[j].op_info.size()):
                                assert reg_info is None
                                reg_info = self.ops[j].opt.rinfo

                    assert reg_info is not None
                    assert self.insn_info.op_infos[i].find_match(
                        reg_info) is not None
                    name = self.ops[i].opt.name_out()
                    if op.op_info.is_write_only():
                        write_spills.append((self.iid, i, name, reg_info))
                    else:
                        namein = self.ops[i].opt.name_in()
                        rw_spills.append((self.iid, i, name, reg_info, namein))
                else:
                    name = self.ops[i].opt.name_in()
                    read_spills.append((i, name, None))
            else:
                assert op.op_info.op_type != Op_Types.AGEN

        self.read_spills = read_spills
        self.write_spills = write_spills
        self.rw_spills = rw_spills

    def replace_collected_spills(self, all_write_spills):
        assert self.read_spills is not None
        assert self.write_spills is not None
        assert self.rw_spills is not None

        for rws in self.rw_spills:

            iid = rws[0]
            idx = rws[1]
            nameout = rws[2]
            namein = rws[4]

            assert not self.ops[idx].op_info.is_write_only()
            assert not self.ops[idx].op_info.is_read_only()

            assert self.ops[idx].op_info.is_write()
            assert self.ops[idx].op_info.is_read()

            assert nameout in all_write_spills
            assert namein in all_write_spills

            spill_info = all_write_spills[nameout]
            assert spill_info[0] == self.iid
            assert spill_info[0] == iid
            reg_info = spill_info[1]

            spill_info = all_write_spills[namein]
            assert spill_info[0] != self.iid
            assert spill_info[0] != iid

            assert reg_info.size == spill_info[1].size
            assert reg_info.rtype == spill_info[1].rtype

            sub_op = self.insn_info.op_infos[idx].find_match(reg_info)
            assert sub_op is not None

            new_reg = Reg(nameout, reg_info, True, namein)
            new_reg.complete(sub_op)
            self.ops[idx].opt = new_reg
            self.ops[idx].op_info = sub_op

            assert all_write_spills[nameout][3] == 0
            all_write_spills[nameout][3] += 1
            all_write_spills[namein][2] += 1
        for ws in self.write_spills:
            iid = ws[0]
            idx = ws[1]
            name = ws[2]

            assert self.ops[idx].op_info.is_write_only()

            assert name in all_write_spills
            spill_info = all_write_spills[name]
            assert spill_info[0] == self.iid
            assert spill_info[0] == iid

            reg_info = spill_info[1]
            assert reg_info is not None
            sub_op = self.insn_info.op_infos[idx].find_match(reg_info)
            assert sub_op is not None

            new_reg = Reg(name, reg_info, True)
            new_reg.complete(sub_op)
            self.ops[idx].opt = new_reg
            self.ops[idx].op_info = sub_op

            assert all_write_spills[name][3] == 0
            all_write_spills[name][3] += 1

        for rs in self.read_spills:
            idx = rs[0]
            name = rs[1]
            assert name in all_write_spills

            assert self.ops[idx].op_info.is_read_only()

            spill_info = all_write_spills[name]
            assert spill_info[0] != self.iid

            reg_info = spill_info[1]
            assert reg_info is not None
            sub_op = self.insn_info.op_infos[idx].find_match(reg_info)
            assert sub_op is not None

            new_reg = Reg(name, reg_info, True)
            new_reg.complete(sub_op)

            self.ops[idx].opt = new_reg
            self.ops[idx].op_info = sub_op

            all_write_spills[name][2] += 1

        self.write_spills = []
        self.read_spills = []

        return all_write_spills

    def get_spill(self, find_reads=False):
        spill_op = None
        spill_idx = None
        reg_info = None
        spill_sub_op = None
        for i in range(0, len(self.ops)):
            op = self.ops[i]
            if isinstance(op.opt, Agen):
                if op.op_info.is_write_only():
                    if find_reads:
                        continue
                elif op.op_info.is_read_only():
                    if not find_reads:
                        continue

                assert op.op_info.op_type == Op_Types.AGEN
                if not op.opt.base.rinfo.is_sp():
                    continue
                if not is_int(op.opt.offset):
                    continue
                if op.opt.index is not None:
                    continue
                if op.opt.scale is not None:
                    continue
                assert spill_op is None
                spill_op = op
                spill_idx = i
                if not find_reads:
                    assert spill_idx != 0
                    for j in range(0, i):
                        if not isinstance(self.ops[j].opt, Reg):
                            continue
                        for sub_op in self.insn_info.op_infos[i].sub_op_infos:
                            if (sub_op.op_type == self.ops[j].op_info.op_type
                                ) and (sub_op.size()
                                       == self.ops[j].op_info.size()):
                                assert reg_info is None
                                reg_info = self.ops[j].opt.rinfo
                                spill_sub_op = sub_op

                    assert reg_info is not None
                    assert spill_sub_op is not None

            else:
                assert op.op_info.op_type != Op_Types.AGEN
        return spill_op, spill_idx, reg_info, spill_sub_op

    def is_spill(self):
        op, i, reg_info, sub_op = self.get_spill()
        assert (op is None) == (i is None)
        assert (op is None) == (reg_info is None)
        assert (op is None) == (sub_op is None)
        return op is not None

    def replace_spill(self, spill, reg_info):
        my_spill, spill_idx, reg_info, sub_op = self.get_spill(True)
        assert reg_info is None
        assert sub_op is None
        if my_spill is None:
            assert spill_idx is None

            return False

        assert spill_idx is not None
        spill_name = None
        if spill.op_info.is_write_only():
            spill_name = spill.opt.name_out()
        else:
            assert False

        my_spill_name = None
        if my_spill.op_info.is_write_only():
            assert False
        else:
            my_spill_name = my_spill.opt.name_in()

        if spill_name != my_spill_name:
            return False

        assert my_spill.opt.offset == spill.opt.offset

        self.ops[spill_idx].opt = Reg(reg_info.name, reg_info)
        self.ops[spill_idx].op_info = self.insn_info.op_infos[
            spill_idx].find_match(reg_info)
        assert self.ops[spill_idx].op_info is not None
        self.ops[spill_idx].opt.complete(self.ops[spill_idx].op_info)

        return True

    def count_net_affect(self, prefix):
        n_defs = 0
        for d in self.defs:
            assert d.count(prefix) <= 1
            n_defs += d.count(prefix)
        n_luses = 0
        for lu in self.last_uses:
            assert lu.count(prefix) <= 1
            n_luses += lu.count(prefix)

        return 10 + (n_defs - n_luses)

    def count_net_affect2(self, prefix):
        global G_ruses
        n_defs = 0
        for d in self.defs:
            assert d.count(prefix) <= 1
            n_defs += d.count(prefix)
        n_luses = 0
        completed = G_ruses.query_insn(self)
        for lu in completed:
            assert lu.count(prefix) <= 1
            n_luses += lu.count(prefix)

        return 10 + (n_defs - n_luses)

    def is_dst_net_destructive(self):
        if not self.is_dst_destructive():
            return False
        return True

    def is_dst_net_destructive_cnt(self):
        if self.is_dst_net_destructive():
            return 1
        return 0

    def is_dst_destructive(self):
        global G_ruses
        if self.insn_info.num_ops() == 0:
            return False
        opi = self.ops[-1].op_info
        opt = self.ops[-1].opt
        if not isinstance(opt, Reg):
            return False
        if opi.op_target != Op_Targets.SRCDST:
            return False
        if opt.proto_in.ssa_name in G_ruses.query_insn(self):
            return False
        return True

    def guses(self):
        gprs = set()
        for use in self.uses:
            if use.startswith("gpr-"):
                gprs.add(use)
        return gprs

    def maybe_dst_destructive(self):
        if self.insn_info.num_ops() == 0:
            return None
        opi = self.ops[-1].op_info
        opt = self.ops[-1].opt
        if not isinstance(opt, Reg):
            return None
        if opi.op_target != Op_Targets.SRCDST:
            return None
        return opt.proto_in.ssa_name

    def get_dst(self):
        assert self.insn_info.num_ops() > 0
        op = self.ops[-1]
        assert op.op_info.is_write()
        return op

    def net_vaffect(self):
        return self.count_net_affect("vec-")

    def net_vaffect2(self):
        return self.count_net_affect2("vec-")

    def net_gaffect2(self):
        return self.count_net_affect2("gpr-")

    def net_gaffect(self):
        return self.count_net_affect("gpr-")

    def vec_dst(self):
        if self.insn_info.nop:
            return False
        assert self.insn_info.num_ops() > 0
        assert self.ops[-1].op_info.is_write()
        return self.ops[-1].op_info.op_type == Op_Types.VEC

    def fix_return_dst(self):
        global G_ret_reg
        assert self.insn_info.num_ops() > 0
        assert self.vec_dst()
        assert self.ops[-1].op_info.is_write()

        assert isinstance(self.ops[-1].opt, Reg)
        assert self.ops[-1].opt.proto_out is not None
        assert self.ops[-1].opt.proto_out.fixed is None
        assert self.ops[-1].opt.rname.endswith(G_ret_reg)
        self.needed = True

        self.is_ret = True


#        self.ops[-1].opt.proto_out.fixed = self.ops[-1].opt.rname


def split_lines(f):
    lines = []
    in_comment = False

    labels = []
    comments = []
    lc = 0
    comment = None
    for line in open(f):
        lc += 1
        line = strs(line)

        assert line.count("/*") <= 1
        assert line.count("*/") <= 1
        assert line.count("//") <= 1

        pos_sc = line.find("/*")
        pos_ec = line.find("*/")
        pos_lc = line.find("//")
        if "/*" in line and "*/" in line:
            assert not in_comment
            assert pos_sc < pos_ec
            assert comment is None
            comment = [lc, [line[pos_sc:pos_ec + 2]]]
            line = line[:pos_sc] + line[pos_ec + 2:]
            comments.append(copy.deepcopy(comment))
            comment = None
        elif "/*" in line and "*/" not in line:
            assert not in_comment
            assert comment is None
            in_comment = True
            comment = [lc, [line]]
        elif "/*" not in line and "*/" in line:
            assert in_comment
            in_comment = False
            assert comment is not None
            comment[1].append(line[:pos_ec + 2])
            line = line[pos_ec + 2:]
            comments.append(copy.deepcopy(comment))
            comment = None

        line = strs(line)

        if "//" in line:
            assert comment is None
            assert not in_comment
            comment = [lc, [line[pos_lc:]]]
            line = line[:pos_lc]
            comments.append(copy.deepcopy(comment))
            comment = None

        if in_comment:
            continue
        line = strs(line)
        if line == "":
            continue
        if line.startswith("cfi_"):
            continue
        if line.lower().startswith("inc_fallback"):
            continue

        if line.count("(") == 1 and line.count(")") == 1 and line.lower(
        ).startswith("l(") and line.endswith("):"):
            labels.append((lc, line))
            continue

        pieces = line.split()
        for i in range(0, len(pieces)):
            piece = pieces[i]
            assert piece.count(",") <= 1
            if "," in piece:
                assert piece[-1] == ","
                piece = piece[:-1]
            pieces[i] = piece
        lines.append((lc, pieces))
    return lines, sorted(comments,
                         key=lambda c: c[0]), sorted(labels,
                                                     key=lambda lbl: lbl[0])


def replace_spills(insns):
    global G_reg_map
    for i in range(0, len(insns)):
        insns[i].collect_spills()

    all_write_spills = {}
    for i in range(0, len(insns)):
        for ws in insns[i].write_spills + insns[i].rw_spills:
            iid = ws[0]
            name = ws[2]
            reg_info = ws[3]

            assert iid == insns[i].iid

            assert name not in all_write_spills
            spill_info = [iid, reg_info, 0, 0]

            all_write_spills[name] = spill_info
    for i in range(0, len(insns)):
        all_write_spills = insns[i].replace_collected_spills(all_write_spills)

    return insns


def move_elim(insns):
    vprint("Move Eliminating!")
    for insn in insns:

        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.op_info.is_read()
                else:
                    assert not op.op_info.is_read()

                if op.opt.proto_out is not None:
                    assert op.op_info.is_write(), "{} -> {}".format(
                        insn.to_str(), op.opt.proto_out.ssa_name)
                else:
                    assert not op.op_info.is_write()

    moves = {}
    trivial_moves = []
    dead = set()
    for i in range(0, len(insns)):
        tmov = insns[i].is_trivial_move()
        if tmov is not None:
            if tmov[0] in moves:
                tmov[2] = moves[tmov[0]][2]
                tmov[0] = moves[tmov[0]][0]
            assert tmov[1] not in moves
            moves[tmov[1]] = tmov
            assert i == insns[i].iid
            trivial_moves.append([i, [tmov[0], tmov[1], tmov[2], tmov[3]]])

    changed = True
    while changed:
        changed = False
        for trivial_move in trivial_moves:
            for i in range(0, len(insns)):
                if i == trivial_move[0]:
                    continue
                ichanged = insns[i].prop_move_elim(trivial_move[1],
                                                   trivial_move[0])
                if ichanged:
                    dead.add(trivial_move[0])
                changed |= ichanged

    tmp_insns = []
    for i in range(0, len(insns)):
        if i in dead:
            continue
        tmp_insns.append(insns[i])
    insns = tmp_insns
    for i in range(0, len(insns)):
        insns[i].iid = i

    for insn in insns:
        vvvprint(insn.to_str())


#        insns[i].pout()
    return insns


def set_return(insns):
    good = 0

    for i in range(0, len(insns)):

        if good % 2 == 0:
            if insns[-(i + 1)].name == "ret":
                good += 1
                continue
            else:
                insns[-(i + 1)].needed = True
        else:
            if insns[-(i + 1)].vec_dst():
                insns[-(i + 1)].fix_return_dst()
                good += 1
                break

    assert good % 2 == 0 and good == 2
    return insns


def collect_DAG_deps(insns, insn, all_defs, all_uses, completed):
    if insn.iid in completed:
        return set()
    completed.add(insn.iid)
    deps = []
    all_deps = {insn.iid}
    for use in insn.uses:
        if use in all_defs:
            deps.append(insns[all_defs[use]])

    for dep in deps:
        all_deps = all_deps | collect_DAG_deps(insns, dep, all_defs, all_uses,
                                               completed)
    return all_deps


def build_DAG(insns, strat):
    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.op_info.is_read()
                else:
                    assert not op.op_info.is_read()

                if op.opt.proto_out is not None:
                    assert op.op_info.is_write(), "{} -> {}".format(
                        insn.to_str(), op.opt.proto_out.ssa_name)
                else:
                    assert not op.op_info.is_write()

    for insn in insns:
        insn.reset_dag_info()
    for insn in insns:
        insn.collect_use_defs()

    all_defs = {}
    all_uses = {}

    misused = []
    for i in range(0, len(insns)):
        assert insns[i].iid == i
        for rdef in insns[i].defs:
            assert rdef not in all_defs
            all_defs[rdef] = i

    for i in range(0, len(insns)):
        for ruse in insns[i].uses:
            if ruse not in all_defs:
                misused.append(ruse)
            all_uses.setdefault(ruse, []).append(i)

    end_goal = []
    for i in range(0, len(insns)):
        if insns[i].needed:
            end_goal.append(insns[i].iid)
    assert len(end_goal) != 0

    all_deps = set()

    for iid in end_goal:
        insn = insns[iid]
        all_deps = all_deps | collect_DAG_deps(insns, insn, all_defs, all_uses,
                                               set())

    cur_defs = copy.deepcopy(all_defs)
    dep_tree = []
    all_pops = set()
    changed = True
    while changed:
        changed = False
        this_level = []
        for iid in sorted(all_deps):
            insn = insns[iid]
            if insn.iid in all_pops:
                continue
            good = True
            for use in insn.uses:
                if use in cur_defs:
                    good = False
                    break
            if good:
                all_pops.add(insn.iid)
                this_level.append(insn.iid)
                changed = True
        for iid in sorted(this_level):
            insn = insns[iid]
            for rdef in insn.defs:
                assert rdef in cur_defs
                cur_defs.pop(rdef)

        dep_tree.append(sorted(this_level))

    max_use = {}
    assert len(all_pops) == len(all_deps)

    for dlvl in range(0, len(dep_tree)):
        for iid in sorted(dep_tree[dlvl]):
            insns[iid].depth = dlvl
            for use in insns[iid].uses:
                max_use[use] = dlvl

    for dlvl in range(0, len(dep_tree)):
        for iid in sorted(dep_tree[dlvl]):
            for use in insns[iid].uses:
                if max_use[use] == dlvl:
                    insns[iid].last_uses.add(use)

    for iid in sorted(all_deps):
        insn = insns[iid]
        for use in sorted(insn.uses):
            if use in sorted(all_defs):
                assert all_defs[use] != iid
                insn.deps.add(all_defs[use])
                if use.startswith("vec-"):
                    insn.vdeps.add(all_defs[use])

    for iid in sorted(all_deps):
        insn = insns[iid]
        for dep in insn.deps:
            insns[dep].children.add(insn.iid)
        for vdep in insn.vdeps:
            insns[dep].vchildren.add(insn.iid)
    for iid in all_deps:
        assert insns[iid].iid not in insns[iid].deps, insns[iid].to_str()
    for iid in all_deps:
        assert insns[iid].iid not in insns[iid].children, insns[iid].to_str()

    for iid in sorted(all_deps):
        insn = insns[iid]
        insn.compute_strahler(insns)

    for iid in sorted(all_deps):
        insn = insns[iid]
        insn.validate_strahler(insns)

    all_pops = set()
    order = []
    destructive = []
    cur_defs = copy.deepcopy(all_defs)
    if True:
        global G_ruses
        usables = set()
        G_ruses = RUses(insns, all_deps)
        G_ruses.init_tracker()

        maybe_destructives = set()
        for iid in sorted(all_deps):
            lu = insns[iid].maybe_dst_destructive()
            if lu is not None:
                maybe_destructives.add(lu)

        def strh_srt_cost(i):

            opt = ((1 - insns[i].is_dst_net_destructive_cnt()) << 0)
            opt2 = len(insns[i].vchildren)
            if strat <= 1:
                opt2 = len(insns[i].recursive_vchildren(insns))

            return (insns[i].strahler << 20) + (opt2 << 10) + opt

        def strh_order(insn):
            if insn.iid in all_pops:
                return

            for dep in sorted(insn.deps, reverse=True, key=strh_srt_cost):
                strh_order(insns[dep])
            if insn.iid not in all_pops:
                all_pops.add(insn.iid)
                order.append(insn.iid)
                destructive.append(insn.is_dst_net_destructive())
                G_ruses.use_insn(insn)

                for rdef in insn.defs:
                    assert rdef in cur_defs
                    cur_defs.pop(rdef)

                changed = True
                while changed:
                    changed = False
                    outstanding = sorted(all_deps - all_pops)
                    for todo in range(0, 2):

                        for iid in outstanding:
                            if todo == 0 and insns[iid].is_dst_net_destructive(
                            ):
                                continue
                            if iid in all_pops:
                                continue
                            assert iid not in all_pops
                            assert iid != insn.iid
                            if insns[iid].net_vaffect2() <= 10:
                                if insns[iid].net_vaffect2() == 10:
                                    gdefs = insns[iid].guses()
                                    plus = 1
                                    for gdef in gdefs:
                                        if gdef in maybe_destructives:
                                            plus = todo
                                    if strat & 1:
                                        plus = todo
                                    if insns[iid].net_gaffect2() > (
                                            10 + (1 - plus)):
                                        continue
                                good = True
                                for use in insns[iid].uses:
                                    if use in cur_defs:
                                        good = False
                                        break
                                if good:
                                    changed = True
                                    all_pops.add(insns[iid].iid)
                                    order.append(insns[iid].iid)
                                    destructive.append(
                                        insns[iid].is_dst_net_destructive())
                                    G_ruses.use_insn(insns[iid])
                                    for rdef in insns[iid].defs:
                                        assert rdef in cur_defs
                                        cur_defs.pop(rdef)

            else:
                pass

        for end_iid in sorted(end_goal,
                              reverse=True,
                              key=lambda i: insns[i].strahler):
            strh_order(insns[end_iid])

        assert len(all_pops) == len(all_deps)

        gprs_waiting = {}
        sched_insns = []
        for iid in order:
            tgpr = insns[iid].is_trivial_gpr_init()
            if tgpr is not None:
                gprs_waiting[tgpr] = iid
            else:
                for use in insns[iid].uses:
                    if use.startswith("gpr-") and (use in gprs_waiting):
                        sched_insns.append(gprs_waiting[use])
                        gprs_waiting.pop(use)
                sched_insns.append(iid)
        assert len(gprs_waiting) == 0
        assert len(order) == len(sched_insns)
        order = sched_insns

    else:

        changed = True
        try_alloc = set()

        def srt_lambda(i):
            return i
            if i in try_alloc:
                # return 100 - insns[i].strahler
                pass
            return 10000 - insns[i].strahler
            if insns[i].needed:
                return 1000
            else:
                return ((100 - insns[iid].depth) << 30) + (
                    insns[i].net_vaffect() << 0) + (
                        insns[i].net_gaffect() >> 20) + (
                            max([max_use[use] for use in insns[i].defs]) >> 30)

        while changed:
            changed = False
            usables = set()
            for iid in all_deps:
                insn = insns[iid]

                if insn.iid in all_pops:
                    continue
                good = True
                assert len(insn.defs) != 0 or insn.needed, insn.iid
                for use in insn.uses:
                    if use in cur_defs:
                        good = False
                        break
                if good:
                    assert iid not in usables
                    usables.add(iid)

            tmp = [x for x in usables]
            tmp.sort(key=srt_lambda)
            for iid in tmp:
                if srt_lambda(iid) > 100:
                    try_alloc = set()
                print("{} -> {}".format(iid, srt_lambda(iid)))
                all_pops.add(iid)
                order.append(iid)
                insn = insns[iid]
                try_alloc |= insn.children
                for rdef in insn.defs:
                    assert rdef in cur_defs
                    cur_defs.pop(rdef)
                changed = True
                break

    tmp_insns = []
    for iid in order:
        tmp_insns.append(insns[iid])

    insns = tmp_insns
    for i in range(0, len(insns)):
        insns[i].iid = i

    return insns, misused


def new_move_insn(insn):
    global G_reg_map
    assert insn.maybe_dst_destructive()
    op = insn.get_dst()
    assert op.op_info.is_write() and op.op_info.is_read()
    assert op.op_info.is_reg()
    iname = None

    commutable_ssa = insn.get_commutable_ssa()
    if op.op_info.is_gpr():
        if op.op_info.size() == 4:
            iname = "movl"
        else:
            assert op.op_info.size() == 8
            iname = "movq"
        pass
    else:
        assert op.op_info.is_vec()
        if op.op_info.size() == 4:
            assert False
        elif op.op_info.size() == 8:
            assert False
        elif op.op_info.size() == 16:
            iname = "movaps"
        elif op.op_info.size() == 32:
            iname = "vmovaps"
        else:
            assert op.op_info.size() == 64
            iname = "vmovdqa64"
    assert iname is not None

    src = copy.deepcopy(op.opt)
    insn_out = Insn(0, -1,
                    [iname, "%" + op.opt.rinfo.name, "%" + op.opt.rinfo.name])
    assert len(insn_out.ops) == 2
    assert insn_out.ops[0].op_info.is_reg() and insn_out.ops[1].op_info.is_reg(
    )
    assert insn_out.ops[0].op_info.op_target == Op_Targets.SRC
    assert insn_out.ops[0].op_info.op_type == op.op_info.op_type
    assert insn_out.ops[1].op_info.op_target == Op_Targets.DST
    assert insn_out.ops[1].op_info.op_type == op.op_info.op_type
    insn_out.ops[0].opt = copy.deepcopy(src)
    assert insn_out.ops[0].opt.proto_out is not None
    insn_out.ops[0].opt.proto_out = None

    return insn_out, commutable_ssa, copy.deepcopy(
        insn_out.ops[0].opt.proto_in.ssa_name), copy.deepcopy(
            insn_out.ops[1].opt.proto_out.ssa_name)


def reinsert_moves(insns):
    vprint("Reinsert Moves")
    global G_ruses

    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.op_info.is_read()
                else:
                    assert not op.op_info.is_read()

                if op.opt.proto_out is not None:
                    assert op.op_info.is_write(), "{} -> {}".format(
                        insn.to_str(), op.opt.proto_out.ssa_name)
                else:
                    assert not op.op_info.is_write()

    for insn in insns:
        insn.reset_dag_info()
    for insn in insns:
        insn.collect_use_defs()

    G_ruses = RUses(insns)
    G_ruses.init_tracker()
    true_last_uses = {}
    for i in range(0, len(insns)):
        for use in insns[i].uses:
            true_last_uses[use] = i

    need_moves = []
    need_moves2 = []
    for i in range(0, len(insns)):
        lu = insns[i].maybe_dst_destructive()
        if lu is not None:
            assert lu in true_last_uses
            if i != true_last_uses[lu]:
                need_moves.append(i)
        if insns[i].is_dst_destructive():
            need_moves2.append(i)
        G_ruses.use_insn(insns[i])
    assert sorted(need_moves) == sorted(need_moves2)
    assert len(need_moves) != 0

    moves = {}

    changed = True
    while changed:
        changed = False
        tmp_insns = []
        moves = {}

        true_last_uses = {}
        for i in range(0, len(insns)):
            insns[i].iid = i
            insns[i].reset_dag_info()
            insns[i].collect_use_defs()
            for use in insns[i].uses:
                true_last_uses[use] = i

        for i in range(0, len(insns)):
            for op in insns[i].ops:
                if len(moves) == 0:
                    break

                if op.op_info.is_read() and (isinstance(op.opt, Reg)
                                             or isinstance(op.opt, Agen)):
                    if isinstance(op.opt, Reg):
                        if op.opt.proto_in.ssa_name not in moves:
                            continue

                        op.opt.proto_in.ssa_name = moves[
                            op.opt.proto_in.ssa_name]
                        op.opt.proto_in.fixed = None
                    else:
                        assert isinstance(op.opt, Agen)
                        ssa_name_base = op.opt.base.proto_in.ssa_name
                        if ssa_name_base in moves:
                            op.opt.base.proto_in.ssa_name = moves[
                                ssa_name_base]
                            assert op.opt.base.proto_in.fixed is None

                        if op.opt.index is not None:
                            ssa_name_index = op.opt.index.proto_in.ssa_name

                            if ssa_name_index in moves:
                                op.opt.index.proto_in.ssa_name = moves[
                                    ssa_name_index]
                                assert op.opt.index.proto_in.fixed is None

            if len(moves) == 0:
                lu = insns[i].maybe_dst_destructive()
                if lu is not None:
                    assert lu in true_last_uses
                    if i != true_last_uses[lu]:
                        new_insn, op_src, mov_src, mov_dst = new_move_insn(
                            insns[i])
                        vvvprint("New Move: {}\n\t{} -> {}".format(
                            insns[i].to_str(), mov_src, mov_dst))
                        changed = True
                        did_commute = False
                        if op_src is not None:
                            if true_last_uses[op_src] == i:
                                did_commute = insns[i].do_commute()
                                vvvprint("COMMUTED!")

                        if not did_commute:
                            vvvprint("INSERTED!")
                            moves[mov_src] = mov_dst
                            tmp_insns.append(new_insn)
                            for op in new_insn.ops:
                                if isinstance(op.opt, Reg):
                                    if op.opt.proto_in is not None:
                                        assert op.op_info.is_read()
                                    else:
                                        assert not op.op_info.is_read()

                                    if op.opt.proto_out is not None:
                                        assert op.op_info.is_write(
                                        ), "{} -> {}".format(
                                            new_insn.to_str(),
                                            op.opt.proto_out.ssa_name)
                                    else:
                                        assert not op.op_info.is_write()

            tmp_insns.append(insns[i])
        insns = tmp_insns

    for i in range(0, len(insns)):
        insns[i].iid = i
        insns[i].last_uses = set()

    for insn in insns:
        insn.reset_dag_info()
    for insn in insns:
        insn.collect_use_defs()

    G_ruses = RUses(insns)
    G_ruses.init_tracker()
    true_last_uses = {}
    for i in range(0, len(insns)):
        for use in insns[i].uses:
            true_last_uses[use] = i

    need_moves = []
    need_moves2 = []
    for i in range(0, len(insns)):
        lu = insns[i].maybe_dst_destructive()
        if lu is not None:
            assert lu in true_last_uses
            if i != true_last_uses[lu]:
                need_moves.append(i)
        if insns[i].is_dst_destructive():
            need_moves2.append(i)
        G_ruses.use_insn(insns[i])
    assert sorted(need_moves) == sorted(need_moves2)
    assert len(need_moves) == 0

    for i in range(0, len(insns)):
        for use in insns[i].uses:
            if true_last_uses[use] == i:
                insns[i].last_uses.add(use)
    for insn in insns:
        vvvprint(insn.to_str())

    for i in range(0, len(insns)):
        for j in range(0, len(insns[i].ops)):
            op = insns[i].ops[j]
            if op.op_info.op_target == Op_Targets.SRCDST:
                assert op.op_info.is_reg()
                assert op.op_info.is_gpr() or op.op_info.is_vec()
                assert j == (len(insns[i].ops) - 1)
                assert op.opt.proto_in.ssa_name in insns[i].last_uses


#                assert op.opt.proto_in.ssa_name != "vec-70", "{} -> {}".format(insns[i].to_str(), "vec-70")
    return insns


class Colorer():

    def __init__(self, insns, liveness, fixed_map):
        global G_all_regs

        vprint("Coloring!")
        for insn in insns:
            vvvprint(insn.to_str())
        color_sets = {}
        interferences = {}
        for reg in G_all_regs:
            color_sets.setdefault(G_all_regs[reg].type_str(), [0, set()])
            interferences.setdefault(G_all_regs[reg].type_str(), {})
        for fixed_ssa in fixed_map:
            fixed_reg = fixed_map[fixed_ssa]
            assert fixed_reg in G_all_regs

        for live_state in liveness:
            for use in live_state:
                tname = ssa_name_to_type_name(use)
                interferences[tname].setdefault(use, set())
                for neighbor in live_state:
                    if ssa_name_to_type_name(
                            neighbor) == tname and neighbor != use:
                        interferences[tname][use].add(neighbor)
        for tname in interferences:
            for use in interferences[tname]:
                assert ssa_name_to_type_name(use) == tname
                assert use not in interferences[tname][use]

        for init_reg in liveness[0]:
            assert init_reg in fixed_map

        self.not_usables = {}
        self.must_uses = {}
        self.must_match = {}
        self.color_sets = color_sets
        self.interferences = interferences
        self.liveness = liveness
        self.all_colored = {}

        for tname in interferences:
            for use in interferences[tname]:
                self.not_usables[use] = set()

        init_colors = {}
        for init_reg in fixed_map:
            color = None
            fixed_reg = fixed_map[init_reg]
            tname = ssa_name_to_type_name(init_reg)
            if fixed_reg in init_colors:
                color = init_colors[fixed_reg]
            else:
                color = self.get_new_color(init_reg)
                init_colors[fixed_reg] = color

            assert tname in self.interferences
            assert init_reg in self.interferences[tname]
            for interference in self.interferences[tname][init_reg]:
                self.not_usables[interference].add(color)
            assert init_reg not in self.must_uses
            self.must_uses[init_reg] = color

        for init_reg in fixed_map:
            assert init_reg in self.must_uses
            assert self.must_uses[init_reg] not in self.not_usables[init_reg]

        self.fixed_map = fixed_map
        self.init_colors = init_colors

        for insn in insns:
            for op in insn.ops:
                if op.op_info.op_target == Op_Targets.SRCDST:
                    assert isinstance(op.opt, Reg)
                    assert op.opt.proto_out.ssa_name not in self.must_match
                    self.must_match[
                        op.opt.proto_out.ssa_name] = op.opt.proto_in.ssa_name
                    assert op.opt.proto_out.ssa_name not in self.must_uses

    def is_constrained_use(self, use):
        return (use in self.must_match) or (use in self.must_uses)

    def get_new_color(self, use):
        tname = ssa_name_to_type_name(use)
        assert tname in self.color_sets
        assert use not in self.all_colored
        next_color = self.color_sets[tname][0]
        self.color_sets[tname][0] += 1
        assert next_color not in self.color_sets[tname][1]
        self.color_sets[tname][1].add(next_color)
        return next_color

    def get_color(self, use):
        assert use in self.not_usables
        not_usable = self.not_usables[use]

        must_use = None
        if use in self.must_uses:
            must_use = self.must_uses[use]

        must_match = None
        if use in self.must_match:
            must_match = self.must_match[use]
            assert must_match in self.all_colored
            must_match = self.all_colored[must_match]
            assert must_use is None or must_use == must_match
            must_use = must_match

        tname = ssa_name_to_type_name(use)
        assert tname in self.color_sets
        assert use not in self.all_colored

        avail = self.color_sets[tname][1] - not_usable

        next_color = None
        if must_use is not None:
            assert must_use in avail, "{} -> {}".format(avail, must_use)
            next_color = must_use
            vvvprint("Coloring2: {} -> {}".format(use, next_color))
        elif len(avail) == 0:
            next_color = self.get_new_color(use)
            vvvprint("Coloring1: {} -> {}".format(use, next_color))
        else:
            next_color = min(avail)
            vvvprint("Coloring0: {} -> {}".format(use, next_color))

        assert next_color in self.color_sets[tname][1]
        self.color_sets[tname][1].remove(next_color)
        assert next_color not in self.color_sets[tname][1]
        return next_color

    def color_use(self, use):
        tname = ssa_name_to_type_name(use)
        if use in self.all_colored:
            return

        color = self.get_color(use)
        assert color not in self.color_sets[tname][1]
        self.all_colored[use] = color
        return

    def release_use(self, use):
        assert use in self.all_colored
        tname = ssa_name_to_type_name(use)
        color = self.all_colored[use]
        assert self.color_sets[tname][0] > color
        assert color not in self.color_sets[tname][1]
        self.color_sets[tname][1].add(color)

    def has_colored_use(self, use):
        return self.get_use_color(use) is not None

    def get_use_color(self, use):
        if use not in self.all_colored:
            return None
        color = self.all_colored[use]
        tname = ssa_name_to_type_name(use)
        assert color < self.color_sets[tname][0]
        return color

    def color_level(self, live_level, next_level):
        for use in live_level:
            if self.is_constrained_use(use):
                self.color_use(use)
        for use in live_level:
            if not self.is_constrained_use(use):
                self.color_use(use)
        if next_level is not None:
            for use in (live_level - next_level):
                self.release_use(use)

    def color(self):
        for i in range(0, len(self.liveness) - 1):
            self.color_level(self.liveness[i], self.liveness[i + 1])
        self.color_level(self.liveness[-1], None)

        for use in self.all_colored:
            tname = ssa_name_to_type_name(use)
            assert tname in self.interferences
            assert use in self.interferences[tname]
            assert self.all_colored[use] not in self.not_usables[use]
            if use in self.must_uses:
                assert self.all_colored[use] == self.must_uses[use]
            for interference in self.interferences[tname][use]:
                assert interference in self.all_colored
                assert self.all_colored[interference] != self.all_colored[use]

        for init_reg in self.fixed_map:
            assert init_reg in self.all_colored
            assert init_reg in self.must_uses
            fixed_reg = self.fixed_map[init_reg]
            assert fixed_reg in self.init_colors

            assert self.init_colors[fixed_reg] == self.all_colored[init_reg]


def build_liveness(insns, misused):
    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.op_info.is_read()
                else:
                    assert not op.op_info.is_read()

                if op.opt.proto_out is not None:
                    assert op.op_info.is_write(), "{} -> {}".format(
                        insn.to_str(), op.opt.proto_out.ssa_name)
                else:
                    assert not op.op_info.is_write()

    misused = set(misused)
    for insn in insns:
        insn.reset_dag_info()
    for insn in insns:
        insn.collect_use_defs()

    fixed_map = {}
    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.op_info.is_write():
                    assert op.opt.proto_out.ssa_name not in misused
                if op.op_info.is_read():
                    if op.opt.proto_in.ssa_name in misused:
                        assert op.opt.proto_in.fixed is not None
                        fixed_map.setdefault(op.opt.proto_in.ssa_name,
                                             op.opt.proto_in.fixed)
                        assert fixed_map[
                            op.opt.proto_in.ssa_name] == op.opt.proto_in.fixed
            elif isinstance(op.opt, Agen):
                assert op.opt.base is not None
                if op.opt.base.proto_in.ssa_name in misused:
                    assert op.opt.base.proto_in.fixed is not None
                    fixed_map.setdefault(op.opt.base.proto_in.ssa_name,
                                         op.opt.base.proto_in.fixed)
                    assert fixed_map[op.opt.base.proto_in.
                                     ssa_name] == op.opt.base.proto_in.fixed
                if (op.opt.index
                        is not None) and (op.opt.index.proto_in.ssa_name
                                          in misused):
                    assert op.opt.index.proto_in.fixed is not None
                    fixed_map.setdefault(op.opt.index.proto_in.ssa_name,
                                         op.opt.index.proto_in.fixed)
                    assert fixed_map[op.opt.index.proto_in.
                                     ssa_name] == op.opt.index.proto_in.fixed
            else:
                pass
    for name in misused:
        assert name in fixed_map

    fixed_ssas = []
    fixed_regs = []
    for insn in insns:
        fixed_ssa, fixed_reg = insn.fix_op_registers(fixed_map)
        fixed_ssas += fixed_ssa
        fixed_regs += fixed_reg

    assert len(fixed_ssas) == len(fixed_regs)
    for i in range(0, len(fixed_regs)):
        assert fixed_ssas[i] is not None
        assert fixed_regs[i] is not None

        if fixed_ssas[i] in fixed_map:
            assert fixed_map[fixed_ssas[i]] == fixed_regs[i]
        fixed_map[fixed_ssas[i]] = fixed_regs[i]

    true_last_uses = {}
    for i in range(0, len(insns)):
        for use in insns[i].uses:
            true_last_uses[use] = i

    init_live = copy.deepcopy(misused)
    liveness = [init_live]

    max_liveness = 0

    for i in range(0, len(insns)):
        live_lvl = copy.deepcopy(liveness[-1])
        for use in insns[i].uses:
            if true_last_uses[use] == i:
                assert use in live_lvl, "{} - {} in {}".format(
                    i, use, live_lvl)
                live_lvl.remove(use)
        for rdef in insns[i].defs:
            live_lvl.add(rdef)
        max_liveness = max(max_liveness, ",".join(live_lvl).count("vec-"))
        liveness.append(live_lvl)
    for i in range(0, len(insns)):
        last_uses = set()
        for use in insns[i].uses:
            assert use in liveness[i]
            if true_last_uses[use] == i:
                last_uses.add(use)
                assert use not in liveness[i + 1]
            else:
                assert use in liveness[i + 1]
        for rdef in insns[i].defs:
            assert rdef not in liveness[i]
            assert rdef in liveness[i + 1]
    return insns, liveness, fixed_map, max_liveness


def color_ssa(insns, liveness, fixed_map):
    colorer = Colorer(insns, liveness, fixed_map)
    colorer.color()
    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.op_info.is_read()
                    assert colorer.has_colored_use(op.opt.proto_in.ssa_name)
                    op.opt.proto_in.color = colorer.get_use_color(
                        op.opt.proto_in.ssa_name)
                else:
                    assert not op.op_info.is_read()

                if op.opt.proto_out is not None:
                    assert op.op_info.is_write(), "{} -> {}".format(
                        insn.to_str(), op.opt.proto_out.ssa_name)
                    assert colorer.has_colored_use(op.opt.proto_out.ssa_name)
                    op.opt.proto_out.color = colorer.get_use_color(
                        op.opt.proto_out.ssa_name)
                else:
                    assert not op.op_info.is_write()

            elif isinstance(op.opt, Agen):
                assert colorer.has_colored_use(op.opt.base.proto_in.ssa_name)
                op.opt.base.proto_in.color = colorer.get_use_color(
                    op.opt.base.proto_in.ssa_name)
                if op.opt.index is not None:
                    assert colorer.has_colored_use(
                        op.opt.index.proto_in.ssa_name)
                    op.opt.index.proto_in.color = colorer.get_use_color(
                        op.opt.index.proto_in.ssa_name)

            else:
                pass

    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.opt.proto_in.color is not None
                if op.opt.proto_out is not None:
                    assert op.opt.proto_out.color is not None
                if (op.opt.proto_in is not None) and (op.opt.proto_out
                                                      is not None):
                    assert op.opt.proto_in.color == op.opt.proto_out.color

            elif isinstance(op.opt, Agen):
                assert op.opt.base.proto_in.color is not None
                if op.opt.index is not None:
                    assert op.opt.index.proto_in.color is not None
            else:
                pass

    return insns


def reg_alloc(insns):
    global G_all_regs
    fixed_colors = {}
    color_counts = {}

    allocs = {}
    ret_color = None
    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.opt.proto_in.color is not None
                    tname = ssa_name_to_type_name(op.opt.proto_in.ssa_name)
                    fixed_colors.setdefault(tname, {})
                    color_counts.setdefault(tname, {})
                    color_counts[tname].setdefault(op.opt.proto_in.color, [])
                    color_counts[tname][op.opt.proto_in.color].append(
                        (insn, op.opt))

                    if op.opt.proto_in.fixed:
                        fixed_colors[tname].setdefault(op.opt.proto_in.color,
                                                       op.opt.proto_in.fixed)
                        assert fixed_colors[tname][
                            op.opt.proto_in.color] == op.opt.proto_in.fixed
                if op.opt.proto_out is not None:
                    assert op.opt.proto_out.color is not None
                    if insn.is_ret:
                        assert ret_color is None
                        ret_color = op.opt.proto_out.color
                    tname = ssa_name_to_type_name(op.opt.proto_out.ssa_name)
                    fixed_colors.setdefault(tname, {})
                    color_counts.setdefault(tname, {})
                    color_counts[tname].setdefault(op.opt.proto_out.color, [])
                    color_counts[tname][op.opt.proto_out.color].append(
                        (insn, op.opt))

                    if op.opt.proto_out.fixed:
                        fixed_colors[tname].setdefault(op.opt.proto_out.color,
                                                       op.opt.proto_out.fixed)
                        assert fixed_colors[tname][
                            op.opt.proto_out.color] == op.opt.proto_out.fixed
                if (op.opt.proto_in is not None) and (op.opt.proto_out
                                                      is not None):
                    assert op.opt.proto_in.color == op.opt.proto_out.color

            elif isinstance(op.opt, Agen):
                assert op.opt.base.proto_in.color is not None
                tname = ssa_name_to_type_name(op.opt.base.proto_in.ssa_name)
                fixed_colors.setdefault(tname, {})
                color_counts.setdefault(tname, {})
                color_counts[tname].setdefault(op.opt.base.proto_in.color, [])
                color_counts[tname][op.opt.base.proto_in.color].append(
                    (insn, op.opt.base))

                if op.opt.base.proto_in.fixed:
                    fixed_colors[tname].setdefault(op.opt.base.proto_in.color,
                                                   op.opt.base.proto_in.fixed)
                    assert fixed_colors[tname][
                        op.opt.base.proto_in.
                        color] == op.opt.base.proto_in.fixed
                if op.opt.index is not None:
                    tname = ssa_name_to_type_name(
                        op.opt.index.proto_in.ssa_name)
                    color_counts.setdefault(tname, {})
                    color_counts[tname].setdefault(op.opt.index.proto_in.color,
                                                   [])
                    color_counts[tname][op.opt.index.proto_in.color].append(
                        (insn, op.opt.index))

                    assert op.opt.index.proto_in.color is not None
                    assert op.opt.index.proto_in.fixed is None
            else:
                pass

    assert ret_color is not None
    avail_regs = {}
    for reg in G_all_regs:
        reg_info = G_all_regs[reg]
        avail_regs.setdefault(reg_info.type_str(), {})
        avail_regs[reg_info.type_str()][reg] = reg_info
    for tname in allocs:
        for alloc in allocs[tname]:
            assert isinstance(alloc, StackSpot) or isinstance(alloc, Reg_Info)
    for tname in fixed_colors:
        for color in fixed_colors[tname]:
            assert color in color_counts[tname]
            color_counts[tname].pop(color)
            reg = fixed_colors[tname][color]
            assert reg in avail_regs[tname]
            for sreg in G_all_regs[reg].siblings:
                avail_regs[tname].pop(sreg)
            assert reg not in avail_regs[tname]
            reg_info = G_all_regs[reg]
            allocs.setdefault(tname, {})
            assert color not in allocs[tname]
            assert isinstance(reg_info, Reg_Info)
            allocs[tname][color] = copy.deepcopy(reg_info)
            assert isinstance(allocs[tname][color], Reg_Info)

    stacks = set()
    for tname in color_counts:
        for color in sorted(color_counts[tname],
                            reverse=True,
                            key=lambda c: tern(
                                c == ret_color and tname == "vec" and
                                ("xmm0" in avail_regs[tname]), 100000,
                                len(color_counts[tname][c]))):
            usable_regs = None
            global G_isa_max
            max_sz = 0
            for reg in sorted(avail_regs[tname],
                              key=lambda rname: G_all_regs[rname].prio):
                vvprint("Checking reg: {}".format(reg))
                good = True
                reg_info = avail_regs[tname][reg]

                for insn_op in color_counts[tname][color]:
                    insn_info = insn_op[0].insn_info
                    op_info = insn_op[1].rinfo

                    assert op_info.rtype == reg_info.rtype
                    max_sz = max(max_sz, op_info.size)
                    if insn_info.isa < reg_info.isa:
                        vvprint("Bad ISA1")
                        good = False
                        break
                    if G_isa_max < reg_info.isa:
                        vvprint("Bad ISA2")
                        good = False
                        break

                    if not reg_info.may_rename:
                        vvprint("Not Usable1")
                        good = False
                        break
                    if reg_info.avoid:
                        vvprint("Done Use2")
                        good = False
                        break
                if good:
                    usable_regs = reg
                    break

            alloc = None
            if usable_regs is None:
                vprint("Unable to allocate({}/{})".format(tname, color))
                assert max_sz in {4, 8, 16, 32, 64}
                alloc = StackSpot(max_sz)
                stacks.add(max_sz)
                assert isinstance(alloc, StackSpot)
            else:
                vprint("Allocating ({}/{}) -> {}".format(
                    tname, color, usable_regs))
                assert usable_regs in avail_regs[tname]
                usable_info = avail_regs[tname][reg]
                for sib_reg in usable_info.siblings:
                    assert sib_reg in avail_regs[tname]
                    avail_regs[tname].pop(sib_reg)
                alloc = copy.deepcopy(usable_info)
                assert isinstance(alloc, Reg_Info)
            assert isinstance(alloc, StackSpot) or isinstance(alloc, Reg_Info)
            allocs.setdefault(tname, {})
            assert color not in allocs[tname]
            allocs[tname][color] = copy.deepcopy(alloc)

    stack_offset = 0
    for sz in sorted(stacks):
        stack_offset += (sz - 1)
        stack_offset -= (stack_offset % sz)
        for tname in allocs:
            for c in allocs[tname]:
                if isinstance(allocs[tname][c], StackSpot):
                    if allocs[tname][c].size == sz:
                        allocs[tname][c].offset = stack_offset
                        stack_offset += sz
                else:
                    assert isinstance(allocs[tname][c], Reg_Info)

    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    tname = ssa_name_to_type_name(op.opt.proto_in.ssa_name)
                    assert op.opt.proto_in.color is not None
                    assert op.opt.proto_in.color in allocs[tname]
                    op.opt.proto_in.alloc = Allocation(
                        allocs[tname][op.opt.proto_in.color])
                if op.opt.proto_out is not None:
                    tname = ssa_name_to_type_name(op.opt.proto_out.ssa_name)
                    assert op.opt.proto_out.color is not None
                    assert op.opt.proto_out.color in allocs[tname]
                    op.opt.proto_out.alloc = Allocation(
                        allocs[tname][op.opt.proto_out.color])
                if (op.opt.proto_in is not None) and (op.opt.proto_out
                                                      is not None):
                    assert op.opt.proto_in.color == op.opt.proto_out.color

            elif isinstance(op.opt, Agen):
                tname = ssa_name_to_type_name(op.opt.base.proto_in.ssa_name)
                assert op.opt.base.proto_in.color is not None
                assert op.opt.base.proto_in.color in allocs[tname]
                op.opt.base.proto_in.alloc = Allocation(
                    allocs[tname][op.opt.base.proto_in.color])
                if op.opt.index is not None:
                    tname = ssa_name_to_type_name(
                        op.opt.index.proto_in.ssa_name)
                    assert op.opt.index.proto_in.color is not None
                    assert op.opt.index.proto_in.color in allocs[tname]
                    op.opt.index.proto_in.alloc = Allocation(
                        allocs[tname][op.opt.index.proto_in.color])

            else:
                pass

    for insn in insns:
        for op in insn.ops:
            if isinstance(op.opt, Reg):
                if op.opt.proto_in is not None:
                    assert op.opt.proto_in.alloc is not None
                if op.opt.proto_out is not None:
                    assert op.opt.proto_out.alloc is not None
                if (op.opt.proto_in is not None) and (op.opt.proto_out
                                                      is not None):
                    assert op.opt.proto_in.color == op.opt.proto_out.color

            elif isinstance(op.opt, Agen):
                assert op.opt.base.proto_in.alloc is not None
                if op.opt.index is not None:
                    assert op.opt.index.proto_in.alloc is not None
            else:
                pass

    return insns


def restore_needed(insns):
    needed_insns = []
    rest = []
    for insn in insns:
        if insn.needed:
            needed_insns.append(insn)
        else:
            rest.append(insn)
    rest += sorted(needed_insns, key=lambda insn: insn.lc)
    insns = []
    for insn in rest:
        insns.append(insn)

    for i in range(0, len(insns)):
        insns[i].iid = i
    return insns


def remove_dup_moves(insns):
    tmp_insns = []
    for insn in insns:
        tmov = insn.is_trivial_move()
        skip = False
        if tmov is not None:
            if tmov[4].color == tmov[5].color:
                skip = True
        if skip:
            assert False
        else:
            tmp_insns.append(insn)

    insns = tmp_insns
    for i in range(0, len(insns)):
        insns[i].iid = i
    return insns


def optimize(insn_lines):
    insns = []
    i = 0
    for insn_pieces in insn_lines:
        insn = Insn(i, insn_pieces[0], insn_pieces[1])
        insns.append(insn)
        i += 1

    insns = set_return(insns)
    insns = replace_spills(insns)

    init_insns = copy.deepcopy(insns)
    best_insns = None
    best_liveness = None
    best_fixed_map = None
    best_cost = None
    iter_cnt = 5
    strat_cnt = 4
    for strat in range(0, strat_cnt):
        for i in range(0, iter_cnt):
            insns = move_elim(insns)
            insns, init_regs = build_DAG(insns, strat)
            insns = restore_needed(insns)
            insns = reinsert_moves(insns)
            insns, liveness, fixed_map, max_liveness = build_liveness(
                insns, init_regs)
            cost = len(insns) + max_liveness * 5
            if max_liveness > 16:
                cost += 250 * (max_liveness - 16)

            if best_cost is None:
                assert best_insns is None
                assert best_liveness is None
                assert best_fixed_map is None
                best_cost = cost + 1

            if cost < best_cost:
                best_cost = cost
                best_insns = copy.deepcopy(insns)
                best_liveness = copy.deepcopy(liveness)
                best_fixed_map = copy.deepcopy(fixed_map)

            assert best_insns is not None
            assert best_liveness is not None
            assert best_fixed_map is not None
            assert best_cost is not None

        init_insns = move_elim(init_insns)
        for i in range(0, iter_cnt):
            tmp_insns = copy.deepcopy(init_insns)
            for j in range(0, i + 1):
                tmp_insns, init_regs = build_DAG(tmp_insns, strat)
            tmp_insns = restore_needed(tmp_insns)
            tmp_insns = reinsert_moves(tmp_insns)

            tmp_insns, liveness, fixed_map, max_liveness = build_liveness(
                tmp_insns, init_regs)

            cost = len(tmp_insns) + max_liveness * 5
            if max_liveness > 16:
                cost += 250 * (max_liveness - 16)

            if cost < best_cost:
                best_cost = cost
                best_insns = copy.deepcopy(tmp_insns)
                best_liveness = copy.deepcopy(liveness)
                best_fixed_map = copy.deepcopy(fixed_map)

    insns = color_ssa(best_insns, best_liveness, best_fixed_map)
    insns = remove_dup_moves(insns)
    insns = reg_alloc(insns)

    return insns


def output(insns, comments, labels):
    insns_out = []
    for insn in insns:
        insns_out.append((insn.lc, insn.to_x86_str()))

    comments_out = []
    for comment in comments:
        comments_out.append((comment[0], "\n".join(comment[1])))

    labels_out = labels

    aux_out = comments_out + labels_out
    aux_out.sort(key=lambda aux: aux[0])

    aux_places = {}
    for aux in aux_out:
        best = 100000
        best_dist = 100000
        for insn in insns_out:
            if insn[0] < aux[0]:
                continue

            dist = insn[0] - aux[0]
            if dist < best_dist:
                best = insn[0]
                best_dist = dist
        aux_places.setdefault(best, []).append(aux[1])

    out = []

    out.append("// Code Out Begin")

    for insn in insns_out:
        if insn[0] in aux_places:
            for aux in aux_places[insn[0]]:
                out.append(aux)
            aux_places.pop(insn[0])

        out.append(insn[1])
    assert len(aux_places) <= 1
    for remaining in aux_places:
        for aux in aux_places[remaining]:
            out.append(aux)

    out.append("// Code Out End")
    print("\n".join(out))


set_verbosity(0)
assert len(sys.argv) > 3
G_isa_max = isa_str_to_enum(sys.argv[1])
if len(sys.argv) > 4:
    G_ret_reg = sys.argv[4]
    
assert G_isa_max is not None
setup(sys.argv[2])
insn_lines, comments, labels = split_lines(sys.argv[3])
insns = optimize(insn_lines)

output(insns, comments, labels)

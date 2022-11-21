import os
import sys
import glob
from common_util import is_int, as_int, strs, findnth

S_files = glob.glob("**/*dev*.S", recursive=True)
H_files = glob.glob("**/*dev*.h", recursive=True)

all_data = {}
all_macros = {}


def is_data(line):
    line = strs(line)
    pieces = line.split()
    if line.startswith(".quad") or line.startswith(".long"):
        assert len(pieces) >= 2
        pieces = pieces[1:]
        vals = []

        N = 8
        if line.startswith(".long"):
            N = 4
        for i in range(0, len(pieces)):
            piece = pieces[i]
            if i + 1 != len(pieces):
                assert piece.endswith(","), line
                piece = piece[:-1]
            if not piece.startswith("0x"):
                assert N == 4
                to_hex = {
                    "0": "0x00000000",
                    "1": "0x00000001",
                    "2": "0x00000002",
                    "3": "0x00000003",
                    "5": "0x00000005",
                    "7": "0x00000007",
                    "-1": "0xffffffff"
                }
                assert piece in to_hex, line
                piece = to_hex[piece]

            assert is_int(piece, 16), line
            assert len(piece) == 2 * N + 2
            vals.append(piece)

        if N == 4:
            assert len(vals) in {1, 2, 3, 4, 8,
                                 16}, "{}: {}".format(len(vals), line)
        else:
            assert len(vals) in {1, 2, 3, 4, 8}, line
        return True, (N, vals), None

    if line.startswith("double_vector") or line.startswith("float_vector"):
        assert len(pieces) == 3
        name = pieces[1]
        val = pieces[2]
        assert val.startswith("0x") and is_int(val, 16)

        N = 8
        if line.startswith("float_vector"):
            N = 4
        assert len(val) == 2 * N + 2
        return True, (N, [val for x in range(0, int(64 / N))]), name
    if line.startswith(".space"):
        assert len(pieces) == 3
        assert pieces[2] == "0x00"
        assert pieces[1].endswith(",")
        sz = pieces[1][:-1]
        sz = int(sz)
        assert sz % 16 == 0
        return True, (1, ["0x00" for x in range(0, sz)]), None
    if line.startswith(".zero"):
        assert len(pieces) == 2
        sz = int(pieces[1])
        assert sz in {32, 64}
        return True, (1, ["0x00" for x in range(0, sz)]), None
    return False, None, None


def validate_expr(line):
    line = strs(line)
    known = [
        ".quad", ".long", ".align", ".p2align", ".section", ".text", ".macro",
        ".endm", ".globl", ".if", ".err", ".endif", ".type", ".size", ".LBL",
        ".cfi_", "...", ".FLT", ".space", ".rept", ".endr", ".zero"
    ]
    if not line.startswith("."):
        return
    for k in known:
        if line.startswith(k):
            return

    assert False, line


def is_align(line):
    line = strs(line)
    pieces = line.split()
    if line.startswith(".align"):
        assert len(pieces) == 2
        return True, int(pieces[-1])
    if line.startswith(".p2align"):
        assert len(pieces) == 2
        return True, 1 << int(pieces[-1])
    return False, None


def is_section(line):
    line = strs(line)
    return line.startswith(".section") or line == ".text"


def get_section_name(line):
    assert is_section(line)
    line = strs(line)
    if line == ".text":
        return ".text.sse2"
    line = strs(line[len(".section"):])
    if line.startswith(".text"):
        assert "," in line
        line = strs(line.split(",")[0])
        assert line.startswith(".text")
        assert line != ".text"
        assert line in {".text.sse4", ".text.avx2", ".text.evex512"}
        return line
    elif line.startswith(".rodata"):
        return ".rodata"
    else:
        assert False


def is_code_section(line):
    return is_section(line) and get_section_name(line).startswith(".text")


def is_data_section(line):
    return is_section(line) and get_section_name(line) == ".rodata"


def line_get_inside_parens(line):
    line.count("(") == line.count(")")
    assert line.count("(") == 1

    start = line.find("(")
    end = line.find(")")

    assert end > start
    content = line[start + 1:end]

    assert content.count("(") == 0 and content.count(")") == 0
    return strs(content)


def is_entry(line):
    line = strs(line)
    return line.startswith("ENTRY") and "(" in line and ")" in line[
        line.find("("):] and line.count("(") == line.count(")")


def is_end(line):
    line = strs(line)
    return line.startswith("END") and "(" in line and ")" in line[
        line.find("("):] and line.count("(") == line.count(")")


def is_include(line):
    line = strs(line)
    return line.startswith("#") and strs(
        line[1:]).startswith("include") and line.count("\"") == 2


def is_define(line):
    line = strs(line)
    return line.startswith("#") and strs(line[1:]).startswith("define")


def is_wrapper_impl(line):
    line = strs(line)
    return line.startswith(".macro WRAPPER_IMPL_")


def is_wrapper_call(line):
    line = strs(line)
    return line.startswith("WRAPPER_IMPL_")


def wrapper_call_find_name(line):
    assert is_wrapper_call(line)
    pieces = line.split()
    assert len(pieces) == 2
    name = pieces[0]
    assert name in all_macros, name
    assert len(pieces) - 1 == len(all_macros[name].macro_args())
    return name


def is_macro(line):
    line = strs(line)
    return line.startswith(".macro")


def is_macro_end(line):
    line = strs(line)
    return line.startswith(".endm")


def get_include_file(line):
    assert is_include(line)
    start = line.find("\"")
    end = findnth(line, "\"", 1)

    assert end > start

    content = line[start + 1:end]
    assert os.path.isfile(content), content
    return content


def collect_insn(lines):
    insns = []
    for line in lines:
        line = strs(line)
        if line == "":
            continue
        if line.startswith("cfi_"):
            continue
        if "JUMPTARGET" in line:
            assert line.startswith("call")
            continue
        if "//" in line:
            assert False
        if "/*" in line:
            assert False
        if "*/" in line:
            assert False
        if "#" in line:
            continue
        if is_entry(line):
            assert False
        if is_end(line):
            assert False
        if is_macro(line):
            assert False
        if is_macro_end(line):
            assert False

        res, unused0, unused1 = is_data(line)
        if res:
            assert False
        if line.startswith("."):
            assert is_align(line)
        if is_wrapper_call(line):
            name = wrapper_call_find_name(line)
            insns += all_macros[name].get_insns()
        insns.append(line)
    return insns


class Function:

    def __init__(self, lines, start, end):
        self.lines = lines
        self.start = start
        self.end = end
        self.trivial = None
        self.insns = None
        assert is_entry(lines[start])
        assert is_end(lines[end])
        assert start + 1 < end, "{} vs {}".format(start, end)

    def func_name(self):
        entry_ln = self.lines[self.start]
        end_ln = self.lines[self.end]

        fname = line_get_inside_parens(entry_ln)
        assert fname == line_get_inside_parens(end_ln)
        return fname

    def is_trivial(self):
        if self.trivial is not None:
            return self.trivial

        in_macro = False
        self.trivial = True
        for i in range(self.start + 1, self.end):
            if in_macro:
                if is_macro_end(self.lines[i]):
                    in_macro = False
                    continue

            if strs(self.lines[i]) == "":
                continue
            if is_wrapper_call(self.lines[i]):
                continue
            if is_macro(self.lines[i]):
                assert not in_macro
                in_macro = True
                continue

            self.trivial = False
            break
        assert not in_macro
        return self.trivial

    def get_insns(self):
        if self.insns is not None:
            return self.insns
        self.insns = collect_insn(self.lines[self.start + 1:self.end])
        assert len(self.insns) > 0, self.lines[self.start:self.end]
        return self.insns


class Macro:

    def __init__(self, lines, start, end):
        global all_macros

        self.lines = lines
        self.start = start
        self.end = end

        assert is_macro(lines[start])
        assert is_macro_end(lines[end])
        assert start < end

        self.insns = None

        assert self.macro_name() not in all_macros, self.macro_name()
        all_macros[self.macro_name()] = self

    def macro_name(self):
        pieces = self.lines[self.start].split()
        assert len(pieces) > 1
        return pieces[1]

    def macro_args(self):
        pieces = self.lines[self.start].split()
        assert len(pieces) > 1
        return pieces[2:]

    def is_wrapper(self):
        return is_wrapper_impl(self.lines[self.start])

    def get_insns(self):
        if self.insns is not None:
            return self.insns
        self.insns = collect_insn(self.lines[self.start + 1:self.end - 1])
        assert len(self.insns) > 0
        return self.insns


def handle_single_line_comment(line):
    check_ln = strs(line)
    if "//" in line:
        if check_ln.startswith("//"):
            line = ""
        else:
            line = line[:line.find("//")]
    return line


def handle_multi_line_comment(line, in_comment):
    check_ln = strs(line)
    if "/*" in line or "*/" in line:
        if "/*" in line and "*/" not in line:
            if "//" in line:
                assert line.find("//") > line.find("/*")
            assert check_ln.startswith("/*")
            assert not in_comment
            in_comment = True
            line = ""
        elif "/*" not in line and "*/" in line:
            if "//" in line:
                assert line.find("//") < line.find("*/")
            assert check_ln.endswith("*/")
            assert in_comment
            in_comment = False
            line = ""
        else:
            assert not in_comment
            assert line.count("/*") == 1
            assert line.count("*/") == 1
            start = line.find("/*")
            end = line.find("*/")
            assert start < end
            line = line[:start] + line[end + 2:]
    return line, in_comment


class File:

    def __init__(self, path):
        assert os.path.isfile(path)

        self.path = path
        self.lines = []

        in_comment = False
        for line in open(path):
            check_ln = strs(line)
            og_line = line
            line, in_comment = handle_multi_line_comment(line, in_comment)
            line = handle_single_line_comment(line)

            if in_comment:
                line = ""
            if "LOE" in line:
                assert check_ln.startswith("# LOE")
                line = ""
            assert "//" not in line, og_line
            assert "/*" not in line, og_line
            assert "*/" not in line, og_line
            assert "LOE" not in line, og_line

            validate_expr(line)
            self.lines.append(line)

        self.funcs = None
        self.includes = None
        self.defines = None
        self.section = None
        self.data_start = None
        self.redirect = None
        self.rodata_start = None
        self.wrapper_calls = None
        self.wrapper_impls = None
        self.arb_macros = None
        self.insns = None
        self.mem_refs = None

        self.get_funcs()
        self.get_includes()
        self.get_defines()
        self.get_section()
        self.get_rodata_start()
        self.is_redirect()
        self.get_wrapper_calls()
        self.get_wrapper_impls()
        self.get_arb_macros()

    def collect_data(self):
        global all_data

        isa_width = self.get_isa_width()
        if isa_width == 0:
            p = os.path.split(self.path)[-1]
            if p.startswith("dev-") and p.endswith("-rodata.S"):
                isa_width = 64
        for i in range(0, len(self.lines)):
            line = strs(self.lines[i])
            res, data, name = is_data(line)

            if res:
                assert self.has_data()
                assert i > self.get_rodata_start(), "{} vs {}".format(
                    i, self.get_rodata_start())
                data_glob = "".join(data[1])
                assert data_glob.count("0x") == len(data[1])
                data_glob = data_glob.replace("0x", "")
                assert len(data_glob) % 2 == 0
                assert len(data_glob) == 2 * data[0] * len(
                    data[1]), "{} ->\n\t{}\n\t{}\n\t{}".format(
                        line, data_glob, data[0], data[1])
                assert data[0] * len(data[1]) != 0
                if data[0] * len(data[1]) == isa_width:
                    all_data.setdefault(data_glob, []).append(self.path)

    def get_funcs(self):
        if self.funcs is not None:
            return self.funcs

        self.funcs = []
        entry_idx = None
        for i in range(0, len(self.lines)):
            if is_entry(self.lines[i]):
                assert entry_idx is None
                entry_idx = i
            if is_end(self.lines[i]):
                self.funcs.append(Function(self.lines, entry_idx, i))

                entry_idx = None
        for func in self.funcs:
            assert func is not None
        return self.funcs

    def get_insns(self):
        if self.insns is not None:
            return self.insns
        self.insns = []
        for func in self.funcs:
            self.insns += func.get_insns()
        return self.insns

    def is_core_file(self):
        return "/core/" in self.path

    def is_redirect(self):
        if self.redirect is not None:
            return self.redirect

        redirect = False

        if len(self.get_funcs()) == 0:
            if len(self.get_includes()) == 2 and len(self.get_defines()) == 1:
                inc0 = self.get_include_file_n(0)
                if os.path.split(inc0)[-1] == "libc-asm-common.h":
                    if self.get_includes()[0] < self.get_defines(
                    )[0] and self.get_includes()[1] > self.get_defines()[0]:
                        redirect = True

        self.redirect = redirect
        return redirect

    def is_wrapper_only(self):
        if len(self.get_includes()) > 1:
            return False
        if len(self.get_defines()) != 0:
            return False
        if len(self.get_funcs()) != 0:
            return False
        if len(self.get_wrapper_calls()) != 0:
            return False
        if len(self.get_arb_macros()) != 0:
            return False
        return len(self.get_wrapper_impls()) != 0

    def get_wrapper_calls(self):
        if self.wrapper_calls is not None:
            return self.wrapper_calls

        self.wrapper_calls = []
        for i in range(0, len(self.lines)):
            if is_wrapper_call(self.lines[i]):
                self.wrapper_calls.append(i)
        return self.wrapper_calls

    def collect_macros(self):
        if self.wrapper_impls is not None:
            assert self.arb_macros is not None
            return
        assert self.arb_macros is None
        self.arb_macros = []
        self.wrapper_impls = []

        macros = []
        begin_idx = None
        for i in range(0, len(self.lines)):
            if is_macro(self.lines[i]):
                assert begin_idx is None
                begin_idx = i
            if is_macro_end(self.lines[i]):
                assert begin_idx is not None
                macros.append(Macro(self.lines, begin_idx, i))
                begin_idx = None
        for macro in macros:
            if macro.is_wrapper():
                self.wrapper_impls.append(macro)
            else:
                self.arb_macros.append(macro)

    def get_wrapper_impls(self):
        self.collect_macros()
        return self.wrapper_impls

    def get_arb_macros(self):
        self.collect_macros()
        return self.arb_macros

    def get_include_file_n(self, n):
        assert n < len(self.get_includes())
        return get_include_file(self.lines[self.get_includes()[n]])

    def get_includes(self):
        if self.includes is not None:
            return self.includes
        self.includes = []
        for i in range(0, len(self.lines)):
            if is_include(self.lines[i]):
                self.includes.append(i)
        return self.includes

    def get_defines(self):
        if self.defines is not None:
            return self.defines
        self.defines = []
        for i in range(0, len(self.lines)):
            if is_define(self.lines[i]):
                self.defines.append(i)
        return self.defines

    def get_rodata_start(self):
        if self.rodata_start is not None:
            return self.rodata_start
        self.rodata_start = -1
        for i in range(0, len(self.lines)):
            if is_data_section(self.lines[i]):
                self.rodata_start = i
                return i

        return self.rodata_start

    def get_section(self):
        if self.section is not None:
            return self.section

        section = ""
        for line in self.lines:
            line = strs(line)
            if line == ".text":
                assert section == ""
                section = line
            elif line.startswith(".section"):

                if line.startswith(".section .rodata"):
                    continue
                assert section == ""
                section = line

        if self.is_core_file():
            assert section != "" or self.is_redirect() or self.is_wrapper_only(
            ), self.path
        else:
            assert section == "", self.path

        self.section = section
        return section

    def has_code(self):
        return self.get_section() != ""

    def has_data(self):
        return self.get_rodata_start() != -1

    def name(self):
        return self.path

    def get_isa(self):
        if self.get_section() == "":
            return "unknown"
        code_section = get_section_name(self.get_section())
        assert code_section.startswith(".text.")
        code_section = code_section.replace(".text.", "")

        assert code_section in {"sse2", "sse4", "avx2", "evex512"}
        return code_section

    def get_isa_width(self):
        isa = self.get_isa()
        widths = {
            "unknown": 0,
            "sse2": 16,
            "sse4": 16,
            "avx2": 32,
            "evex512": 64
        }
        assert isa in widths
        return widths[isa]

    def get_type(self):
        if "double/" in self.path:
            return 8
        if "float/" in self.path:
            return 4
        assert False

    def trivial_only(self):
        for func in self.get_funcs():
            assert func is not None
            if not func.is_trivial():
                return False
        return True

    def get_mem_refs(self):
        if self.mem_refs is not None:
            return self.mem_refs

        self.mem_refs = []
        for insn in self.get_insns():
            assert insn.count("(") == insn.count(")")
            if "(" in insn:
                self.mem_refs.append(insn)
        return self.mem_refs


files = []
for S_file in S_files:
    if S_file.startswith("double/"):
        continue
    f = File(S_file)
    if f.get_section() == "":
        continue

    files.append(f)

all_mem_refs = set()
for f in files:
    for insn in f.get_mem_refs():
        all_mem_refs.add(insn)
print("\n".join(all_mem_refs))

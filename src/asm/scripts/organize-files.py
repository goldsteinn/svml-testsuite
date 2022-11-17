import os
import sys
import copy

assert len(sys.argv) > 3

impl_dir = os.path.realpath(sys.argv[1])
base_dir = os.path.realpath(sys.argv[2])
ulp_file = os.path.realpath(sys.argv[3])
assert os.path.isdir(impl_dir)
assert os.path.isdir(base_dir)
assert os.path.isfile(ulp_file)

libc_hdr = os.path.realpath(os.path.join(base_dir, "libc-asm-common.h"))

known_data_gotdefs = {
    "__svml_dlog_data@GOTPCREL", "__svml_d_trig_data@GOTPCREL",
    "__svml_s_trig_data@GOTPCREL", "__svml_dpow_data@GOTPCREL",
    "__svml_dexp_data@GOTPCREL", "__svml_sexp_data@GOTPCREL",
    "__svml_slog_data@GOTPCREL", "__svml_spow_data@GOTPCREL"
}
known_data_defs = set()
for k in known_data_gotdefs:
    known_data_defs.add(k.replace("@GOTPCREL", ""))
known_data_defs.add("__svml_stanh_data")

svml_impls = []
misc_impls = []

all_impls = {}
stubs = {}
extensions = {
    "scalar": None,
    "sse2": 16,
    "sse41": 16,
    "sse42": 16,
    "sse4": 16,
    "sse": 16,
    "avx": 16,
    "avx2": 32,
    "avx512": 64
}

all_cores = set()
all_defs = set()
all_protos = {}
proto_defs = {}
unique_protos = set()

all_stub_classes = {}
all_output_classes = {}

all_unique_replacements = set()
all_replacements = []
all_repls_sorted = False
all_unique_vprotos = set()


def get_vec_type(proto, cnt):
    types = {
        "double complex": 16,
        "float complex": 8,
        "complex": 16,
        "float": 4,
        "double": 8,
        "long int": 8,
        "long long int": 8,
        "int": 4,
        "void": None,
        "int *": 4,
        "float *": 4,
        "double *": 8,
        "const char *": 1,
        "long double": 16
    }
    proto = proto.split("|")

    vtypes = []
    for p in proto:
        assert p in types
        sz = types[p]
        if sz is None:
            vtypes.append("void")
            continue
        sz *= int(cnt)
        assert sz <= 64
        assert sz >= 16 or cnt == 1, p
        T = None
        if cnt == 1:
            T = p
        else:
            if sz == 16:
                T = "__m128i"
            elif sz == 32:
                T = "__m256i"
            elif sz == 64:
                T = "__m512i"
            if "*" in p:
                T += "*"
        vtypes.append(T)
    return vtypes


def get_vproto(proto, cnt, fn, pd, sz):
    global all_unique_vprotos
    vtypes = get_vec_type(proto, cnt)
    vproto = "{}" + vtypes[0] + " {}("
    if len(vtypes) == 1:
        vproto += "void"
    else:
        vproto += ",".join(vtypes[1:])
    vproto += ");"

    all_unique_vprotos.add(vproto.format("typedef ", "(*{}_{})".format(pd,
                                                                       sz)))
    return vproto.format("extern ", fn + "_dev") + "\n" + vproto.format(
        "extern ", fn + "_glibc")


class Replacement:

    def __init__(self, old, new, at_end=False):
        global all_replacements
        global all_unique_replacements

        new = new.replace("__", "_")
        new = new.replace("--", "-")
        assert new[0] != "_"
        assert new[0] != "-"
        self.old = old
        self.new = new
        self.at_end = at_end

        assert (old is None) == (new is None)
        if old is not None:
            print("{} -> {}".format(old, new))
            assert old not in all_unique_replacements
            assert new not in all_unique_replacements, new

            all_unique_replacements.add(old)
            all_unique_replacements.add(new)

            all_replacements.append(self)

    def run0(self, s):
        assert self.old is not None
        assert self.new is not None
        return s.replace(self.old, "|" + self.new.upper() + "|")

    def run1(self, s):
        assert self.old is not None
        assert self.new is not None
        return s.replace("|" + self.new.upper() + "|", self.new)


def cmp_repls(r0):
    r0_len = len(r0.old)
    if r0.at_end and False:
        r0_len -= 1000
    return -r0_len


def run_repls(use_content):
    global all_replacements
    global all_repls_sorted
    if not all_repls_sorted:
        all_repls_sorted = True
        all_replacements.sort(key=cmp_repls)


#    ll = 1000 * 1000
    for repl in all_replacements:
        #        assert len(repl.old) <= ll
        #        ll = len(repl.old)
        use_content = repl.run0(use_content)
    for repl in all_replacements:
        use_content = repl.run1(use_content)

    return use_content


class Stub:

    def __init__(self, src_path, src_fname, dst_fname, dst_dir, used_for,
                 no_core, content):
        global all_defs
        global all_cores

        self.src_path = src_path

        self.used_for = used_for
        self.content = copy.deepcopy(content)

        assert os.path.isdir(dst_dir)

        extra_path = "/core/stubs"
        if no_core:
            extra_path = ""
        def_dir = ""
        if used_for == 0:
            def_dir = os.path.join(dst_dir, "double")
        elif used_for == 1:
            def_dir = os.path.join(dst_dir, "float")
        elif used_for == 2:
            def_dir = os.path.join(dst_dir, "shared")

        self.no_core = no_core
        self.dst_path = os.path.join(def_dir + extra_path, dst_fname)
        self.def_path = os.path.join(def_dir, "stub-" + dst_fname)
        r = Replacement(src_fname, self.dst_path)

    def output(self):
        global all_replacements
        global libc_hdr
        use_content = "/* Stub .*/\n" + "\n".join(self.content)
        use_content = run_repls(use_content)

        for impl in ["dev", "glibc"]:
            core_name = self.dst_path.replace("[DEV-OR-GLIBC]", impl)
            core_f = open(core_name, "w+")
            c = use_content
            c = c.replace("[DEV-OR-GLIBC]", impl)
            c = c.replace("[DEV_OR_GLIBC]", impl)
            c += "\n"
            core_f.write(c)
            core_f.close()
            if not no_core:

                #                print(self.def_path.replace("[DEV-OR-GLIBC]", impl))
                #                assert False

                assert not os.path.isfile(
                    self.def_path.replace("[DEV-OR-GLIBC]", impl))
                def_f = open(self.def_path.replace("[DEV-OR-GLIBC]", impl),
                             "w+")

                def_f.write(
                    "/* Stub Def. */\n#include \"{}\"\n#include \"{}\"\n".
                    format(libc_hdr, core_name))
                def_f.close()


class Outputter:

    def __init__(self, src_path, def_dst_path, core_dst_path, func_name,
                 base_func, func_names, nentries, is_float, ext, cnt,
                 stub_redirect, misc, stub, content):
        global all_impls
        global all_defs
        global all_cores
        global all_protos

        src_fname = src_path.split("/")[-1]
        r = Replacement(src_fname, core_dst_path)
        assert nentries >= 0 and nentries <= 2
        if nentries == 0:
            assert stub_redirect is not None
            assert not misc
            assert not stub
        if misc:
            assert stub_redirect is None
        if stub:
            assert stub_redirect is None
            assert nentries == 1

        assert def_dst_path not in all_defs
        all_defs.add(def_dst_path)
        assert core_dst_path not in all_cores
        all_cores.add(core_dst_path)

        assert def_dst_path.count(func_name) == 1, "{} -> {}".format(
            def_dst_path, func_name)
        assert core_dst_path.count(func_name) == 1

        self.key = func_name
        self.src_path = src_path
        self.def_dst_path = def_dst_path
        self.core_dst_path = core_dst_path
        self.func_name = func_name
        self.func_names = func_names
        self.base_func = base_func
        self.nentries = nentries
        self.is_float = is_float
        self.ext = ext
        self.cnt = cnt
        self.stub_redirect = stub_redirect
        self.misc = misc
        self.stub = stub

        self.content = copy.deepcopy(content)

        all_impls.setdefault(self.key, [False, False])

        if is_float:
            assert not all_impls[self.key][0]
            all_impls[self.key][0] = True
        else:
            assert not all_impls[self.key][1], self.key
            all_impls[self.key][1] = True

        if self.base_func in all_protos:
            info = all_protos[self.base_func]

            proto = None
            for ap in info:
                if proto is None:
                    proto = ap
                assert proto == ap
            self.proto = proto
            wext = self.func_info()
            if wext not in info[proto]:
                wext = "ANY"

            if wext not in info[proto] or "none" in info[proto][wext]:
                self.ulp = 0
            else:
                self.ulp = int(info[proto][wext])

        else:
            assert self.base_func in {
                "signbit", "roundevenf", "roundeven", "signbitf", "exp10f",
                "exp10"
            }

            Tstr = "double"
            if self.is_float:
                Tstr = "float"
            ret = "int"
            if "roundeven" in self.base_func or "exp10" in self.base_func:
                ret = Tstr

            self.proto = "{}|{}".format(ret, Tstr)
            self.ulp = 0

    def output_files(self):
        global all_replacements
        use_content = "/* Core .*/\n" + "\n".join(self.content)
        use_content = run_repls(use_content)

        for impl in ["dev", "glibc"]:
            core_name = self.core_dst_path.replace("[DEV-OR-GLIBC]", impl)
            core_f = open(core_name, "w+")
            c = use_content
            c = c.replace("[DEV-OR-GLIBC]", impl)
            c = c.replace("[DEV_OR_GLIBC]", impl)
            c += "\n"
            core_f.write(c)
            core_f.close()

            print(self.def_dst_path.replace("[DEV-OR-GLIBC]", impl))
            assert not os.path.isfile(
                self.def_dst_path.replace("[DEV-OR-GLIBC]", impl))
            def_f = open(self.def_dst_path.replace("[DEV-OR-GLIBC]", impl),
                         "w+")

            assert self.nentries <= 2
            if False:
                fmts = [
                    self.func_name,
                    self.func_name.replace(".S", "-wrapper.S")
                ]
                i = 0
                def_f.write("#define FUNCNAME{} {}\n".format(
                    i, fmts[i].replace("[DEV-OR-GLIBC]", impl)))
                if self.nentries == 2:
                    i = 1
                    def_f.write("#define FUNCNAME{} {}\n".format(
                        i, fmts[i].replace("[DEV-OR-GLIBC]", impl)))

            def_f.write("/* Def. */\n#include \"{}\"\n".format(core_name))
            def_f.close()

    def func_info(self):
        if self.stub_redirect:
            tsz = 8
            tcnt = int(int(self.cnt) / 1)
            if self.is_float:
                tsz = 4

            wext = ""
            sz = tsz * tcnt
            if sz <= 16:
                wext = "scalar"
            elif sz == 32:
                wext = "vlen{}".format(int(tcnt / 2))
            elif sz == 64:
                wext = "vlen{}".format(int(tcnt / 2))
            else:
                assert False

            return wext
        else:
            if self.cnt == 1:
                return "scalar"
            wext = "vlen{}".format(self.cnt)
            if self.is_float and self.cnt == 8:
                assert self.ext == "avx2"
                wext += "_avx2"
            elif (not self.is_float) and self.cnt == 4:
                assert self.ext == "avx2"
                wext += "_avx2"
            return wext

    def simple_proto(self):
        global unique_protos
        pieces = self.proto.split("|")
        out = []
        for i in range(0, len(pieces)):
            p = pieces[i]
            types = [
                "double complex", "float complex", "complex", "float",
                "double", "long int", "long long int", "int", "void", "int *",
                "float *", "double *", "const char *", "long double"
            ]
            types_sz = [
                "dc", "fc", "c", "f", "d", "li", "lli", "i", "v", "ip", "fp",
                "dp", "kcp", "ld"
            ]

            pout = ""
            for j in range(0, len(types)):
                if types[j] == p:
                    assert pout == ""
                    pout = types_sz[j]
            out.append(pout)
        unique_protos.add("_".join(out))
        return "_".join(out)

    def output_defines(self):
        assert "[DEV-OR-GLIBC].S" in self.func_name
        func_name_c = self.func_names[0].replace("-[DEV-OR-GLIBC]",
                                                 "").replace(
                                                     "_[DEV_OR_GLIBC]",
                                                     "").replace("-", "_")
        base_c = self.base_func
        cnt = self.cnt
        fp = 0
        sz = int(cnt)
        sz *= 8
        if self.is_float:
            fp = 1
            sz = int(sz / 2)

        defs = [
            "ADD_DEF({}, run_{}, {}, {}, {}, {}, {})".format(
                func_name_c, base_c, sz, cnt, fp, self.ulp,
                self.simple_proto())
        ]
        vprotos = []
        vprotos.append(
            get_vproto(self.proto, self.cnt, func_name_c, self.simple_proto(),
                       sz))

        assert self.nentries <= 2
        if self.nentries == 2:
            func_name_c = self.func_names[1].replace(
                "-[DEV-OR-GLIBC]",
                "-wrapper").replace("_[DEV_OR_GLIBC]",
                                    "_wrapper").replace("-", "_")
            vprotos.append(
                get_vproto(self.proto, self.cnt, func_name_c,
                           self.simple_proto(), sz))
            defs.append("ADD_DEF({}, run_{}, {}, {}, {}, {}, {})".format(
                func_name_c, base_c, sz, cnt, fp, self.ulp,
                self.simple_proto()))
        return vprotos, self.simple_proto(), defs


def is_define(line):
    line = line.lstrip().rstrip()
    return "#" in line and "define" in line


def is_include(line):
    line = line.lstrip().rstrip()
    return "#" in line and "include" in line


def is_section(line):
    line = line.lstrip().rstrip()
    return ".section" in line or ".text" in line


def is_entry(line):
    line = line.lstrip().rstrip()
    return line.startswith("ENTRY") and "(" in line and ")" in line


def is_end(line):
    line = line.lstrip().rstrip()
    return line.startswith("END") and "(" in line and ")" in line


def is_wrapper(line):
    line = line.lstrip().rstrip()
    return line.startswith("WRAPPER_")


def is_mwrapper(line):
    line = line.lstrip().rstrip()
    line = line.split()
    if len(line) < 2:
        return False
    return line[0] == ".macro" and is_wrapper(line[1])


def remove_at_idx(s, idx):
    return s[:idx] + s[idx + 1:]


class Include:

    def __init__(self, line):
        global impl_dir

        assert is_include(line)
        line = line.lstrip().rstrip()

        while "# " in line:
            line = line.replace("# ", "#")
        assert line.startswith("#include")
        assert line.count("#include") == 1
        line = line.replace("#include", "")
        line = line.lstrip().rstrip()

        assert line[0] == "<" or line[0] == "\""

        is_sys_include = line[0] == "<"

        if is_sys_include:
            assert line[-1] == ">"
            assert line.count("<") == 1
            assert line.count(">") == 1

            line = line[1:-1]
        else:
            assert line[-1] == "\""
            assert line.count("\"") == 2
            line = line[1:-1]

        if line.startswith("../"):
            assert line.count("../") == 1
            assert not is_sys_include
            line = line.replace("../", "stubs/")
            assert line.count("/") == 1
        elif not is_sys_include and (line.endswith(".h") or
                                     line.count("svml_s_tanhf_rodata.S") == 1):
            assert line.count("/") == 0
            line = "stubs/" + line
            assert line.count("/") == 1

        if is_sys_include:
            assert ".." not in line

        to_include = line
        if not is_sys_include:
            assert os.path.isfile(os.path.join(impl_dir, to_include))

        to_include = to_include.replace("stubs/", "")

        self.is_sys_include = is_sys_include
        self.to_include = to_include

    def is_stub(self):
        return not self.is_sys_include

    def replace_include(self):
        global stubs
        if self.is_stub():
            stub_fname = os.path.split(self.to_include)[-1]
            #assert stub_fname in stubs, stub_fname
            return "#include \"{}\"".format(self.to_include)
        return ""


def fixup_data_ref(line):
    global known_data_gotdefs
    assert "@GOTPCREL" in line
    assert line.count("@GOTPCREL") == 1
    end_pcrel = line.find("@GOTPCREL")
    assert end_pcrel != -1
    end_pcrel += len("@GOTPCREL")
    pcrel = line[:end_pcrel]

    end_defs = [pcrel.rfind(","), pcrel.rfind(" "), pcrel.rfind("\t")]
    end_def = max(end_defs)
    assert end_def != -1

    pcrel = pcrel[end_def + 1:]

    cur_pcrel = pcrel
    assert pcrel in known_data_gotdefs
    pcrel = pcrel.replace("@GOTPCREL", "")
    pcrel = pcrel.replace("__svml", "[DEV-OR-GLIBC]")
    pcrel = "GOTREF({})".format(pcrel)
    assert line.count(cur_pcrel) == 1

    return line.replace(cur_pcrel, pcrel)


def replace_arg(line, func_name, n, new_name):
    assert line.count(func_name) == 1
    start_func = line.find(func_name) + len(func_name)
    tmp_line = line[start_func:]

    start_arg = tmp_line.find("(")
    assert start_arg > -1
    start_arg += 1
    tmp_line = tmp_line[start_arg:]

    end_arg = tmp_line.rfind(")")
    assert end_arg > -1
    end_arg -= 1
    tmp_line = tmp_line[:end_arg]

    assert line.count(tmp_line) == 1
    return line.replace(tmp_line, "FUNCNAME{}".format(n)), tmp_line


def get_ext(fname_fmt):
    global extensions

    ext = None

    fname_pieces0 = fname_fmt.split("-")
    fname_pieces = []
    for piece in fname_pieces0:
        pieces = piece.split(".")
        for p in pieces:
            fname_pieces.append(p)

    for extension in extensions:
        if extension in fname_pieces:
            assert ext is None
            ext = extension
    assert ext is not None, fname_pieces
    if ext == "sse":
        return "sse2"

    return ext


def misc_dst(src_path, dst_fmt, fname_fmt, nentries, old_names, content):
    assert nentries == len(old_names)
    assert fname_fmt.startswith("s-")
    fname_fmt = fname_fmt[len("s-"):]
    fname_func_end = fname_fmt.find("-")
    assert fname_func_end != -1
    fname_func_end -= 1
    is_float = fname_fmt[fname_func_end] == "f"

    func_name = 0
    if is_float:
        func_name = fname_fmt[:fname_func_end] + 'f'
        fname_fmt = remove_at_idx(fname_fmt, fname_func_end)
    else:
        func_name = fname_fmt[:fname_func_end + 1]

    type_dir = "double"
    if is_float:
        type_dir = "float"

    fname_fmt = fname_fmt.replace("sse4-1", "sse4")
    fname_fmt = fname_fmt.replace("sse4-2", "sse4")
    fname_fmt = fname_fmt.replace("sse.S", "sse2.S")

    ext = get_ext(fname_fmt)

    red = ""
    fname_fmt = "{}-{}-{}-{}[DEV-OR-GLIBC].S".format(func_name, 1, ext, red)

    fname = fname_fmt.replace("-", "_").replace(".S", "")
    #    print("{} -> {}".format(fname_fmt, fname))
    #    assert False
    print(fname)

    repls = []
    func_names = []
    for old_name in old_names:
        old = old_name[1]
        if old != "":
            old = old + "_"
        repls.append(Replacement(old_name[0], fname.replace("[", old + "[")))
        func_names.append(fname.replace("[", old + "["))
    core_dst = os.path.join(dst_fmt, type_dir, "core", fname_fmt)
    def_dst = os.path.join(dst_fmt, type_dir, fname_fmt)

    return Outputter(src_path, def_dst, core_dst, fname_fmt, func_name,
                     func_names, nentries, is_float, ext, 1, None, True, False,
                     content)


def get_func_float_cnt(fname_fmt, is_float, no_ext, nentries):
    fname_fmt = fname_fmt.replace("sse4-1", "sse4")
    fname_fmt = fname_fmt.replace("sse4-2", "sse4")

    did_replace = False
    known_replacements = ["log10", "exp10", "log2", "exp2", "atan2", "expm1"]
    for rep in known_replacements:
        did_replace = did_replace or (rep in fname_fmt)
        unused = "QQQQQQQQ-"
        assert unused not in fname_fmt
        fname_fmt = fname_fmt.replace(rep + "-", unused)
        fname_fmt = fname_fmt.replace(rep, rep + "-")
        fname_fmt = fname_fmt.replace(unused, rep + "-")

    fname_fmt = fname_fmt.replace("-sse-", "-sse2-")
    ext = get_ext(fname_fmt)
    assert fname_fmt.count(ext) == 1, ext
    assert fname_fmt.count("-" + ext) == 1

    fname_start = fname_fmt.find("-" + ext)
    assert fname_start != -1

    fname = fname_fmt[:fname_start]

    i = -1
    while fname[i].isdigit():
        i -= 1
        assert len(fname) + i > 0

    cnt = fname[i + 1:]

    if is_float is None:
        is_float = fname[i] == "f"

        assert is_float

    red = ""
    if nentries == 0:
        red = "wrapped-"
    if is_float:
        fname = fname[:i]
        if fname[-1] == "-":
            fname = fname[:-1]
        fname += "f"
        fname_fmt = "{}-{}-{}-{}[DEV-OR-GLIBC].S".format(fname, cnt, ext, red)
    else:
        fname = fname[:i + 1]
        fname_fmt = "{}-{}-{}-{}[DEV-OR-GLIBC].S".format(fname, cnt, ext, red)
    if fname.endswith("-"):
        fname = fname[:-1]
        fname_fmt = "{}-{}-{}-{}[DEV-OR-GLIBC].S".format(fname, cnt, ext, red)
    print(fname_fmt, fname)
    return fname_fmt, fname, cnt, is_float, ext


def svml_dst(src_path, dst_fmt, fname_fmt, nentries, old_names, content):
    assert nentries == len(old_names)
    assert fname_fmt.startswith("svml-")
    fname_fmt = fname_fmt[len("svml-"):]
    assert fname_fmt.startswith("d-") or fname_fmt.startswith("s-")
    is_float = None
    if fname_fmt.startswith("d-"):
        is_float = False

    fname_fmt = fname_fmt[len("d-"):]
    assert fname_fmt.count("-core-") == 1
    fname_fmt = fname_fmt.replace("-core", "")

    fname_fmt, fname, cnt, is_float, ext = get_func_float_cnt(
        fname_fmt, is_float, False, nentries)

    sz = int(cnt)
    if is_float:
        sz *= 4
    else:
        sz *= 8
    assert sz in {16, 32, 64}, sz

    stub_redirect = sz != extensions[ext] or (nentries == 0)

    if stub_redirect:
        assert nentries == 0 or (nentries <= 2 and ext == "avx")

    if not stub_redirect:
        stub_redirect = None

    type_dir = "double"
    if is_float:
        type_dir = "float"

    func_name = fname_fmt.replace("-", "_").replace(".S", "")

    repls = []
    func_names = []
    for old_name in old_names:
        old = old_name[1]
        if old != "":
            old = old + "_"
        repls.append(
            Replacement(old_name[0], func_name.replace("[", old + "[")))
        func_names.append(func_name.replace("[", old + "["))
    if nentries == 0:
        assert stub_redirect
        has_def_then_incl = []
        for i in range(0, len(content)):
            line = content[i]
            if len(has_def_then_incl) == 0 and is_define(line):
                has_def_then_incl.append(i)
            if len(has_def_then_incl) == 1 and is_include(line):
                has_def_then_incl.append(i)

        assert len(has_def_then_incl) == 2
        def_line = content[has_def_then_incl[0]]
        incl_line = content[has_def_then_incl[1]]
        assert is_define(def_line)
        assert is_include(incl_line)
        print(func_name)
        print(def_line)
        assert len(def_line.split()) == 3
        repls.append(Replacement(def_line.split()[2], func_name))
        func_names = [func_name]

    assert nentries >= 0 and nentries <= 2
    assert len(cnt) >= 1 and len(cnt) <= 2, cnt

    core_dst = os.path.join(dst_fmt, type_dir, "core", fname_fmt)
    def_dst = os.path.join(dst_fmt, type_dir, fname_fmt)

    assert len(func_names) > 0
    if len(func_names) == 0:
        func_names = [func_name]

    return Outputter(src_path, def_dst, core_dst, fname_fmt, fname, func_names,
                     nentries, is_float, ext, cnt, stub_redirect, False, False,
                     content)


def proccess_file_content(fpath, dst_fmt, hdr, get_dst_f):
    src_path = fpath
    assert fpath.endswith(".S")
    fname = os.path.split(fpath)[-1]

    content = []
    has_include = 1000 * 1000 + 1
    has_section = 1000 * 1000 + 2
    has_define = 1000 * 1000 + 3
    has_entry = []
    has_end = []

    entries = []
    ends = []
    wrappers = []
    for line in open(fpath):
        content.append(line.rstrip())
    for i in range(0, len(content)):
        line = content[i]
        if is_entry(line):
            entries.append(i)
        if is_end(line):
            ends.append(i)
        if is_wrapper(line):
            wrappers.append(i)

    assert len(entries) == len(ends)
    assert sorted(entries) == entries
    assert sorted(ends) == ends

    if len(entries) > 1:
        ent_i = 0
        end_i = 0
        while True:
            if ent_i == end_i:
                if ent_i == len(entries):
                    break
                assert entries[ent_i] not in wrappers
                assert ends[end_i] not in wrappers

                assert entries[ent_i] != ends[end_i]
                assert entries[ent_i] < ends[end_i]
                ent_i += 1
            else:
                if ent_i != len(entries):
                    assert entries[ent_i] != ends[end_i]
                    assert entries[ent_i] > ends[end_i]
                end_i += 1
    for w_i in wrappers:
        okay = False
        for i in range(0, len(entries) - 1):
            if entries[i] < w_i and entries[i + 1] > w_i:
                okay = True
                assert ends[i] > w_i
                break
        if not okay:
            assert w_i > entries[-1] and w_i < ends[-1]
            okay = True

        assert okay

    if len(entries) - len(wrappers) > 1:
        assert len(entries) - len(wrappers) == 2
        assert (len(entries) - len(wrappers)) % 2 == 0
        skips = []
        for i in range(0, len(entries)):
            ent_line = content[entries[i]]
            end_line = content[ends[i]]

            assert is_entry(ent_line)
            assert is_end(end_line)

            assert ent_line.endswith("_knl)") == end_line.endswith("_knl)")

            if ent_line.endswith("_knl)"):
                skips.append([entries[i], ends[i]])

        if len(skips) != 0:
            tmp_content = []
            assert len(skips) > 0, "\n" + "\n".join(content)
            lb = skips[0][0]
            ub = skips[0][1]
            skip_idx = 1
            for i in range(0, len(content)):
                if i > lb and i < ub:
                    continue
                if i == lb:
                    continue
                if i == ub:
                    if skip_idx == len(skips):
                        lb = 1000 * 1000
                        ub = 1000 * 1000
                    else:

                        lb = skips[skip_idx][0]
                        ub = skips[skip_idx][1]
                        skip_idx += 1
                    continue
                tmp_content.append(content[i])

            assert lb == 1000 * 1000
            assert ub == 1000 * 1000
            content = copy.deepcopy(tmp_content)
            wrappers = []
            for i in range(0, len(content)):
                if is_wrapper(content[i]):
                    wrappers.append(i)
        else:
            pass
    includes = []
    for i in range(0, len(content)):
        line = content[i]
        if is_define(line):
            if has_define > (1000 * 1000):
                has_define = i

        if is_include(line):
            includes.append(Include(line))
            if has_include > (1000 * 1000):
                has_include = i
        if is_section(line):
            if has_section > (1000 * 1000):
                has_section = i

        if is_entry(line):
            has_entry.append(i)
            assert len(has_entry) - len(wrappers) <= 2
#            assert len(wrappers) > 0 or len(has_entry) == 1
        if is_end(line):
            has_end.append(i)
            assert len(has_end) - len(wrappers) <= 2


#            assert len(wrappers) > 0 or len(has_end) == 1

    assert len(has_entry) == len(has_end)

    has_stub = False
    for incl in includes:
        has_stub = has_stub or incl.is_stub()

    if len(has_entry) == 0 or len(has_entry) > 1:
        assert has_stub, "\n".join(content)

    if len(has_entry) != 0:
        if src_path.endswith("_avx.S"):
            assert len(has_entry) == len(wrappers)
        else:
            assert len(has_entry) > len(wrappers)

    assert len(has_entry) - len(wrappers) <= 2

    uniques = set()
    uniques.add(has_define)
    uniques.add(has_include)
    uniques.add(has_section)

    expec = 3
    if len(has_entry) != 0:
        uniques.add(has_entry[0])
        uniques.add(has_end[0])
        expec = 5

    add_def = min(uniques)

    assert len(uniques) == expec
    assert add_def < 1000 * 1000

    new_content = []

    fn_count = 0
    fn_replacements = []
    for i in range(0, len(content)):
        line = content[i]
        if i == add_def:
            new_content.append("#include \"{}\"".format(hdr))
            for j in range(0, len(has_entry)):
                break
                new_content.append("#ifndef FUNCNAME{}".format(j))
                new_content.append("#error \"No FUNCNAME{}\"".format(j))
                new_content.append("#endif")
        if is_include(line):
            incl = Include(line)
            line = incl.replace_include()

        elif is_entry(line):
            old_name = None
            assert line.count("(") == 1
            assert line.count(")") == 1
            pos_open = line.find("(") + 1
            assert line[pos_open:].count(")") == 1
            assert old_name is None
            old_name = line[pos_open:].replace(")", "").lstrip().rstrip()
            okay = 0
            name_ext = ""
            if old_name.startswith("_Z"):
                if old_name.endswith("_skx"):
                    #                    print("{} -> {}".format(old_name, old_name[:-4]))
                    pass
                assert get_dst_f == svml_dst
                assert old_name.startswith("_Z"), line
                assert old_name.count("N") == 1
                npos = old_name.find("N")
                npos += 1
                assert old_name[npos].isdigit()
                npos += 1
                if old_name[npos].isdigit():
                    npos += 1
                assert not old_name[npos].isdigit()
                assert "_" in old_name[npos:]
                name_ext = old_name[npos:]
                _pos = name_ext.find("_")

                name_ext = name_ext[1:_pos]
            else:
                assert get_dst_f == misc_dst
            if len(fn_replacements) >= 1 and False:
                name_ext += "_wrapped"
            fn_replacements.append([old_name, name_ext])

            #            line, replacement = replace_arg(line, "ENTRY", fn_count)
            #            fn_replacements.append(replacement)
            pass
        elif is_end(line):
            #            line, replacement = replace_arg(line, "END", fn_count)
            #            assert replacement == fn_replacements[-1]
            fn_count += 1
        elif is_wrapper(line):
            line = line.replace("_SSE2", "_SCALAR")
            tmp = line.split()
            assert is_wrapper(tmp[0])
            tmp[0] += "_[DEV-OR-GLIBC]"
            line = " ".join(tmp)
        elif is_mwrapper(line):
            line = line.replace("_SSE2", "_SCALAR")
            tmp = line.split()

            assert is_wrapper(tmp[1])
            tmp[1] += "_[DEV-OR-GLIBC]"
            line = " ".join(tmp)

        new_content.append(line)

    #assert len(fn_replacements) == len(has_entry)

    for i in range(0, len(new_content)):
        line = new_content[i]
        #for j in range(0, len(fn_replacements)):
        #    line = line.replace(fn_replacements[j], "FUNCNAME{}".format(j))
        if "@GOTPCREL" in line:
            line = fixup_data_ref(line)
        new_content[i] = line

    assert fname.endswith(".S")
    assert fname.count(".S") == 1
    fname = fname.replace(".S", "-[DEV-OR-GLIBC].S").replace("_", "-")
    return get_dst_f(src_path, dst_fmt, fname, len(has_entry), fn_replacements,
                     new_content)

for line in open(ulp_file):
    line = line.split(",")
    assert len(line) == 4
    all_protos.setdefault(line[0],
                          {}).setdefault(line[1],
                                         {}).setdefault(line[2], line[3])

assert os.path.exists(os.path.join(base_dir, "libc-asm-common.h"))

stubs_dir = []
for fname in os.listdir(impl_dir):
    if fname.endswith("~"):
        continue
    assert "#" not in fname
    if os.path.isdir(os.path.join(impl_dir, fname)):
        stubs_dir.append(fname)
        continue
    assert fname.endswith(".S"), fname
    if fname.startswith("svml_"):
        svml_impls.append(fname)
    else:
        misc_impls.append(fname)

assert len(stubs_dir) == 1
assert stubs_dir[0] == "stubs"
stubs_dir = "stubs"
for stub in os.listdir(os.path.join(impl_dir, stubs_dir)):
    no_core = False
    stub_name = stub
    assert stub_name.startswith("svml_")
    stub_name = stub_name[len("svml_"):]
    is_float = 0
    #    print(stub_name)
    if stub_name.startswith("d_"):
        stub_name = stub_name[2:]
        pass
    elif stub_name.startswith("s_"):
        stub_name = stub_name[2:]
        is_float = 1
    elif stub_name == "sd_wrapper_impl.h":
        stub_name = stub_name[3:]
        is_float = 2
    else:
        assert False

    ftype = None
    if stub_name.endswith(".h"):
        if stub_name.endswith("_data.h"):
            stub_name = "[DEV-OR-GLIBC]-" + stub_name
            assert stub_name.count("_data.h") == 1

            ftype = 0
        else:
            assert stub_name.endswith("wrapper_impl.h")
            stub_name = stub_name.replace("wrapper_impl.h",
                                          "common-wrapper-[DEV-OR-GLIBC].S")
            ftype = 1
    else:
        if stub_name.endswith("data.S"):
            assert stub_name.count("data.S") == 1
            if stub_name.count("_data.S") == 1:
                stub_name = stub_name.replace("data.S", "rodata.S")
            stub_name = "[DEV-OR-GLIBC]-" + stub_name
            no_core = True
            ftype = 2
        assert stub_name.endswith("_core.S") or stub_name.endswith(
            "_rodata.S"), stub_name
        if stub_name.endswith("_core.S"):
            stub_name = stub_name.replace("_core.S", "_wrapper.S")
            ftype = 3
    stub_name = stub_name.replace("_", "-")
    lines = []
    stub_f = os.path.join(impl_dir, stubs_dir, stub)
    repls = []
    new_name_base = None
    if ftype == 3:
        is_float = None
        if stub.startswith("svml_d"):
            is_float = False
        fname_fmt, fname, cnt, is_float, ext = get_func_float_cnt(
            stub_name.replace("wrapper", "avx"), is_float, False, 10)
        assert fname_fmt.count("-avx-") == 1
        print(fname_fmt)

        fname_fmt = fname_fmt.replace("-avx-", "-wrapper-")
        #        assert False, fname_fmt
        base_name = fname
        new_name_base = fname_fmt.replace(".S", "").replace("-", "_")
        stub_name = fname_fmt
        print("{} -> {}".format(stub_name, new_name_base))
        print("{} -> {}".format(stub, stub_name))
#        assert False

    assert os.path.isfile(stub_f)
    lines.append("// Ftype: {}".format(ftype))
    for line in open(stub_f):
        if is_include(line):
            incl = Include(line)
            line = incl.replace_include()

        if ftype == 0 or ftype == 2:
            for data_def in known_data_defs:
                line = line.replace("__svml", "[DEV-OR-GLIBC]")
        elif ftype == 1:
            line = line.replace("_SSE2", "_SCALAR")
            if is_wrapper(line):
                line = line.replace("_SSE2", "_SCALAR")
                tmp = line.split()

                assert is_wrapper(tmp[0])
                tmp[0] += "_[DEV-OR-GLIBC]"
                line = " ".join(tmp)
            if is_mwrapper(line):
                line = line.replace("_SSE2", "_SCALAR")
                tmp = line.split()

                assert is_wrapper(tmp[1])
                tmp[1] += "_[DEV-OR-GLIBC]"
                line = " ".join(tmp)
        elif ftype == 3:
            if is_entry(line):
                old_name = None
                assert line.count("(") == 1
                assert line.count(")") == 1
                pos_open = line.find("(") + 1
                assert line[pos_open:].count(")") == 1
                assert old_name is None
                old_name = line[pos_open:].replace(")", "").lstrip().rstrip()
                okay = 0
                assert old_name.startswith("_Z")
                assert old_name.count("N") == 1
                npos = old_name.find("N")
                npos += 1
                assert old_name[npos].isdigit()
                npos += 1
                if old_name[npos].isdigit():
                    npos += 1
                assert not old_name[npos].isdigit()
                assert "_" in old_name[npos:]
                name_ext = old_name[npos:]
                _pos = name_ext.find("_")

                name_ext = name_ext[1:_pos]

                repls.append(
                    Replacement(old_name,
                                new_name_base.replace("[", name_ext + "_["),
                                True))

            if is_wrapper(line):
                line = line.replace("_SSE2", "_SCALAR")
                tmp = line.split()

                assert is_wrapper(tmp[0])
                tmp[0] += "_[DEV-OR-GLIBC]"
                line = " ".join(tmp)
            if is_mwrapper(line):
                line = line.replace("_SSE2", "_SCALAR")
                tmp = line.split()

                assert is_wrapper(tmp[1])
                tmp[1] += "_[DEV-OR-GLIBC]"
                line = " ".join(tmp)

        lines.append(line.rstrip())
    #    repls.append(Replacement(stub, stub_name))
    stubs[stub] = Stub(os.path.join(impl_dir, stubs_dir,
                                    stub), stub, stub_name,
                       os.path.join(base_dir, "svml"), is_float, no_core,
                       lines)
#assert False, "OKAY"
misc_outs = []
svml_outs = []
for misc_impl in misc_impls:
    fpath = os.path.join(impl_dir, misc_impl)
    dst_fmt = os.path.join(base_dir, "misc")
    misc_outs.append(proccess_file_content(fpath, dst_fmt, libc_hdr, misc_dst))

#for func_name in all_impls:
#    assert all_impls[func_name][0] and all_impls[func_name][1]

#all_impls = {}

for svml_impl in svml_impls:
    print("Running: " + svml_impl)
    fpath = os.path.join(impl_dir, svml_impl)
    dst_fmt = os.path.join(base_dir, "svml")
    svml_outs.append(proccess_file_content(fpath, dst_fmt, libc_hdr, svml_dst))

outs_map = {}
for out in svml_outs + misc_outs:
    outs_map[out.func_name] = out

protos = []
adddefs = {}

for out in outs_map:
    p, sp, d = outs_map[out].output_defines()
    protos += p
    adddefs.setdefault(sp, [])
    adddefs[sp] += d

    outs_map[out].output_files()

for stub in stubs:
    pass
    stubs[stub].output()

print("\n".join(protos))
print("\n".join(adddefs))
for proto in unique_protos:
    #    print(proto)
    pass

for proto in adddefs:
    print("/* Proto: {}.  */".format(proto))
    print(",\n".join(adddefs[proto]) + ",")

#for proto in all_unique_vprotos:
#    print(proto)
#    print(proto[proto.find("*") + 1:proto.find(")")])
for out in outs_map:
    if outs_map[out].stub_redirect:
        pass
#        print(outs_map[out].func_name)

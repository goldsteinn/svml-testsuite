import os
import sys

import copy

mans = {}
for line in open("funcs.txt"):
    func = line.lstrip().rstrip()
    path = "{}.man".format(func)
    assert os.path.exists(path)

    lines = []
    f = open(path, "rb")
    index = f.read()
    f.close()
    index = index.decode("utf-8", errors='ignore')
    lines = index.split("\n")

    include_at = None
    compls = None
    link = None
    for i in range(0, len(lines)):
        line = lines[i].lstrip().rstrip()
        if line == "#include &lt;math.h&gt;":
            if include_at is None:
                include_at = i
        elif line == "#include &lt;complex.h&gt;":
            if compls is None:
                compls = i

        if line == "Link with <b><i>-lm.":
            if link is None:
                link = i

    assert link is not None
    assert (include_at is not None) or (compls is not None)

    key = "math"
    idx = include_at
    if include_at is None:
        key = "complex"
        idx = compls
    link = int(link)
    idx = int(idx)
    assert link > idx
    assert link - idx < 100

    fdef = None
    expec_def = " {}(".format(func)
    for i in range(idx, link):
        line = lines[i]
        if expec_def in line:
            assert ");" in line
            fdef = line.lstrip().rstrip()
            break

    mans.setdefault(key, {}).setdefault(func, fdef)

defs = {}
sortables = []
for k0 in mans:
    for func in mans[k0]:
        fdef = mans[k0][func]
        fdef = fdef.replace("float complex", "cfloat_t").replace(
            "double complex", "cdouble_t").replace("complex", "cdouble_t")

        pos = fdef.find(func)
        ret = fdef[:pos]
        rest = fdef[pos:]

        assert rest.startswith(func + "(")
        assert rest.endswith(");")
        rest = rest[len(func + "("):-len(");")]

        ret = ret.lstrip().rstrip()
        types = [
            "cdouble_t", "cfloat_t", "float", "double", "long int",
            "long long int", "int", "void", "int *", "float *", "double *",
            "const char *", "long double"
        ]

        rest = rest.split(",")
        assert ret in types

        best_matches = []
        best_match = ""
        for arg in rest:
            arg = arg.lstrip().rstrip()
            for T in types:
                if arg.startswith(T) and len(arg) > len(T):
                    if arg[:len(T)] != T:
                        continue

                    assert len(T) != len(best_match)
                    if len(T) > len(best_match):
                        best_match = T
            assert best_match != "", arg
            best_matches.append(best_match.lstrip().rstrip())
            best_match = ""
        assert func not in defs
        defs[func] = [ret] + best_matches
        sortables.append("-".join(defs[func] + ["ZZZ" + func]))
sortables.sort()

all_vnames = set()
for s in sortables:
    func = s.split("-")[-1][3:]
    assert func in defs, func
    types = defs[func]

    assert len(types) != 0
    #    print(func)
    #    print(str(types))

    bad = "die(\"Unimplemented!\");"
    is_bad = False
    mpfrs = []
    for t in types:
        if t == "float":
            mpfrs.append("flt")
        elif t == "double":
            mpfrs.append("d")
        elif t == "float *":
            mpfrs.append("flt")
        elif t == "long double":
            mpfrs.append("ld")
        elif t == "int" or t == "long" or t == "long int" or t == "long long int":
            mpfrs.append("si")
        elif t == "unsigned int" or t == "unsigned long" or t == "unsigned":
            mpfrs.append("ui")
        else:
            is_bad = True
    args = types[1:]
    for i in range(0, len(args)):
        args[i] = args[i] + " " + "arg{}".format(i)
    func_def = "static {} run_mpfr_{}({})".format(types[0], func,
                                                  ",".join(args))
    content = []

    if is_bad:
        for i in range(0, len(args)):
            content.append("(void)(arg{});".format(i))
        content.append("die(\"Unimplemented!\");")
    else:
        cnts = {}
        vnames = []
        content = []
        for i in range(0, len(types)):
            vname = "G_{}_op{}_".format(mpfrs[i], "{}")
            if vname not in cnts:
                cnts[vname] = 0
            cnt = cnts[vname]
            cnts[vname] += 1
            vname = vname.format(cnt)
            all_vnames.add(vname)
            vname = vname.format(cnt)
            if i != 0:
                content.append("mpfr_set_{} ({}, arg{}, MPFR_RNDN);".format(
                    mpfrs[i], vname, i - 1))
            vnames.append(vname)

        f = func
        if f[-1] == "f" and "float " in " ".join(types):
            f = f[:-1]
        content.append("mpfr_{} ({}, MPFR_RNDN);".format(f, ",".join(vnames)))
        content.append("return mpfr_get_{} ({}, MPFR_RNDN);".format(mpfrs[0], vnames[0]))
    print(func_def + "{" + "\n".join(content) + "}")

print("\n".join(["extern __thread mpfr_t {};".format(x) for x in all_vnames]))
sys.exit(0)
ulps = {}

func = None
ext = None
for line in open("ulp.txt"):
    line = line.lstrip().rstrip()
    if line.startswith("Function:"):

        assert func is None
        assert ext is None
        assert line.count("\"") == 2
        func = line.split("\"")[1]
        ext = None
        if func.count("_") != 0:
            ext = "_".join(func.split("_")[1:])
            func = func.split("_")[0]
            assert func in ulps
        else:
            ext = "scalar"
        if "Imaginary" in line:
            ext += "-i"
        elif "Real" in line:
            ext += "-r"
        if func in ulps:
            assert ext not in ulps[func], ulps
        ulps.setdefault(func, {}).setdefault(ext, {})

    elif line == "":
        func = None
        ext = None
    elif line.startswith("#"):
        continue
    else:
        hit = False
        T = None
        for types in ["float", "double", "float128", "ldouble"]:
            if line.split(":")[0] == types:
                T = types
                break

        assert func is not None
        assert ext is not None
        assert func in ulps
        assert ext in ulps[func]
        ulp = int(line.split(":")[1])
        assert T not in ulps[func][ext], T
        ulps[func][ext][T] = ulp

for func in defs:
    pass
#    print("{} -> {}".format(func, ",".join(defs[func])))

#print(ulps)

for func in defs:
    proto = defs[func]

    has = False
    bfunc = func
    if bfunc not in ulps:
        if bfunc[-1] == 'f':
            if bfunc[:-1] in defs:
                bfunc = bfunc[:-1]
    good = False
    if bfunc in ulps:
        rets = [proto[0]]
        for i in range(1, len(proto)):
            if "*" in proto[i]:
                if proto[i].startswith("double") or proto[i].startswith(
                        "float"):
                    rets.append(proto[i].replace("*", "").lstrip().rstrip())

        for ext in ulps[bfunc]:
            for ret in rets:
                if ret in ulps[bfunc][ext]:
                    print("{},{},{},{}".format(func, "|".join(proto), ext,
                                               ulps[bfunc][ext][ret]))
                    good = True
                    break
    if not good:
        print("{},{},ANY,{}".format(func, "|".join(proto), "none"))

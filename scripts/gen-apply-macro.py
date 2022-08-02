#! /usr/bin/env python3

import sys
from datetime import datetime

N = 256


class Macro():

    def __init__(self, text):
        if isinstance(text, list):
            text = " ".join(text)

        self.text = text

    def define(self):
        return "#define " + self.text


def gen_PP_NARG():
    macros = []
    TO_N = N + 2

    macros.append("PP_NARG(...) I_PP_NARG_(__VA_ARGS__, I_PP_RSEQ_N())")
    macros.append("I_PP_NARG_(...) I_PP_ARG_N(__VA_ARGS__)")

    text = []
    text.append("I_PP_ARG_N(")
    text.append(",".join(["_{}".format(x) for x in range(1, TO_N)]))
    text.append(", N,...) N")

    macros.append(text)

    text = []
    text.append("I_PP_RSEQ_N()")
    text.append(",".join(["{}".format(TO_N - x - 1) for x in range(0, TO_N)]))

    macros.append(text)

    return macros


def gen_NOT_ONE_NARG():
    macros = []
    TO_N = N + 2
    macros.append(
        "NOT_ONE_NARG(...) I_PP_NARG_(__VA_ARGS__,I_NOT_ONE_RSEQ_N())")

    text = []
    text.append("I_NOT_ONE_RSEQ_N()")
    text.append(",".join(["{}".format("MANY") for x in range(0, TO_N - 2)]))
    text.append(",ONE,ONE")

    macros.append(text)

    return macros


def gen_NOT_TWO_NARG():
    macros = []
    TO_N = N + 2
    macros.append(
        "NOT_TWO_NARG(...) I_PP_NARG_(__VA_ARGS__,I_NOT_TWO_RSEQ_N())")

    text = []
    text.append("I_NOT_TWO_RSEQ_N()")
    text.append(",".join(["{}".format("MANY") for x in range(0, TO_N - 3)]))
    text.append(",TWO,TWO,TWO")

    macros.append(text)

    return macros


def gen_IS_EMPTY():
    macros = []
    TO_N = N + 2

    macros.append("IS_EMPTY(...) I_IS_EMPTY_(DEPAREN(__VA_ARGS__))")
    macros.append("I_IS_EMPTY_TRIGGER_PARENTHESIS_(...) ,")
    macros.append("IS_EMPTY_PASTE5(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4")

    text = []
    text.append("I_IS_EMPTY_RESULT(_0, _1, _2, _3)")
    text.append(
        "I_IS_EMPTY_HAS_COMMA(IS_EMPTY_PASTE5(I_IS_EMPTY_RESULT_CASE_, _0, _1, _2, _3))"
    )
    macros.append(text)

    macros.append("I_IS_EMPTY_RESULT_CASE_0001 ,")

    text = []
    text.append("I_IS_EMPTY_ARG64(")
    text.append(",".join(["_{}".format(x) for x in range(0, TO_N)]))
    text.append(", ...)")
    text.append("_{}".format(TO_N - 1))

    macros.append(text)

    text = []
    text.append("I_IS_EMPTY_HAS_COMMA(...)")
    text.append("I_IS_EMPTY_ARG64(__VA_ARGS__,")
    text.append(",".join(["{}".format("1") for x in range(0, TO_N - 2)]))
    text.append(", 0)")

    macros.append(text)

    text = []
    text.append("I_IS_EMPTY_(...)")
    text.append("I_IS_EMPTY_RESULT(")
    text.append("I_IS_EMPTY_HAS_COMMA(__VA_ARGS__),")
    text.append(
        "I_IS_EMPTY_HAS_COMMA(I_IS_EMPTY_TRIGGER_PARENTHESIS_ __VA_ARGS__),")
    text.append("I_IS_EMPTY_HAS_COMMA(__VA_ARGS__(/* empty.  */)),")
    text.append(
        "I_IS_EMPTY_HAS_COMMA(I_IS_EMPTY_TRIGGER_PARENTHESIS_ __VA_ARGS__(/* empty.  */))"
    )
    text.append(")")

    macros.append(text)

    return macros


def I_gen_apply_recurse_level(vals):
    pairs = []
    for i in range(0, len(vals) - 1, 2):
        pairs.append("m({}, {})".format(vals[i], vals[i + 1]))
    if len(vals) % 2 == 1:
        pairs.append(vals[-1])
    return pairs


def gen_apply_recurse_level(bound, factor):
    assert bound >= 2

    unroll = bound % factor
    if unroll == 0:
        unroll = factor

    vals = ["x{}".format(x) for x in range(0, unroll)]
    args = ["m"] + vals
    while len(vals) > 1:
        vals = I_gen_apply_recurse_level(vals)

    assert len(vals) == 1

    vals = vals[0]
    if bound > factor:
        args.append("...")
        vals = "m({}, APPLY_RECURSE_{}(m, __VA_ARGS__))".format(
            vals, bound - unroll)

    out = "APPLY_RECURSE_{}({})".format(bound, ",".join(args))

    return out + vals


def gen_APPLY_RECURSE():
    macros = []

    macros.append(
        "APPLY_RECURSE(macro, ...) CAT_BASE(APPLY_RECURSE, IS_EMPTY(__VA_ARGS__))(macro, __VA_ARGS__)"
    )

    macros.append("APPLY_RECURSE1(macro, ...)")
    macros.append(
        "APPLY_RECURSE0(macro, ...) CAT_BASE(APPLY_RECURSE_, PP_NARG(__VA_ARGS__))(macro, __VA_ARGS__)"
    )

    for i in range(2, N + 1):
        macros.append(gen_apply_recurse_level(i, 8))

    return macros


def gen_arg_list(base_name, i):
    return ",".join([base_name + "{}".format(x) for x in range(0, i)])


def gen_applier(base_name, base_args, sep, gen_todo):
    macros = []

    for i in range(1, N + 1):
        xargs = gen_arg_list("x", i)

        todo = " {} ".format(sep).join([gen_todo(x) for x in range(0, i)])

        macros.append("{}_{}({}, {}) {}".format(base_name, i, base_args, xargs,
                                                todo))
    return macros


def gen_apply_preamble(name):
    macros = []
    macros.append(
        "{}(macro, OP, ...) CAT_BASE({}, IS_EMPTY(__VA_ARGS__))(macro, FWD_TOKEN(OP), __VA_ARGS__)"
        .format(name, name))
    macros.append("{}1(macro, OP, ...)".format(name))
    macros.append(
        "{}0(macro, OP, ...) CAT_BASE(APPLY_, PP_NARG(__VA_ARGS__))(macro, OP, __VA_ARGS__)"
        .format(name))
    return macros


def gen_APPLY():
    macros = []
    macros += gen_apply_preamble("APPLY")
    macros += gen_apply_preamble("APPLY_TOO")
    macros += gen_apply_preamble("APPLY2")

    macros += gen_applier("APPLY", "m, OP", "OP", lambda x: "m(x{})".format(x))
    return macros


def gen_packer(right):
    r_or_l = "L"
    gen_todo = lambda x: "(argp, x{})".format(x)
    if right:
        r_or_l = "R"
        gen_todo = lambda x: "(x{}, argp)".format(x)

    macros = []
    macros.append(
        "APPLY_PACK{}(argp, ...) CAT_BASE(APPLY_PACK{}, IS_EMPTY(__VA_ARGS__))(argp, __VA_ARGS__)"
        .format(r_or_l, r_or_l))
    macros.append("APPLY_PACK{}1(argp, ...)".format(r_or_l))

    macros.append(
        "APPLY_PACK{}0(argp, ...) CAT_BASE(APPLY_PACK{}_, PP_NARG(__VA_ARGS__))(argp, __VA_ARGS__)"
        .format(r_or_l, r_or_l))

    macros += gen_applier("APPLY_PACK{}".format(r_or_l), "argp", ",", gen_todo)

    return macros


def gen_ARGP_APPLY():
    macros = []
    macros.append(
        "ARGP_APPLY(argp, OP, ...) CAT_BASE(ARGP_APPLY, IS_EMPTY(__VA_ARGS__))(argp, FWD_TOKEN(OP), __VA_ARGS__)"
    )
    macros.append("ARGP_APPLY1(argp, OP, ...)")
    macros.append(
        "ARGP_APPLY0(argp, OP, ...) CAT_BASE(ARGP_APPLY_, PP_NARG(__VA_ARGS__))(argp, OP, __VA_ARGS__)"
    )

    macros += gen_applier("ARGP_APPLY", "argp, OP", "OP",
                          lambda x: "x{}(argp)".format(x))

    return macros


def gen_APPLY_COMBINE():
    macros = []
    macros.append(
        "APPLY_COMBINE(...) CAT_BASE(APPLY_COMBINE, IS_EMPTY(__VA_ARGS__))(__VA_ARGS__)"
    )
    macros.append("APPLY_COMBINE1(...)")
    macros.append(
        "APPLY_COMBINE0(...) CAT_BASE(APPLY_COMBINE_, NOT_TWO_NARG(__VA_ARGS__))(__VA_ARGS__)"
    )
    macros.append(
        "APPLY_COMBINE_TWO(x, y) APPLY_COMBINE_MANY_(DEPAREN(x), DEPAREN(y))")
    macros.append("APPLY_COMBINE_MANY_(...) APPLY_COMBINE_MANY(__VA_ARGS__)")
    macros.append(
        "APPLY_COMBINE_MANY(...) CAT_BASE(APPLY_COMBINE_, PP_NARG(__VA_ARGS__))(__VA_ARGS__)"
    )

    for i in range(1, int(N / 2) + 1):
        xargs = gen_arg_list("x", i)
        yargs = gen_arg_list("y", i)
        args = xargs + "," + yargs

        todo = ",".join(["(x{}, y{})".format(x, x) for x in range(0, i)])
        macros.append("APPLY_COMBINE_{}({}) {}".format(i * 2, args, todo))

    return macros


def gen_PACKR():
    return gen_packer(True)


def gen_PACKL():
    return gen_packer(False)


def gen_info():
    print("/* Generated\n *\tBy: {}\n *\tOn: {}\n */\n".format(
        sys.argv[0],
        datetime.today().strftime('%Y-%m-%d %H:%M:%S')))


def tests():
    print("#include \"util/macro.h\"")
    print("#include \"test/test-common.h\"")
    print("#define I_ONE 1")
    print("#define I_TWO 2")
    print("#define I_MANY 3")

    print("static int64_t count; static void foo(int64_t i) { count += i; }")

    print("static void foo2(int64_t i0, int64_t i1) { count += (i0 - i1); }")
    print("#define dfoo2(...) foo2(DEPAREN(__VA_ARGS__))")

    print("static void foo22(int64_t i0, int64_t i1) { count += (i0 + i1); }")
    print("#define dfoo22(...) foo22(DEPAREN(__VA_ARGS__))")

    print(
        "static uint64_t foo23(int64_t i0, int64_t i1) { return (i0 * i1); }")
    print("#define dfoo23(...) foo23(DEPAREN(__VA_ARGS__))")

    print(
        "#define I_TEST_MIN(x, y) ({ long _x = (x); long _y = (y); ((_x) < (_y) ? (_x) : (_y)); })"
    )
    for i in range(0, N):
        print("#define I_MUL{}(x) ((x) * {}L)".format(i, i))

    print("#define I_MMUL(x) CAT(I_MUL, x)")

    print("int32_t test_generated_macros() {")

    print("#define INDIRECT_APPLY(m, op, ...) APPLY(m, op, __VA_ARGS__)")
    print("#define I_PLUS(x, y) ((x) + (y))")

    todo = []
    for i in range(0, min(16, N)):
        todo.append(i)

    i = 16
    while i < N:
        todo.append(i)
        i += i

    todo.append(N - 10)
    todo.append(N - 2)
    todo.append(N - 1)
    todo.append(N)

    for i in todo:
        li = ",".join([str(x) for x in range(0, i)])
        li2 = ",".join([str(x + 1)
                        for x in range(0, i)] + [str(x) for x in range(0, i)])
        si = sum(x for x in range(0, i))

        mul_sum = 0
        for x in range(0, i):
            mul_sum += (x + (x + 1))

        xor_sum = 0
        for x in range(0, i):
            xor_sum ^= (x * (x + 1))

        xor_sum2 = 0
        for x in range(0, i):
            xor_sum2 ^= (x * x)

        if i > 0:
            print("test_assert(PP_NARG({}) == {});".format(li, i))

        NON_res = "I_MANY"
        NOT_res = "I_MANY"
        if i < 2:
            NON_res = "I_ONE"
        if i < 3:
            NOT_res = "I_TWO"

        print("test_assert(CAT(I_, NOT_ONE_NARG({})) == {});".format(
            li, NON_res))
        print(
            "test_assert(CAT(I_, NOT_TWO_NARG({})) == {}, \"Fail({}: !=%d)\", {});"
            .format(li, NOT_res, i, NOT_res))

        IE_res = "0"
        if i == 0:
            IE_res = "1"

        print("test_assert(IS_EMPTY({}) == {});".format(li, IE_res))

        if i >= 2:
            print(
                "test_assert(strcmp(V_TO_STR(CAT({})), \"{}\") == 0);".format(
                    li, "".join([str(x) for x in range(0, i)])))
            print("test_assert(APPLY_RECURSE(I_TEST_MIN, {}) == {});".format(
                li, 0))
            print("test_assert(APPLY_RECURSE(I_PLUS, {}) == {});".format(
                li, si))

        if i <= int(N / 2):
            print(
                "count = 0; APPLY(dfoo22, ;, APPLY_COMBINE({})); test_assert(count == {}, \"Fail({}) Combine 1: %lu != %lu\", count, {}UL);"
                .format(li2, mul_sum, i, mul_sum))
            print(
                "count = 0; APPLY(dfoo2, ;, APPLY_COMBINE({})); test_assert(count == {},\"Fail({}) Combine 2\");"
                .format(li2, i, i))

            if i != 0:

                print(
                    "test_assert((APPLY(dfoo23, ^, APPLY_COMBINE({}))) == {}, \"Fail({}) Combine 0: %lu != %lu\", count, {}UL);"
                    .format(li2, xor_sum, i, xor_sum))

                print(
                    "count = 0; APPLY(dfoo22, ;, APPLY_COMBINE(({}), ({}))); test_assert(count == {}, \"Fail({}) Combine 4: %lu != %lu\", count, {}UL);"
                    .format(li, li, si * 2, i, si * 2))
                print(
                    "count = 0; APPLY(dfoo2, ;, APPLY_COMBINE(({}), ({}))); test_assert(count == {},\"Fail({}) Combine 2\");"
                    .format(li, li, 0, i))
                print(
                    "test_assert((APPLY(dfoo23, ^, APPLY_COMBINE(({}), ({})))) == {}, \"Fail({}) Combine 0: %lu != %lu\", count, {}UL);"
                    .format(li, li, xor_sum2, i, xor_sum2))

            if False:
                print(
                    "count = 0; APPLY(dfoo22, ;, APPLY_COMBINE(({}))); test_assert(count == {},\"Fail({}) Combine 3\");"
                    .format(li2, mul_sum, i))
                print(
                    "count = 0; APPLY(dfoo2, ;, APPLY_COMBINE(({}))); test_assert(count == {}, \"Fail({}) Combine 4\");"
                    .format(li2, i, i))

        if i == 0:
            print("APPLY(a, b);")
            print("APPLY_TOO(a, b);")
            print("APPLY2(a, b);")
            print("ARGP_APPLY(a, b);")
            print("APPLY_PACKR(a);")
            print("APPLY_PACKL(a);")
        else:
            print("test_assert(APPLY(I_MUL2, +, {}) == {});".format(
                li, si * 2))
            print("test_assert(APPLY_TOO(I_MUL2, +, {}) == {});".format(
                li, si * 2))
            print("test_assert(APPLY2(I_MUL2, +, {}) == {});".format(
                li, si * 2))
            print("test_assert(ARGP_APPLY(2, +, {}) == {});".format(
                ",".join(["I_MUL{}".format(x) for x in range(0, i)]), si * 2))

            print(
                "test_assert(ARGP_APPLY(2, +, APPLY(I_MMUL, COMMA, {})) == {});"
                .format(li, si * 2))

            print(
                "test_assert(ARGP_APPLY(2, +, APPLY_TOO(I_MMUL, COMMA, {})) == {});"
                .format(li, si * 2))
            print(
                "test_assert(ARGP_APPLY(2, +, APPLY2(I_MMUL, COMMA, {})) == {});"
                .format(li, si * 2))

            print("count = 0;")
            print("APPLY(foo, ;, {}); test_assert(count == {});".format(
                li, si))

            print("count = 0;")
            print("APPLY_TOO(foo, ;, {}); test_assert(count == {});".format(
                li, si))
            print("count = 0;")
            print("APPLY2(foo, ;, {}); test_assert(count == {});".format(
                li, si))
            print("count = 0;")
            print(
                "APPLY(foo, ;, ARGP_APPLY(2, COMMA, {})); test_assert(count == {});"
                .format(",".join(["I_MUL{}".format(x) for x in range(0, i)]),
                        si * 2))

            print("count = 0;")
            print(
                "APPLY(dfoo2, ;, APPLY_PACKR(100L, {})); test_assert(count == {});"
                .format(li, si - 100 * i))

            print("count = 0;")
            print(
                "APPLY(dfoo2, ;, APPLY_PACKL(100L, {})); test_assert(count == {});"
                .format(li, 100 * i - si))

    print("return 0; }")


def gen():
    out = []

    out.append(gen_PP_NARG())
    out.append(gen_NOT_ONE_NARG())
    out.append(gen_NOT_TWO_NARG())
    out.append(gen_IS_EMPTY())
    out.append(gen_APPLY_RECURSE())
    out.append(gen_APPLY())
    out.append(gen_ARGP_APPLY())
    out.append(gen_PACKR())
    out.append(gen_PACKL())
    out.append(gen_APPLY_COMBINE())

    print("#ifndef _SRC__UTIL__INTERNAL__GENERATED_MACRO_H_")
    print("#define _SRC__UTIL__INTERNAL__GENERATED_MACRO_H_")
    print("")
    gen_info()
    print("/* clang-format off */")
    print("")
    for mpack in out:
        print(
            "/********************************************************************/"
        )
        mpack = [Macro(x) for x in mpack]
        for m in mpack:
            print(m.define(), end="\n\n")
        print("\n\n")

    print("")
    print("/* clang-format on */")
    print("")
    print("#endif")


if len(sys.argv) == 1:
    gen()
else:
    tests()

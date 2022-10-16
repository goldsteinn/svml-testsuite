#! /usr/bin/env python3
import sys
from datetime import datetime
attrs = [
    "noclone", "always_inline", "noinline", "malloc", "const", "pure",
    "unused", ["tls_model", "..."], "cold", "noreturn", ["aligned", "..."],
    ["nonnull", "..."], ["format", "..."], ["optimize", "..."],
    ["alloc_size", "..."]
]


def gen_hdr():
    hdr = "#ifndef _SRC__UTIL__ATTR_PORTABILITY_H_\n#define _SRC__UTIL__ATTR_PORTABILITY_H_\n\n"
    hdr += "#include \"util/portability.h\""
    hdr += "/* Generated\n *\tBy: {}\n *\tOn: {}\n */\n".format(
        sys.argv[0],
        datetime.today().strftime('%Y-%m-%d %H:%M:%S'))
    return hdr


def gen_tail():
    tail = "\n#endif\n"
    return tail


def gen_attr_check(attr):
    attr_name = None
    attr_args_in = ""
    attr_args_out = ""

    if isinstance(attr, list):
        attr_name = attr[0]
        attr_args_in = "(" + attr[1] + ")"
        attr_args_out = "(__VA_ARGS__)"
    else:
        attr_name = attr

    check = "#if I_has_attr({})\n#define I_attr_{}{} __attribute__({}{})\n#else\n#define I_attr_{}{}\n#endif\n"

    return check.format(attr_name, attr_name, attr_args_in, attr_name,
                        attr_args_out, attr_name, attr_args_in)


out = [gen_hdr()]
for attr in attrs:
    out.append(gen_attr_check(attr))
out.append(gen_tail())
print("\n".join(out))

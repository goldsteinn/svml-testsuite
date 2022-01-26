errs = [
    "sincos_8_avx512_vv_wrapper_dev", "sincos_8_avx512_vv_wrapper_glibc",
    "sincos_2_sse2_wrapped_dev", "sincos_2_sse4_vv_wrapper_dev",
    "sincos_2_sse4_vv_wrapper_glibc", "sincos_4_avx2_vv_wrapper_dev",
    "sincos_4_avx2_vv_wrapper_glibc", "sincos_4_avx_vv_wrapper_dev",
    "sincos_4_avx_vv_wrapper_glibc"
]

for line in errs:
    fname = line.replace("_", "-") + ".S"
    lines = []

    pieces = line.split("_")
    func = pieces[0]
    n = pieces[1]
    ext = pieces[2]
    owner = pieces[-1]

    lines.append("#include \"/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h\"")
    lines.append("#define {}_{}_wrapper_vv_{} {}".format(func, n, owner, line))
    lines.append("#include \"/home/noah/programs/projects/svml-new/src/asm/svml/double/core/stubs/sincos-{}-wrapper-dev.S\"".format(n))
    fname = open(fname, "w+")
    fname.write("\n".join(lines) + "\n")
    fname.close()

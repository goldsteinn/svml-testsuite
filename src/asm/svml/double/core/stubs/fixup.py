import os

for f in os.listdir("."):
    if not f.startswith("sincos"):
        continue
    lines = []
    for line in open(f):
        if "sincos_2_wrapper_l8l8_dev" in line:
            print("{} -> ".format(line))
            assert line.count("sincos_2_wrapper_l8l8_dev") == 1
            line = line.replace("sincos_2_wrapper_l8l8_dev",
                                "sincos_2_sse2_wrapped_l8l8_dev")
            print("\t{}".format(line))
        if "sincos_4_wrapper_l8l8_dev" in line:
            print("{} -> ".format(line))
            assert line.count("sincos_4_wrapper_l8l8_dev") == 1
            line = line.replace("sincos_4_wrapper_l8l8_dev",
                                "sincos_4_avx2_wrapped_l8l8_dev")
            print("\t{}".format(line))
        if "sincos_8_wrapper_l8l8_dev" in line:
            print("{} -> ".format(line))
            assert line.count("sincos_8_wrapper_l8l8_dev") == 1
            line = line.replace("sincos_8_wrapper_l8l8_dev",
                                "sincos_8_avx512_wrapped_l8l8_dev")
            print("\t{}".format(line))
        if "sincos_2_wrapper_l8l8_glibc" in line:
            print("{} -> ".format(line))
            assert line.count("sincos_2_wrapper_l8l8_glibc") == 1
            line = line.replace("sincos_2_wrapper_l8l8_glibc",
                                "sincos_2_sse2_wrapped_l8l8_glibc")
            print("\t{}".format(line))
        if "sincos_4_wrapper_l8l8_glibc" in line:
            print("{} -> ".format(line))
            assert line.count("sincos_4_wrapper_l8l8_glibc") == 1
            line = line.replace("sincos_4_wrapper_l8l8_glibc",
                                "sincos_4_avx2_wrapped_l8l8_glibc")
            print("\t{}".format(line))
        if "sincos_8_wrapper_l8l8_glibc" in line:
            print("{} -> ".format(line))
            assert line.count("sincos_8_wrapper_l8l8_glibc") == 1
            line = line.replace("sincos_8_wrapper_l8l8_glibc",
                                "sincos_8_avx512_wrapped_l8l8_glibc")
            print("\t{}".format(line))
        lines.append(line)
    f_out = open(f, "w+")
    f_out.write("".join(lines))
    f_out.close()

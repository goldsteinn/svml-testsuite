import os

for f in os.listdir("."):
    if not f.startswith("sincos"):
        continue
    lines = []
    for line in open(f):
        if "sincosf_4_wrapper_l4l4_dev" in line:
            print("{} -> ".format(line))
            assert line.count("sincosf_4_wrapper_l4l4_dev") == 1
            line = line.replace("sincosf_4_wrapper_l4l4_dev",
                                "sincosf_4_sse2_wrapped_l4l4_dev")
            print("\t{}".format(line))
        if "sincosf_8_wrapper_l4l4_dev" in line:
            print("{} -> ".format(line))
            assert line.count("sincosf_8_wrapper_l4l4_dev") == 1
            line = line.replace("sincosf_8_wrapper_l4l4_dev",
                                "sincosf_8_avx2_wrapped_l4l4_dev")
            print("\t{}".format(line))
        if "sincosf_16_wrapper_l4l4_dev" in line:
            print("{} -> ".format(line))
            assert line.count("sincosf_16_wrapper_l4l4_dev") == 1
            line = line.replace("sincosf_16_wrapper_l4l4_dev",
                                "sincosf_16_avx512_wrapped_l4l4_dev")
            print("\t{}".format(line))
        if "sincosf_4_wrapper_l4l4_glibc" in line:
            print("{} -> ".format(line))
            assert line.count("sincosf_4_wrapper_l4l4_glibc") == 1
            line = line.replace("sincosf_4_wrapper_l4l4_glibc",
                                "sincosf_4_sse2_wrapped_l4l4_glibc")
            print("\t{}".format(line))
        if "sincosf_8_wrapper_l4l4_glibc" in line:
            print("{} -> ".format(line))
            assert line.count("sincosf_8_wrapper_l4l4_glibc") == 1
            line = line.replace("sincosf_8_wrapper_l4l4_glibc",
                                "sincosf_8_avx2_wrapped_l4l4_glibc")
            print("\t{}".format(line))
        if "sincosf_16_wrapper_l4l4_glibc" in line:
            print("{} -> ".format(line))
            assert line.count("sincosf_16_wrapper_l4l4_glibc") == 1
            line = line.replace("sincosf_16_wrapper_l4l4_glibc",
                                "sincosf_16_avx512_wrapped_l4l4_glibc")
            print("\t{}".format(line))
        lines.append(line)
    f_out = open(f, "w+")
    f_out.write("".join(lines))
    f_out.close()

import os
import sys
import mmap
import platform
import json
import argparse
from common_util import (run_proc, ez_open, set_verbosity, warn_assert,
                         die_assert, vprint, vvprint, vvvprint, die, is_int,
                         as_int, ez_path, is_dir, ez_read, ez_close)

parser = argparse.ArgumentParser(description="Get CPU information")
parser.add_argument("--exe",
                    action="store",
                    default="build/third-party/cpu_features/list_cpu_features",
                    help="File to parse")
parser.add_argument("-o",
                    "--out",
                    action="store",
                    default=None,
                    help="Output file (default stdout)")
parser.add_argument("--arch",
                    action="store",
                    default=platform.machine(),
                    help="Set arch (default grabs from system)")
parser.add_argument("-v", action="count", default=0, help="Verbosity")


def fmt_def(v):
    return "#define {}".format(v)


def min_type(arr):
    max_v = max(arr)
    die_assert(max_v > 0)
    die_assert(min(arr) > -1)
    for i in range(0, 3):
        shift = 8 << i
        if max_v < (2**shift):
            return "uint{}_t".format(shift)
    die("No suitable type found")


def fmt_map(field, arr):
    T = min_type(arr)
    func_impl = []
    func_impl.append(
        "static uint32_t inline __attribute__((pure, always_inline)) cpu_to_{}(uint32_t cpu)"
        .format(field))
    func_impl.append("{")
    func_impl.append("static const {} cpu_to_{}_tbl".format(T, field))
    func_impl.append(" = {")
    func_impl.append(",".join(arr))
    func_impl.append("};")
    func_impl.append("if (cpu >= {})".format(len(arr)))
    func_impl.append("{ __builtin_unreachable(); }")
    func_impl.append("return cpu_to_{}_tbl[cpu];".format(field))
    func_impl.append("}")


def enum_fields(fields, fmt):
    out = []
    i = 0
    for field in sorted(fields):
        out.append(fmt.format(field, i))
    return out


def extract_int_field(d, field):
    die_assert(field in d)
    v = d[field]

    die_assert(is_int(v))
    return as_int(v)


class Topology():

    def __init__(self):
        self.id_vals = {
            "physical_package_id", "die_id", "core_id", "cluster_id",
            "book_id", "drawer_id"
        }
        self.map_vals = {
            "core_cpus", "package_cpus", "die_cpus", "cluster_cpus",
            "book_siblings", "drawer_siblings"
        }

        self.cpu_topology_path_fmt = "/sys/devices/system/cpu/cpu{}/topology"

        self.topology_info = {}
        self.id_preamble = []
        self.maps_out = {}

    def info_out(self):
        out = []
        out.append(
            "/********************************************************************/"
        )
        out.append("/* Topology Information.  */")
        out.append("\n".join([fmt_def(x) for x in self.id_preamble]))
        out.append("/* CPU Mappings.  */")
        out.append("\n".join([fmt_map(x, self.maps_out[x]) for x in self.maps_out]))
        out.append(
            "/********************************************************************/"
        )
    def collect(self):
        cpu_num = 0
        while True:
            path = self.cpu_path(cpu_num)
            if path is None:
                break

            for key_val in self.id_vals | self.map_vals:
                key_path = os.path.join(path, key_val)
                f = ez_open(key_path, "r")
                key_data = None
                if f:
                    key_data = ez_read(f)
                    die_assert(not ez_close(f))

                self.topology_info.setdefault(cpu_num, {})[key_val] = key_data

            cpu_num += 1

        maximums = {}
        for id_val in self.id_vals:
            maximums[id_val] = 0

        for i in range(0, cpu_num):
            assert i in self.topology_info
            cpu_info = self.topology_info[i]
            for id_val in self.id_vals:
                if cpu_info[id_val] is not None:
                    this_val = cpu_info[id_val]
                    assert is_int(this_val)
                    maximums[id_val] = max(maximums[id_val], as_int(this_val))

        self.id_preamble = []
        self.id_preamble.append("NUM_CPUS\t{}".format(cpu_num))
        for id_val in self.id_vals:

            assert id_val.endswith("_id")
            id_name = "num_" + id_val[:-3] + "s"
            id_name = id_name.upper()
            self.id_preamble.append("{}\t{}".format(id_name,
                                                    maximums[id_val] + 1))

        self.maps_out = {}
        for key_val in self.id_vals | self.map_vals:
            self.maps_out[key_val] = []
            has_none = 0
            has_real = 0
            base = 10
            if key_val in self.map_vals:
                base = 16

            for i in range(0, cpu_num):
                v = self.topology_info[i][key_val]
                assert is_int(v, base)
                if v is None:
                    has_none = 1
                else:
                    has_real = 1
                self.maps_out[key_val].append(as_int(v, base))
            die_assert(has_none != has_real, "Invalid sys info")

    def cpu_path(self, cpu_num):
        path = self.cpu_topology_path_fmt.format(cpu_num)
        if is_dir(path):
            return path
        return None


class CPU():

    def __init__(self, arch):
        self.features = None
        self.cache_types = None
        self.uarches = None
        self.architectures = None

        self.arch_preamble = None
        self.uarch_preamble = None

        self.arch_out = None
        self.uarch_out = None
        self.features_out = None
        self.cache_out = None
        self.tlb_out = None

        x86_features = {
            "fpu", "tsc", "cx8", "clfsh", "mmx", "aes", "erms", "f16c", "fma4",
            "fma3", "vaes", "vpclmulqdq", "bmi1", "hle", "bmi2", "rtm",
            "rdseed", "clflushopt", "clwb", "sse", "sse2", "sse3", "ssse3",
            "sse4_1", "sse4_2", "sse4a", "avx", "avx_vnni", "avx2", "avx512f",
            "avx512cd", "avx512er", "avx512pf", "avx512bw", "avx512dq",
            "avx512vl", "avx512ifma", "avx512vbmi", "avx512vbmi2",
            "avx512vnni", "avx512bitalg", "avx512vpopcntdq", "avx512_4vnniw",
            "avx512_4vbmi2", "avx512_second_fma", "avx512_4fmaps",
            "avx512_bf16", "avx512_vp2intersect", "amx_bf16", "amx_tile",
            "amx_int8", "pclmulqdq", "smx", "sgx", "cx16", "sha", "popcnt",
            "movbe", "rdrnd", "dca", "ss", "adx", "lzcnt"
        }

        x86_uarches = {
            "x86_unknown", "zhaoxin_zhangjiang", "zhaoxin_wudaokou",
            "zhaoxin_lujiazui", "zhaoxin_yongfeng", "intel_80486", "intel_p5",
            "intel_lakemont", "intel_core", "intel_pnr", "intel_nhm",
            "intel_atom_bnl", "intel_wsm", "intel_snb", "intel_ivb",
            "intel_atom_smt", "intel_hsw", "intel_bdw", "intel_skl",
            "intel_ccl", "intel_atom_gmt", "intel_atom_gmt_plus",
            "intel_atom_tmt", "intel_kbl", "intel_cfl", "intel_whl",
            "intel_cml", "intel_cnl", "intel_icl", "intel_tgl", "intel_spr",
            "intel_adl", "intel_rcl", "intel_knights_m", "intel_knights_l",
            "intel_knights_f", "intel_knights_c", "intel_netburst",
            "amd_hammer", "amd_k10", "amd_k11", "amd_k12", "amd_bobcat",
            "amd_piledriver", "amd_streamroller", "amd_excavator",
            "amd_bulldozer", "amd_jaguar", "amd_puma", "amd_zen",
            "amd_zen_plus", "amd_zen2", "amd_zen3", "amd_zen4"
        }

        x86_cache_types = {
            "data", "instruction", "unified", "tlb", "dtlb", "stlb", "prefetch"
        }

        if arch == "x86":
            self.features = x86_features
            self.uarches = x86_uarches
            self.cache_types = x86_cache_types
        else:
            die("Unkown arch: {}".format(arch))

        self.architectures = {"x86", "aarch64"}

    def get_features_out(self):
        out = []
        for arch_feature in self.features:
            has_feature = 0
            if arch_feature in self.this_features:
                has_feature = 1
            out.append("{}_FEATURE_{}\t{}".format(self.this_arch.upper(),
                                                  arch_feature, has_feature))

        return out

    def get_cache_level_out(self, cache_info):
        level = extract_int_field(cache_info, "level")
        cache_size = extract_int_field(cache_info, "cache_size")
        ways = extract_int_field(cache_info, "ways")
        line_size = extract_int_field(cache_info, "line_size")
        cache_size = extract_int_field(cache_info, "cache_size")
        tlb_entries = extract_int_field(cache_info, "tlb_entries")
        partitioning = extract_int_field(cache_info, "partitioning")

        die_assert("cache_type" in cache_info)
        cache_type = cache_info["cache_type"]

        assert cache_type in {"data", "instruction", "unified"}

        cache = []
        cache_sets = cache_size / (ways * line_size)
        die_assert(float(int(cache_sets)) == cache_sets)
        cache_sets = int(cache_sets)
        cache_psize = int(cache_size / partitioning)
        cache_prefix = "L{}{}_CACHE".format(level, cache_type[0].upper())
        cache.append("{}_WAYS\t{}".format(cache_prefix, ways))
        cache.append("{}_SETS\t{}".format(cache_prefix, cache_sets))
        cache.append("{}_LINE_SIZE\t{}".format(cache_prefix, line_size))
        cache.append("{}_SIZE\t{}".format(cache_prefix, cache_size))
        cache.append("{}_PSIZE\t{}".format(cache_prefix, cache_psize))

        tlb = []
        tlb_prefix = "L{}{}_TLB".format(level, cache_type[0].upper())
        tlb_size = mmap.PAGESIZE * tlb_entries
        tlb_psize = int(tlb_size / partitioning)
        tlb.append("{}_ENTRY_SIZE\t{}".format(tlb_prefix, mmap.PAGESIZE))
        tlb.append("{}_SIZE\t{}".format(tlb_prefix, tlb_size))
        tlb.append("{}_PSIZE\t{}".format(tlb_prefix, tlb_psize))

        return cache, tlb

    def collect(self, cpu_info):
        die_assert("arch" in cpu_info)
        die_assert("uarch" in cpu_info)
        die_assert("flags" in cpu_info)
        die_assert("cache_info" in cpu_info)

        self.this_arch = cpu_info["arch"].lower()
        assert self.this_arch in self.architectures

        self.this_uarch = cpu_info["uarch"].lower()
        assert self.this_uarch in self.uarches

        self.this_features = set(cpu_info["flags"])

        for feature in self.this_features:
            assert feature in self.features

        this_cache_info = cpu_info["cache_info"]

        self.arch_preamble = enum_fields(self.architectures, "ARCH_{}\t{}")
        self.uarch_preamble = enum_fields(self.uarches, "UARCH_{}\t{}")

        self.arch_out = ["CPU_ARCH\t{}".format(self.this_arch)]
        self.uarch_out = ["CPU_UARCH\t{}".format(self.this_uarch)]
        self.features_out = self.get_features_out()

        self.cache_out = []
        self.tlb_out = []
        for this_cache_level in this_cache_info:
            cache, tlb = self.get_cache_level_out(this_cache_level)
            self.cache_out.append(cache)
            self.tlb_out.append(tlb)


args = parser.parse_args()

executable = args.exe
outfile = args.out
verbosity = args.v
arch = args.arch.lower()
arch_map = {"x86": {"x86_64", "amd64", "x86", "x64"}}
for known_arch in arch_map:
    if arch in arch_map[known_arch]:
        arch = known_arch
        break

set_verbosity(verbosity)

if outfile is None:
    vprint("Outputting to stdout")
    outfile = sys.stdout
else:
    warn_assert(not os.path.isfile(outfile),
                "Warning: writing over outfile ({})".format(outfile))
    vprint("Outputting to {}".format(outfile))
    outfile = ez_open(outfile, "w+")

success, ret, stdout_data, stderr_data = run_proc(executable + " --json")

die_assert(success, "Failed to run: {}".format(executable))

cpu_info = json.loads(stdout_data)

cpu = CPU(arch)
topology = Topology()
cpu.collect(cpu_info)
topology.collect()

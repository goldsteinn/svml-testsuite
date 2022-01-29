#include <stdint.h>

#include "util/func-decl-generator.h"
#include "util/macro.h"

#include "bench/bench-decls.h"

#define decl_benchmark(name) extern uint64_t name(uint32_t trials);

make_decls(benchmarks, decl_benchmark, BENCHMARKS);

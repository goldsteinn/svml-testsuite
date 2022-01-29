#include "bench/timing.h"


void
print_res(const char * desc,
          uint64_t     time,
          uint64_t     trials,
          const char * units) {
    const char *_desc = "", *_units = "";
    if (desc) {
        _desc = desc;
    }
    if (units) {
        _units = units;
    }

    printf("%-16s -> %10.3lf %s\n", _desc,
           CAST(double, time) / CAST(double, trials), _units);
}

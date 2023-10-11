add_library(libfmt
    "include/fmt/args.h"
    "include/fmt/chrono.h"
    "include/fmt/color.h"
    "include/fmt/compile.h"
    "include/fmt/core.h"
    "include/fmt/format-inl.h"
    "include/fmt/format.h"
    "include/fmt/os.h"
    "include/fmt/ostream.h"
    "include/fmt/printf.h"
    "include/fmt/ranges.h"
    "include/fmt/std.h"
    "include/fmt/xchar.h"

    "lib/format.cc"
    "lib/os.cc"
)

target_include_directories(libfmt PUBLIC "include")

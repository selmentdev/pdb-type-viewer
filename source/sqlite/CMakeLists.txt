add_library(sqlite3 STATIC
  "source/sqlite3.c"
  "include/sqlite3.h"
  "include/sqlite3ext.h"
)

target_include_directories(sqlite3 PUBLIC
    "include"
    )
install(
    TARGETS sqlite3
    ARCHIVE DESTINATION "lib/static"
)

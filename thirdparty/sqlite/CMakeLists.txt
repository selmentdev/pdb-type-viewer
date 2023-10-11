add_library(libsqlite3 STATIC "src/sqlite3.c")
target_include_directories(libsqlite3 PUBLIC "include")

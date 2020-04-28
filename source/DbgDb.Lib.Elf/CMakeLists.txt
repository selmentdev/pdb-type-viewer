add_library(DbgDb.Lib.Elf SHARED
  "source/Plugin.cxx"
)

target_link_libraries(DbgDb.Lib.Elf PUBLIC DbgDb.Lib)

add_dependencies(DbgDb.App DbgDb.Lib.Elf)

target_include_directories(DbgDb.Lib.Elf PUBLIC
    "include"
    "${CMAKE_CURRENT_BINARY_DIR}"
    )

generate_export_header(DbgDb.Lib.Elf BASE_NAME DbgDbLibElf)

install(
    TARGETS DbgDb.Lib.Elf
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib/static"
)

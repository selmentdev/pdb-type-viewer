add_library(DbgDb.Lib.Pdb SHARED
  "source/Plugin.cxx"
)

target_link_libraries(DbgDb.Lib.Pdb PUBLIC DbgDb.Lib)

add_dependencies(DbgDb.App DbgDb.Lib.Pdb)

target_include_directories(DbgDb.Lib.Pdb PUBLIC
    "include"
    "${CMAKE_CURRENT_BINARY_DIR}"
    )
target_compile_options(DbgDb.Lib.Pdb PRIVATE /WX /W4)

generate_export_header(DbgDb.Lib.Pdb BASE_NAME DbgDbLibPdb)

install(
    TARGETS DbgDb.Lib.Pdb
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib/static"
)

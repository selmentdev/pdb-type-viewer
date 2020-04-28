add_library(DbgDb.Lib SHARED "source/DbgDb.Lib.SourceProvider.cxx" "include/DbgDb.Lib.SourceProvider.hxx" "source/DbgDb.Lib.Session.cxx" "source/DbgDb.Lib.Result.cxx")


target_include_directories(DbgDb.Lib PUBLIC
    "include"
    "${CMAKE_CURRENT_BINARY_DIR}"
    )

target_link_libraries(DbgDb.Lib PRIVATE sqlite3)

generate_export_header(DbgDb.Lib BASE_NAME DbgDbLib)

install(
    TARGETS DbgDb.Lib
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib/static"
)

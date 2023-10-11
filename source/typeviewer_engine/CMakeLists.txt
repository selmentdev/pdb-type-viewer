add_library(typeviewer_engine STATIC
 "src/Diagnostic.cxx"
 "src/Importer.cxx" "src/Model.cxx")

target_include_directories(typeviewer_engine PUBLIC "include")
target_link_libraries(typeviewer_engine PUBLIC libfmt libsqlite3)

add_executable(DbgDb.App ${GUI_TYPE}
 "source/Main.cxx"
 "source/Forms/FormMain.cxx"
 "include/Forms/FormMain.hxx"
 "resources/app.qrc"
 )

target_link_libraries(DbgDb.App PRIVATE Qt5::Widgets)
target_link_libraries(DbgDb.App PRIVATE DbgDb.Lib)
target_include_directories(DbgDb.App PRIVATE "include")

install(
    TARGETS DbgDb.App
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib/static"
)

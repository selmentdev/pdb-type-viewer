qt_add_executable(
 typeviewer_app
 "src/App.cxx"
 "res/App.qrc"
 "res/Win32.rc"
)

target_include_directories(
    typeviewer_app
    PUBLIC "include"
)

target_link_libraries(
    typeviewer_app
    PUBLIC Qt6::Widgets
    PUBLIC typeviewer_engine
)

set_target_properties(
    typeviewer_app PROPERTIES WIN32_EXECUTABLE ON
)

install(
    TARGETS typeviewer_app
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET typeviewer_app
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

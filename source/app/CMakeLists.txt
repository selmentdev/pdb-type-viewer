qt_add_executable(
    ptv-ui
        "public/Forms/FormAboutBox.hxx"
        "public/Forms/FormMain.hxx"
        "public/Models/ModelTypeDescriptor.hxx"
        "public/Models/ModelTypeList.hxx"
        "public/Models/ModelValidationList.hxx"
        "private/Main.cxx"
        "private/Models/ModelTypeDescriptor.cxx"
        "private/Models/ModelTypeList.cxx"
        "private/Models/ModelValidationList.cxx"
        "private/Forms/FormAboutBox.cxx"
        "private/Forms/FormMain.cxx"
        "resources/app.qrc"
        "resources/resources.rc"
)

target_include_directories(
    ptv-ui
    PUBLIC "public"
)

target_link_libraries(
    ptv-ui
    PUBLIC Qt6::Widgets
    PUBLIC ptv-core
)

set_target_properties(
    ptv-ui PROPERTIES
    WIN32_EXECUTABLE ON
)

install(
    TARGETS ptv-ui
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET ptv-ui
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

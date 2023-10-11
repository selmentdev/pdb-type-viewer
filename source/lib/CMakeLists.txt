add_library(
        ptv-core
    SHARED
    "private/LibPtv.cxx"
    "private/LibPtv/BaseTypeMember.cxx"
    "private/LibPtv/Session.cxx"
    "private/LibPtv/Type.cxx"
    "private/LibPtv/TypeDescriptor.cxx"
    "private/LibPtv/TypeMemberField.cxx"
    "private/LibPtv/TypeMemberInherited.cxx"
    "private/LibPtv/TypeMemberPadding.cxx"
)

target_include_directories(
    ptv-core
    PUBLIC "public"
)

target_compile_definitions(
    ptv-core
    PRIVATE "PTV_BUILDING_LIB"
)

if (WIN32)
    find_package(DiaSDK REQUIRED) # Used for constants and declarations.
endif (WIN32)

if (WIN32)
    target_include_directories(ptv-core PRIVATE ${DIASDK_INCLUDE_DIRS})
    target_link_libraries(ptv-core PRIVATE ${DIASDK_LIBRARIES})
endif(WIN32)

install(
    TARGETS  ptv-core
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    FILES       ${DIASDK_MSDIADLL}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

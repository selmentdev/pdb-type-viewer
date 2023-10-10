cmake_minimum_required(VERSION 3.17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

# >8 Customize Qt location here here 8<
# FIXME: This can be done automatically in QtCreator / CLion or other magic cmake json files.
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "D:/Qt/6.6.0/msvc2019_64")
# *8 ---------- 8*

project(pdb-type-viewer VERSION 0.3.0.0)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.hxx.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/PdbTypeViewer/Version.hxx" @ONLY
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_OPTIMIZE_DEPENDENCIES TRUE)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

qt_standard_project_setup()

add_subdirectory(source)

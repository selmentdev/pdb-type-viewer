cmake_minimum_required(VERSION 3.16)

project(DbgApp LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
set(GUI_TYPE WIN32)
endif(WIN32)

include(GenerateExportHeader)

#find_package(Qt5 COMPONENTS Widgets REQUIRED)
#find_package(Qt5Widgets)

find_package(Qt5 REQUIRED Widgets)

add_subdirectory(source/DbgDb.App)
add_subdirectory(source/DbgDb.Lib)
add_subdirectory(source/DbgDb.Lib.Pdb)
add_subdirectory(source/DbgDb.Lib.Elf)
add_subdirectory(source/sqlite)

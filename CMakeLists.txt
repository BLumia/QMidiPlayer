cmake_minimum_required(VERSION 3.10.0)

project(QMidiPlayer VERSION 0.8.7 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FindPkgConfig)
find_package(Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Threads REQUIRED)
pkg_search_module(fluidsynth REQUIRED fluidsynth>=2.0.0)
pkg_search_module(rtmidi REQUIRED rtmidi)

option(BUILD_VISUALIZATION "Build visualization plugin" ON)
if(WIN32)
	option(BUILD_BACKTRACE "Build backtrace library" OFF)
endif()
if(UNIX)
	option(BUILD_PORTABLE "Instruct the built binary not to search system directories" OFF)
endif()

add_definitions(-DAPP_VERSION=\"${PROJECT_VERSION}\")
add_definitions(-DPLUGIN_VERSION=\"${PROJECT_VERSION}\")
add_definitions(-DRC_VER_MAJOR=${PROJECT_VERSION_MAJOR})
add_definitions(-DRC_VER_MINOR=${PROJECT_VERSION_MINOR})
add_definitions(-DRC_VER_PATCH=${PROJECT_VERSION_PATCH})

add_subdirectory(core)
add_subdirectory(qmidiplayer-desktop)
add_subdirectory(sample-plugin)
add_subdirectory(midifmt-plugin)
add_subdirectory(simple-visualization)
if(BUILD_VISUALIZATION)
    add_subdirectory(visualization)
endif()
if(WIN32)
if(BUILD_BACKTRACE)
	add_subdirectory(third_party/backtrace-mingw)
endif()
endif()

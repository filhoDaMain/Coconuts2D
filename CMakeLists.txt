cmake_minimum_required(VERSION 3.1)
project(Coconuts2D VERSION 0.1 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

option(COMPILE_GAME_EDITOR "Build Game Editor application" ON)
add_compile_definitions("COCONUTS2D_SOURCES_ROOTDIR=\"${PROJECT_SOURCE_DIR}\"")

if( CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_definitions(COCONUTS2D_BUILDTYPE_DEBUG)
endif()

# Thirdparty
add_subdirectory(thirdparty/lua)
add_subdirectory(thirdparty/yaml-cpp)

# Coconuts2D
add_subdirectory(src)

cmake_minimum_required(VERSION 3.1)
project(Coconuts2D VERSION 0.1 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

set(COMPILE_GAME_EDITOR true)

if( CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_definitions(COCONUTS2D_BUILDTYPE_DEBUG)
endif()

# Thirdparty
add_subdirectory(thirdparty/lua)

# Coconuts2D
add_subdirectory(src)

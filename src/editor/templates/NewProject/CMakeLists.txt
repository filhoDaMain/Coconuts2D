cmake_minimum_required(VERSION 3.1)
project(Game VERSION 0.1 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

include(${PROJECT_SOURCE_DIR}/tools/res-compiler/CMakeRC.cmake)

FILE(GLOB_RECURSE Resources
    res/desc.txt res/*
)
cmrc_add_resource_library(resources ${Resources})

set(SOURCES
    src/main.cpp
)
set(LINK_AGAINST
    $ENV{ENGINE}
    resources
)

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCES}
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    $ENV{ENV_COCONUTS2D_EXPORT_INCLUDES_DIRPATH}
    src
)
target_link_libraries(${CMAKE_PROJECT_NAME}
    ${LINK_AGAINST}
)

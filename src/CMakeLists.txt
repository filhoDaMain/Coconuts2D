# Core Library
# ---------------------------------------------------------------------------
set(CORE_SOURCES)
list(APPEND CORE_SOURCES
    Application.cpp
    SceneManager.cpp
    FileSystem.cpp
    ecs/Scene.cpp
    ResourceManager.cpp
)
add_library(core ${CORE_SOURCES})

target_include_directories(core PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty"
)


# Scripting Library
# ---------------------------------------------------------------------------
include(${PROJECT_SOURCE_DIR}/thirdparty/cmrc/CMakeRC.cmake)
FILE(GLOB_RECURSE Scripting
    Scripting/*
)
cmrc_add_resource_library(scripting ${Scripting})


# Combined ccn2d Library
# ---------------------------------------------------------------------------
# Combined library output
set(CCN2D_LIB ${CMAKE_CURRENT_BINARY_DIR}/libccn2d.a)
add_custom_target(combined
    COMMAND ar -x $<TARGET_FILE:core>
    COMMAND ar -x $<TARGET_FILE:lua>
    COMMAND ar -x $<TARGET_FILE:scripting>
    COMMAND ar -rcs ${CCN2D_LIB} *.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    BYPRODUCTS ${CCN2D_LIB}
    DEPENDS core lua scripting
)

# Combined library target
add_library(ccn2d STATIC IMPORTED GLOBAL)
add_dependencies(ccn2d combined)
set_target_properties(ccn2d
    PROPERTIES
    IMPORTED_LOCATION ${CCN2D_LIB}
)
target_include_directories(ccn2d INTERFACE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty"
)


# Compile as a standalone Game Editor Application
# ---------------------------------------------------------------------------
add_subdirectory(editor)

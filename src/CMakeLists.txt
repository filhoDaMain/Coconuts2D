# Core Library
# ---------------------------------------------------------------------------
set(CORE_SOURCES)
list(APPEND CORE_SOURCES
    Application.cpp
    SceneManager.cpp
    FileSystem.cpp
    ecs/Scene.cpp
)
add_library(core ${CORE_SOURCES})

target_include_directories(core PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty"
)


# Combined ccn2d Library
# ---------------------------------------------------------------------------
# Full list of third party targets to link against
set(THIRD_PARTY_TARGETS
    lua
)

# Combined library output
set(CCN2D_LIB ${CMAKE_CURRENT_BINARY_DIR}/libccn2d.a)
add_custom_target(combined
    COMMAND ar -x $<TARGET_FILE:core>
    COMMAND ar -x $<TARGET_FILE:${THIRD_PARTY_TARGETS}>
    COMMAND ar -rcs ${CCN2D_LIB} *.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    BYPRODUCTS ${CCN2D_LIB}
    DEPENDS core lua
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

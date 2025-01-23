# Core Library
# ---------------------------------------------------------------------------
set(CORE_SOURCES)
list(APPEND CORE_SOURCES
    Application.cpp
    SceneManager.cpp
    Executor.cpp
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
    COMMAND ar -qcs ${CCN2D_LIB} *.o
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
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


# Main app
# ---------------------------------------------------------------------------
set(APP_LINK_AGAINST ccn2d)

# Compile as a standalone Game Editor
if (COMPILE_GAME_EDITOR)
    add_compile_definitions(COCONUTS2D_COMPILE_GAME_EDITOR)
    add_subdirectory(editor)
    list(APPEND APP_LINK_AGAINST editor)
endif()

add_executable(Coconuts2D
    main.cpp
)
target_link_libraries(Coconuts2D ${APP_LINK_AGAINST})

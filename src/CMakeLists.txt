# Core Library
# ---------------------------------------------------------------------------
set(CCN2D_SOURCES)
list(APPEND CCN2D_SOURCES
    Application.cpp
    SceneManager.cpp
    Executor.cpp
    ecs/Scene.cpp
)
add_library(ccn2d ${CCN2D_SOURCES})

# Game Editor
if (COMPILE_GAME_EDITOR)
    add_compile_definitions(COCONUTS2D_COMPILE_GAME_EDITOR)
    add_subdirectory(editor)
    target_link_libraries(ccn2d editor)
endif()

# Link with 3rd party libraries
target_link_libraries(ccn2d lua)

target_include_directories(ccn2d PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty"
)


# Main app
# ---------------------------------------------------------------------------
add_executable(Coconuts2D
    main.cpp
)
target_link_libraries(Coconuts2D ccn2d)

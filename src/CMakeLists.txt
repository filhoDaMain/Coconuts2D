# Core Library
# ---------------------------------------------------------------------------
set(CCN2D_SOURCES)

list(APPEND CCN2D_SOURCES
    Application.cpp
    SceneManager.cpp
    ecs/Scene.cpp
)

if (COMPILE_GAME_EDITOR)
    list(APPEND CCN2D_SOURCES
        editor/EditorScene.cpp
    )
    add_compile_definitions(COCONUTS2D_COMPILE_GAME_EDITOR)
endif()

add_library(ccn2d ${CCN2D_SOURCES})

target_include_directories(ccn2d PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty")


# Main app
# ---------------------------------------------------------------------------
add_executable(Coconuts2D
    main.cpp
)

target_link_libraries(Coconuts2D ccn2d lua)

# Core Library
# ---------------------------------------------------------------------------
add_library(ccn2d
    Application.cpp
    SceneManager.cpp
    ecs/Scene.cpp
)
target_include_directories(ccn2d PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty")


# Main app
# ---------------------------------------------------------------------------
add_executable(Coconuts2D
    main.cpp
)

target_link_libraries(Coconuts2D ccn2d)

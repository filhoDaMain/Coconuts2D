# Core Library
# ---------------------------------------------------------------------------
add_library(ccn2d
    Application.cpp
    Scene.cpp
)

target_include_directories(ccn2d PUBLIC  "${PROJECT_SOURCE_DIR}/include")

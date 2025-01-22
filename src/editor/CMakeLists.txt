# Editor Library
# ---------------------------------------------------------------------------
set(CCN2D_EDITOR_SOURCES)
list(APPEND CCN2D_EDITOR_SOURCES
    EditorScene.cpp
)
add_library(editor ${CCN2D_EDITOR_SOURCES})

target_include_directories(editor PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty"
)

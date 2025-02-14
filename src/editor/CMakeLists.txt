# Editor Library
# ---------------------------------------------------------------------------
set(CORE_EDITOR_SOURCES)
list(APPEND CORE_EDITOR_SOURCES
    EditorScene.cpp
    ProjectManager.cpp
    templates/NewProject.cpp
)
add_library(core_editor ${CORE_EDITOR_SOURCES})

target_include_directories(core_editor PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty"
)

# Full list of third party targets to link against
set(THIRD_PARTY_EDITOR_TARGETS
    yaml-cpp
)

# Combined library output
set(EDITOR_LIB ${CMAKE_CURRENT_BINARY_DIR}/libeditor.a)
add_custom_target(editor_combined
    COMMAND ar -x $<TARGET_FILE:core_editor>
    COMMAND ar -x $<TARGET_FILE:${THIRD_PARTY_EDITOR_TARGETS}>
    COMMAND ar -rcs ${EDITOR_LIB} *.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS core_editor yaml-cpp
)

# Combined library target
add_library(editor STATIC IMPORTED GLOBAL)
add_dependencies(editor editor_combined)
set_target_properties(editor
    PROPERTIES
    IMPORTED_LOCATION ${EDITOR_LIB}
)
target_include_directories(editor INTERFACE
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/include/thirdparty"
)

# Compile time macros
add_compile_definitions(
    "COCONUTS2D_NEWPROJECT_TEMPLATE_FILE=\"${PROJECT_SOURCE_DIR}/templates/NewProject/template.txt\"")
add_compile_definitions(
    "COCONUTS2D_NEWPROJECT_TEMPLATE_BUILDSH=\"${PROJECT_SOURCE_DIR}/templates/NewProject/build.sh\"")
add_compile_definitions(
        "COCONUTS2D_NEWPROJECT_TEMPLATE_CMAKELISTS=\"${PROJECT_SOURCE_DIR}/templates/NewProject/CMakeLists.txt\"")
add_compile_definitions(
    "COCONUTS2D_NEWPROJECT_TEMPLATE_ENV=\"${PROJECT_SOURCE_DIR}/templates/NewProject/ENV\"")
add_compile_definitions(
        "COCONUTS2D_NEWPROJECT_TEMPLATE_MAIN=\"${PROJECT_SOURCE_DIR}/templates/NewProject/main.cpp\"")
add_compile_definitions(
        "COCONUTS2D_SCRIPTING_API_DIR=\"${PROJECT_SOURCE_DIR}/scripting\"")


# Standalone Game Editor Application
# ---------------------------------------------------------------------------
set(APP_LINK_AGAINST ccn2d editor)

add_executable(Coconuts2D
    main.cpp
)
target_link_libraries(Coconuts2D ${APP_LINK_AGAINST})

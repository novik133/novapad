##############################################################################
# NovaPad - Source Directory CMake Configuration
#
# Aggregates all source modules into a single executable target.
# Each subdirectory contributes its own source files via target_sources().
##############################################################################

# ---------------------------------------------------------------------------
# Collect all source files from each module
# ---------------------------------------------------------------------------
set(NOVAPAD_CORE_SOURCES
    core/application.cpp
    core/application.h
    core/single_instance.cpp
    core/single_instance.h
    core/globals.cpp
    core/globals.h
    core/command_line_parser.cpp
    core/command_line_parser.h
)

set(NOVAPAD_EDITOR_SOURCES
    editor/code_editor_widget.cpp
    editor/code_editor_widget.h
    editor/syntax_highlighter.cpp
    editor/syntax_highlighter.h
    editor/language_manager.cpp
    editor/language_manager.h
    editor/line_number_area.cpp
    editor/line_number_area.h
    editor/minimap_widget.cpp
    editor/minimap_widget.h
    editor/bracket_matcher.cpp
    editor/bracket_matcher.h
    editor/auto_completer.cpp
    editor/auto_completer.h
    editor/code_folding_manager.cpp
    editor/code_folding_manager.h
)

set(NOVAPAD_UI_SOURCES
    ui/main_window.cpp
    ui/main_window.h
    ui/top_editor_container.cpp
    ui/top_editor_container.h
    ui/editor_tab_widget.cpp
    ui/editor_tab_widget.h
    ui/status_bar_manager.cpp
    ui/status_bar_manager.h
    ui/toolbar_manager.cpp
    ui/toolbar_manager.h
    ui/icon_provider.cpp
    ui/icon_provider.h
    ui/svg_icon_engine.cpp
    ui/svg_icon_engine.h
)

set(NOVAPAD_DIALOG_SOURCES
    ui/dialogs/about_dialog.cpp
    ui/dialogs/about_dialog.h
    ui/dialogs/preferences_dialog.cpp
    ui/dialogs/preferences_dialog.h
    ui/dialogs/encoding_chooser_dialog.cpp
    ui/dialogs/encoding_chooser_dialog.h
    ui/dialogs/indentation_dialog.cpp
    ui/dialogs/indentation_dialog.h
    ui/dialogs/go_to_line_dialog.cpp
    ui/dialogs/go_to_line_dialog.h
    ui/dialogs/run_command_dialog.cpp
    ui/dialogs/run_command_dialog.h
)

set(NOVAPAD_DOCK_SOURCES
    ui/dock/file_tree_dock.cpp
    ui/dock/file_tree_dock.h
    ui/dock/terminal_dock.cpp
    ui/dock/terminal_dock.h
    ui/dock/output_dock.cpp
    ui/dock/output_dock.h
)

set(NOVAPAD_SEARCH_SOURCES
    search/search_replace_dialog.cpp
    search/search_replace_dialog.h
    search/advanced_search_dock.cpp
    search/advanced_search_dock.h
    search/search_helpers.cpp
    search/search_helpers.h
    search/search_instance.cpp
    search/search_instance.h
    search/search_worker.cpp
    search/search_worker.h
    search/search_result_model.cpp
    search/search_result_model.h
)

set(NOVAPAD_SESSION_SOURCES
    sessions/session_manager.cpp
    sessions/session_manager.h
    sessions/backup_service.cpp
    sessions/backup_service.h
    sessions/persistent_cache.cpp
    sessions/persistent_cache.h
)

set(NOVAPAD_EXTENSION_SOURCES
    extensions/extension.cpp
    extensions/extension.h
    extensions/extension_loader.cpp
    extensions/extension_loader.h
    extensions/extension_server.cpp
    extensions/extension_server.h
    extensions/extension_api.cpp
    extensions/extension_api.h
)

set(NOVAPAD_SETTINGS_SOURCES
    settings/novapad_settings.cpp
    settings/novapad_settings.h
    settings/shortcut_manager.cpp
    settings/shortcut_manager.h
)

set(NOVAPAD_UTILS_SOURCES
    utils/file_utils.cpp
    utils/file_utils.h
    utils/encoding_detector.cpp
    utils/encoding_detector.h
    utils/text_operations.cpp
    utils/text_operations.h
    utils/platform_utils.cpp
    utils/platform_utils.h
)

set(NOVAPAD_THEMES_SOURCES
    themes/theme_manager.cpp
    themes/theme_manager.h
    themes/color_scheme.cpp
    themes/color_scheme.h
)

# ---------------------------------------------------------------------------
# Language Support Sources
# ---------------------------------------------------------------------------
set(NOVAPAD_LANGUAGES_BASE_SOURCES
    languages/base/language_support_base.cpp
    languages/base/language_support_base.h
    languages/base/c_family_base.cpp
    languages/base/c_family_base.h
)

set(NOVAPAD_LANGUAGES_DEFINITIONS_SOURCES
    languages/definitions/c_language.cpp
    languages/definitions/c_language.h
    languages/definitions/cpp_language.cpp
    languages/definitions/cpp_language.h
    languages/definitions/cs_language.cpp
    languages/definitions/cs_language.h
    languages/definitions/rust_language.cpp
    languages/definitions/rust_language.h
    languages/definitions/go_language.cpp
    languages/definitions/go_language.h
    languages/definitions/python_language.cpp
    languages/definitions/python_language.h
    languages/definitions/javascript_language.cpp
    languages/definitions/javascript_language.h
    languages/definitions/kotlin_language.cpp
    languages/definitions/kotlin_language.h
    languages/definitions/swift_language.cpp
    languages/definitions/swift_language.h
    languages/definitions/php_language.cpp
    languages/definitions/php_language.h
    languages/definitions/perl_language.cpp
    languages/definitions/perl_language.h
    languages/definitions/html_css_language.cpp
    languages/definitions/html_css_language.h
    languages/definitions/markdown_language.cpp
    languages/definitions/markdown_language.h
    languages/definitions/sql_language.cpp
    languages/definitions/sql_language.h
)

set(NOVAPAD_LANGUAGES_SUPPORT_SOURCES
    languages/support/language_registry.cpp
    languages/support/language_registry.h
    languages/support/advanced_syntax_highlighter.cpp
    languages/support/advanced_syntax_highlighter.h
    languages/support/language_system_initializer.cpp
    languages/support/language_system_initializer.h
)

# ---------------------------------------------------------------------------
# Qt Resource Files
# ---------------------------------------------------------------------------
set(NOVAPAD_RESOURCES
    resources/novapad.qrc
)

# ---------------------------------------------------------------------------
# Main entry point
# ---------------------------------------------------------------------------
set(NOVAPAD_MAIN
    main.cpp
)

# ---------------------------------------------------------------------------
# Create object library for sources (shared between main app and tests)
# ---------------------------------------------------------------------------
add_library(novapad_lib OBJECT
    ${NOVAPAD_CORE_SOURCES}
    ${NOVAPAD_EDITOR_SOURCES}
    ${NOVAPAD_UI_SOURCES}
    ${NOVAPAD_DIALOG_SOURCES}
    ${NOVAPAD_DOCK_SOURCES}
    ${NOVAPAD_SEARCH_SOURCES}
    ${NOVAPAD_SESSION_SOURCES}
    ${NOVAPAD_EXTENSION_SOURCES}
    ${NOVAPAD_SETTINGS_SOURCES}
    ${NOVAPAD_UTILS_SOURCES}
    ${NOVAPAD_THEMES_SOURCES}
    ${NOVAPAD_LANGUAGES_BASE_SOURCES}
    ${NOVAPAD_LANGUAGES_DEFINITIONS_SOURCES}
    ${NOVAPAD_LANGUAGES_SUPPORT_SOURCES}
)

target_include_directories(novapad_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/generated
)

target_link_libraries(novapad_lib PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Network
    Qt6::Svg
    Qt6::SvgWidgets
    Qt6::Concurrent
    Qt6::DBus
    Qt6::Core5Compat
)

# Compile definitions for the library
target_compile_definitions(novapad_lib PRIVATE
    $<$<CONFIG:Debug>:NOVAPAD_DEBUG>
    QT_DISABLE_DEPRECATED_BEFORE=0x061000
)

if(NOVAPAD_ENABLE_PLUGINS)
    target_compile_definitions(novapad_lib PRIVATE NOVAPAD_PLUGINS_ENABLED)
endif()

if(TARGET Qt6::WebEngineWidgets)
    target_link_libraries(novapad_lib PRIVATE Qt6::WebEngineWidgets)
    target_compile_definitions(novapad_lib PRIVATE NOVAPAD_HAS_WEBENGINE)
endif()

# ---------------------------------------------------------------------------
# Create the NovaPad executable
# ---------------------------------------------------------------------------
qt_add_executable(novapad
    ${NOVAPAD_MAIN}
    ${NOVAPAD_RESOURCES}
)

target_link_libraries(novapad PRIVATE novapad_lib)

# ---------------------------------------------------------------------------
# Include Directories
# ---------------------------------------------------------------------------
target_include_directories(novapad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/generated
)

# ---------------------------------------------------------------------------
# Link Qt Libraries
# ---------------------------------------------------------------------------
target_link_libraries(novapad PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Network
    Qt6::Svg
    Qt6::SvgWidgets
    Qt6::Concurrent
    Qt6::DBus
    Qt6::Core5Compat
)

# Optionally link WebEngine for rich editor rendering
if(TARGET Qt6::WebEngineWidgets)
    target_link_libraries(novapad PRIVATE Qt6::WebEngineWidgets)
    target_compile_definitions(novapad PRIVATE NOVAPAD_HAS_WEBENGINE)
endif()

# ---------------------------------------------------------------------------
# Compile Definitions
# ---------------------------------------------------------------------------
target_compile_definitions(novapad PRIVATE
    $<$<CONFIG:Debug>:NOVAPAD_DEBUG>
    QT_DISABLE_DEPRECATED_BEFORE=0x061000
)

if(NOVAPAD_ENABLE_PLUGINS)
    target_compile_definitions(novapad PRIVATE NOVAPAD_PLUGINS_ENABLED)
endif()

# ---------------------------------------------------------------------------
# Translations (optional)
# ---------------------------------------------------------------------------
if(NOVAPAD_ENABLE_TRANSLATIONS)
    file(GLOB TS_FILES "${CMAKE_SOURCE_DIR}/resources/translations/*.ts")
    if(TS_FILES)
        qt_add_translations(novapad TS_FILES ${TS_FILES})
    endif()
endif()

# ---------------------------------------------------------------------------
# Platform-specific settings
# ---------------------------------------------------------------------------
if(APPLE)
    set_target_properties(novapad PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "NovaPad"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.novapad.NovaPad"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_ICON_FILE "novapad.icns"
    )
elseif(WIN32)
    set_target_properties(novapad PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

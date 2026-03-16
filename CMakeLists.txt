##############################################################################
# NovaPad - Modern Programmer's Notebook
# Root CMake Configuration
#
# Build system targeting C++23 and Qt 6.10.
# Supports Linux, macOS, and Windows builds.
#
# Usage:
#   cmake -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build --parallel
#
# Author: NovaPad Team
# License: GPL-3.0-or-later
##############################################################################

cmake_minimum_required(VERSION 3.25)

project(NovaPad
    VERSION 1.0.0
    DESCRIPTION "Modern programmer's notebook built with C++23 and Qt 6.10"
    HOMEPAGE_URL "https://github.com/novapad/novapad"
    LANGUAGES CXX
)

# -----------------------------------------------------------------------------
# Global C++ Standard Configuration
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Qt 6.10 Configuration
# -----------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    PrintSupport
    Network
    Svg
    SvgWidgets
    LinguistTools
    Concurrent
    DBus
    Core5Compat        # For QTextCodec backwards compatibility
)

# Optional components for enhanced functionality
find_package(Qt6 6.10 OPTIONAL_COMPONENTS
    WebEngineWidgets   # For embedded web-based editor rendering
)

# -----------------------------------------------------------------------------
# Compiler-Specific Flags
# -----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -Werror=return-type
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    endif()
elseif(MSVC)
    add_compile_options(/W4 /permissive- /Zc:__cplusplus)
endif()

# -----------------------------------------------------------------------------
# Build Options
# -----------------------------------------------------------------------------
option(NOVAPAD_ENABLE_PLUGINS     "Enable the plugin/extension system"   ON)
option(NOVAPAD_ENABLE_TESTS       "Build the test suite"                 ON)
option(NOVAPAD_ENABLE_TRANSLATIONS "Build translation files"             ON)
option(NOVAPAD_USE_SYSTEM_THEMES  "Use system icon themes when available" ON)

# -----------------------------------------------------------------------------
# Version Embedding
# -----------------------------------------------------------------------------
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/novapad_version.h"
    @ONLY
)

# -----------------------------------------------------------------------------
# Source Subdirectory
# -----------------------------------------------------------------------------
add_subdirectory(src)

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
if(NOVAPAD_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# -----------------------------------------------------------------------------
# Install Rules
# -----------------------------------------------------------------------------
include(GNUInstallDirs)

install(TARGETS novapad
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES resources/images/novapad.svg
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
    RENAME novapad.svg
)

install(DIRECTORY resources/icons/novapad/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/novapad
)

# Desktop entry for Linux
if(UNIX AND NOT APPLE)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/support/novapad.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    )
endif()

message(STATUS "-----------------------------------------------")
message(STATUS "NovaPad ${PROJECT_VERSION}")
message(STATUS "  C++ Standard:  ${CMAKE_CXX_STANDARD}")
message(STATUS "  Qt Version:    ${Qt6_VERSION}")
message(STATUS "  Build Type:    ${CMAKE_BUILD_TYPE}")
message(STATUS "  Plugins:       ${NOVAPAD_ENABLE_PLUGINS}")
message(STATUS "  Tests:         ${NOVAPAD_ENABLE_TESTS}")
message(STATUS "  Translations:  ${NOVAPAD_ENABLE_TRANSLATIONS}")
message(STATUS "-----------------------------------------------")

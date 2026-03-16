# NovaPad

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/novik133/novapad)](https://github.com/novik133/novapad/releases)
[![GitHub license](https://img.shields.io/github/license/novik133/novapad)](https://github.com/novik133/novapad/blob/main/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/novik133/novapad)](https://github.com/novik133/novapad/issues)
[![GitHub stars](https://img.shields.io/github/stars/novik133/novapad)](https://github.com/novik133/novapad/stargazers)
[![C++](https://img.shields.io/badge/C++-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![Qt](https://img.shields.io/badge/Qt-6.10-green.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](https://github.com/novik133/novapad)

> **A modern fork of [Notepadqq](https://notepadqq.com)** — rebuilt from scratch with native Qt6 widgets (C++23), replacing the web-based editor for better performance and native integration.
>
> **Original project:** [notepadqq/notepadqq](https://github.com/notepadqq/notepadqq) by Daniele Di Sarli and contributors (2015-2019)

A modern programmer's notebook built with **C++23** and **Qt 6.10**.

NovaPad provides all the features a programmer needs for daily work, with a focus on native performance, modern C++ practices, and clean architecture.

---

## Features

- **Native Qt Code Editor** — High-performance text editing with QPlainTextEdit
- **Syntax Highlighting** — 50+ programming languages supported natively
- **Multi-Tab Editing** — Tabbed interface with split-view support
- **Code Folding** — Collapse/expand code regions (brace-based and indent-based)
- **Auto-Completion** — Word-based and keyword-based completion popup
- **Bracket Matching** — Highlights matching delimiters in real time
- **Minimap** — Zoomed-out document overview for quick navigation
- **Line Numbers** — Gutter with line numbers and fold indicators
- **Search & Replace** — In-document and multi-file search with regex support
- **Find in Files** — Recursive grep-like search across project directories
- **File Browser** — Integrated file tree dock for project navigation
- **Integrated Terminal** — Run shell commands without leaving the editor
- **Output Panel** — View build/command output in a dedicated dock
- **Session Management** — Save and restore open tabs across sessions
- **Crash Recovery** — Automatic backup with sentinel-based crash detection
- **Extension System** — Plugin API with manifest-based extensions
- **Configurable Themes** — Light, Dark, Monokai, Solarized Light/Dark
- **Encoding Support** — Auto-detection of UTF-8, UTF-16, UTF-32 with BOM handling
- **EOL Conversion** — Unix (LF), Windows (CRLF), Old Mac (CR)
- **Print / PDF Export** — Print documents or export via print preview
- **Customisable Shortcuts** — All keyboard shortcuts can be reassigned
- **Run External Commands** — Execute shell commands with output capture
- **Single Instance** — Enforces a single running instance via local sockets
- **Drag & Drop** — Open files by dropping them onto the editor
- **Recent Files** — Quick access to recently opened documents
- **Multi-Window** — Support for multiple independent editor windows

---

## Building

### Prerequisites

- **CMake** 3.25 or later
- **Qt** 6.10 or later (Core, Gui, Widgets, PrintSupport, Network, Svg, SvgWidgets, Concurrent, DBus, Core5Compat)
- **C++23** compatible compiler (GCC 13+, Clang 17+, MSVC 2022+)

### Build steps

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

### CMake options

| Option                       | Default | Description                          |
|------------------------------|---------|--------------------------------------|
| `NOVAPAD_ENABLE_PLUGINS`     | `OFF`   | Enable the extension/plugin system   |
| `NOVAPAD_ENABLE_TRANSLATIONS`| `OFF`   | Build translation files              |

---

## Project Structure

```
NovaPad/
├── CMakeLists.txt              # Top-level CMake configuration
├── cmake/
│   └── version.h.in            # Version header template
├── icons/                      # Application icons (copied from Notepadqq)
├── src/
│   ├── CMakeLists.txt          # Source-level CMake configuration
│   ├── main.cpp                # Application entry point
│   ├── core/                   # Core subsystems
│   │   ├── application.*       # QApplication wrapper
│   │   ├── single_instance.*   # Single-instance enforcement
│   │   ├── command_line_parser.* # CLI argument parsing
│   │   └── globals.*           # Application-wide constants and paths
│   ├── editor/                 # Native code editor module
│   │   ├── code_editor_widget.* # Main editor widget
│   │   ├── syntax_highlighter.* # QSyntaxHighlighter engine
│   │   ├── language_manager.*  # Language catalogue singleton
│   │   ├── line_number_area.*  # Line number gutter
│   │   ├── minimap_widget.*    # Document overview minimap
│   │   ├── bracket_matcher.*   # Bracket matching engine
│   │   ├── auto_completer.*    # Auto-completion popup
│   │   └── code_folding_manager.* # Code folding engine
│   ├── ui/                     # User interface
│   │   ├── main_window.*       # Primary application window
│   │   ├── top_editor_container.* # Split-view editor container
│   │   ├── editor_tab_widget.* # Tab widget for editors
│   │   ├── status_bar_manager.* # Status bar widgets
│   │   ├── toolbar_manager.*   # Configurable toolbar
│   │   ├── icon_provider.*     # Centralised icon loading
│   │   ├── svg_icon_engine.*   # Palette-aware SVG icon engine
│   │   ├── dialogs/            # Modal dialogs
│   │   │   ├── about_dialog.*
│   │   │   ├── preferences_dialog.*
│   │   │   ├── encoding_chooser_dialog.*
│   │   │   ├── indentation_dialog.*
│   │   │   ├── go_to_line_dialog.*
│   │   │   └── run_command_dialog.*
│   │   └── dock/               # Dock widgets
│   │       ├── file_tree_dock.*
│   │       ├── terminal_dock.*
│   │       └── output_dock.*
│   ├── search/                 # Search and replace subsystem
│   │   ├── search_replace_dialog.* # In-document search dialog
│   │   ├── advanced_search_dock.*  # Multi-file search dock
│   │   ├── search_helpers.*    # Core find/replace logic
│   │   ├── search_instance.*   # Search configuration value type
│   │   ├── search_worker.*     # Background file search worker
│   │   └── search_result_model.* # Tree model for search results
│   ├── sessions/               # Session management
│   │   ├── session_manager.*   # Save/restore sessions (XML)
│   │   ├── backup_service.*    # Autosave and crash recovery
│   │   └── persistent_cache.*  # On-disk cache for unsaved content
│   ├── extensions/             # Extension/plugin system
│   │   ├── extension.*         # Single extension metadata + lifecycle
│   │   ├── extension_loader.*  # Discovery and loading service
│   │   ├── extension_server.*  # Extension host process manager
│   │   └── extension_api.*     # C++ API surface for extensions
│   ├── settings/               # Application settings
│   │   ├── novapad_settings.*  # Centralised settings facade
│   │   └── shortcut_manager.*  # Customisable keyboard shortcuts
│   ├── utils/                  # Utility libraries
│   │   ├── file_utils.*        # File I/O and DocEngine
│   │   ├── encoding_detector.* # Automatic encoding detection
│   │   ├── text_operations.*   # Text manipulation utilities
│   │   └── platform_utils.*    # Platform-specific helpers
│   ├── themes/                 # Colour themes
│   │   ├── theme_manager.*     # Theme registry and application
│   │   └── color_scheme.*      # Colour scheme data structure
│   └── resources/              # Qt resource files
│       └── novapad.qrc
├── tests/                      # Unit and integration tests
│   └── CMakeLists.txt
└── translations/               # Localisation files
```

---

## License

NovaPad is released under the **GNU General Public License v2.0** (GPL-2.0).

This project is a derivative work based on [Notepadqq](https://github.com/notepadqq/notepadqq), which is also licensed under GPL-2.0.

---

## Credits & Attribution

### Original Project
**[Notepadqq](https://notepadqq.com)** — Created by Daniele Di Sarli and contributors (2015-2019)
- GitHub: [notepadqq/notepadqq](https://github.com/notepadqq/notepadqq)
- Original icons and design inspiration

### NovaPad (This Fork)
**Maintainer:** Kamil 'Novik' Nowicki (2025-2026)
- Website: [https://noviktech.com](https://noviktech.com)
- GitHub: [https://github.com/novik133/novapad](https://github.com/novik133/novapad)

### Technologies
- Built with [Qt6](https://www.qt.io/) — The Qt Company
- Icons: Original Notepadqq icons + custom variants

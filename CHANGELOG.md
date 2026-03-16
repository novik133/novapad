# Changelog

All notable changes to NovaPad will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

#### Language Support (Major Feature)
- **Comprehensive language support system** with modular architecture
- **17 programming languages** with syntax highlighting, brace matching, and indentation:
  - **C** - C89/C99/C11/C17/C23 support with preprocessor directives
  - **C++** - C++23 features including concepts, modules, coroutines, raw strings
  - **C#** - .NET 6+ support with records, nullable types, LINQ, interpolated strings
  - **Rust** - Ownership system, lifetimes, macros, async/await
  - **Go** - Go 1.18+ generics, goroutines, struct tags
  - **Python** - Python 3.10+ with type hints, match/case, f-strings
  - **JavaScript/TypeScript** - ES2023, JSX/TSX, template literals, optional chaining
  - **Kotlin** - Coroutines, multiplatform features
  - **Swift** - Swift 5.9+ with actors, property wrappers
  - **PHP** - PHP 8.2+ attributes, union types, nullsafe operator
  - **Perl** - Regex, heredocs, POD documentation
  - **HTML** - Tag matching, attributes, entities
  - **CSS/SCSS** - Properties, values, pseudo-classes, CSS variables
  - **JSON** - JSON5/JSONC comment support
  - **XML** - Namespaces, processing instructions
  - **Markdown** - CommonMark with tables, task lists, footnotes
  - **SQL** - MySQL, PostgreSQL, SQLite dialects, window functions

#### Language Infrastructure
- `LanguageSupport` - Abstract base interface for all languages
- `CFamilyLanguageSupport` - Shared base for C-style languages
- `LanguageRegistry` - Central registry with language detection
- `AdvancedSyntaxHighlighter` - Language-aware syntax highlighting engine
- `LanguageSystemInitializer` - Application startup initialization

#### Language Features
- Regex-based token rules with priority system
- Multi-line construct highlighting (block comments, strings)
- Brace matching: curly `{}`, round `()`, square `[]`, angle `<>`
- Smart indentation with configurable triggers
- Auto-completion trigger configuration
- Language detection via:
  - File extensions (e.g., `.cpp`, `.py`, `.rs`)
  - Filenames (e.g., `Cargo.toml`, `package.json`)
  - Shebang patterns (e.g., `#!/usr/bin/env python3`)

### Changed

#### Build System
- Updated `src/CMakeLists.txt` to include 40+ new language support source files
- Organized sources into `NOVAPAD_LANGUAGES_BASE_SOURCES`, `NOVAPAD_LANGUAGES_DEFINITIONS_SOURCES`, and `NOVAPAD_LANGUAGES_SUPPORT_SOURCES`

#### Project Structure
- New directory structure under `src/languages/`:
  - `base/` - Base interfaces and C-family support
  - `definitions/` - Individual language implementations
  - `support/` - Registry, highlighter, and utilities

### Documentation
- Added comprehensive language support documentation (`docs/LANGUAGE_SUPPORT.md`)
  - Architecture overview
  - Complete API reference
  - Usage examples
  - Contributing guidelines
  - Future enhancement roadmap

## [1.0.0] - 2026-03-16

### Added
- Initial NovaPad release
- Qt6-based cross-platform code editor
- Basic editor functionality with syntax highlighting
- Multi-document tab interface
- Search and replace capabilities
- Session management and persistent cache
- Extension/plugin system infrastructure
- Theme and color scheme support
- Settings management with `QSettings`
- File tree dock panel
- Terminal integration dock
- Output/console dock
- Minimap widget for code overview
- Line number display
- Bracket matching
- Auto-completion infrastructure
- Multi-instance support with single instance enforcement

### Technical Details
- C++23 standard compliance
- Qt 6.10+ required
- CMake 3.25+ build system
- Modular architecture with clean separation of concerns
- SVG icon support with custom icon engine

---

## Release Planning

### [1.1.0] - Planned

#### Language Enhancements
- **Semantic Analysis**: Parser-based highlighting for accurate token recognition
- **Code Folding**: Integration with `CodeFoldingManager` for all supported languages
- **Auto-Completion**: Context-aware completion using document symbols

#### New Languages
- Ruby (`.rb`, `.erb`)
- Java (`.java`)
- Scala (`.scala`)
- Haskell (`.hs`)
- Lua (`.lua`)

### [1.2.0] - Planned

#### Advanced Features
- **Language Server Protocol (LSP)** integration
  - Diagnostics
  - Code actions
  - Go-to-definition
  - Find references
  - Refactoring support

#### Editor Enhancements
- Rainbow brackets coloring
- Indentation guides
- Column rulers
- Invisible character display

### [2.0.0] - Future

#### Major Architecture Changes
- Tree-sitter integration for incremental parsing
- Background thread syntax highlighting
- Memory optimization with shared pattern cache
- Full LSP client implementation

---

## Contributing

When adding new features or fixing bugs:

1. Update this changelog under `[Unreleased]` section
2. Follow [Keep a Changelog](https://keepachangelog.com/) format
3. Categorize changes as:
   - `Added` - New features
   - `Changed` - Changes to existing functionality
   - `Deprecated` - Soon-to-be removed features
   - `Removed` - Removed features
   - `Fixed` - Bug fixes
   - `Security` - Security-related changes

---

*For detailed technical documentation, see `docs/LANGUAGE_SUPPORT.md`*

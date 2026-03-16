# NovaPad Language Support Documentation

## Overview

NovaPad is a modern Qt6-based code editor with comprehensive language support for 17+ programming languages. The language support system is built on a modular, extensible architecture that enables syntax highlighting, brace matching, indentation rules, and auto-completion.

## Implemented Language Support

### Core Architecture

**Base Classes** (`src/languages/base/`):
- `LanguageSupport` - Abstract base interface defining all language capabilities
- `CFamilyLanguageSupport` - Shared implementation for C-style languages

**Support Infrastructure** (`src/languages/support/`):
- `LanguageRegistry` - Central registry for language detection and lookup
- `AdvancedSyntaxHighlighter` - Language-aware syntax highlighting engine
- `LanguageSystemInitializer` - Application startup initialization

### Supported Languages

| Language | Files | Features |
|----------|-------|----------|
| **C** | `.c`, `.h` | C89/C99/C11/C17/C23, preprocessor, generic selection |
| **C++** | `.cpp`, `.hpp`, `.cc`, `.cxx` | C++23 features, concepts, modules, coroutines, raw strings |
| **C#** | `.cs`, `.csx` | .NET 6+, records, nullable types, LINQ, interpolated strings |
| **Rust** | `.rs` | Ownership, lifetimes, macros, async/await, raw strings |
| **Go** | `.go` | Go 1.18+ generics, goroutines, channels, struct tags |
| **Python** | `.py`, `.pyw`, `.pyi` | Python 3.10+, type hints, match/case, f-strings |
| **JavaScript/TypeScript** | `.js`, `.jsx`, `.ts`, `.tsx` | ES2023, JSX/TSX, template literals, optional chaining |
| **Kotlin** | `.kt`, `.kts` | Coroutines, multiplatform, null-safety |
| **Swift** | `.swift` | Swift 5.9+, actors, property wrappers, result builders |
| **PHP** | `.php`, `.phtml` | PHP 8.2+, attributes, union types, nullsafe operator |
| **Perl** | `.pl`, `.pm`, `.t` | Regex, heredocs, POD documentation |
| **HTML** | `.html`, `.htm` | Tags, attributes, entities, CDATA |
| **CSS/SCSS** | `.css`, `.scss`, `.sass`, `.less` | Properties, values, pseudo-classes, variables |
| **JSON** | `.json`, `.jsonc`, `.json5` | Schema validation, JSON5 comments |
| **XML** | `.xml`, `.xsl`, `.svg`, `.xsd` | Namespaces, processing instructions |
| **Markdown** | `.md`, `.markdown` | CommonMark, tables, task lists, footnotes |
| **SQL** | `.sql`, `.ddl`, `.dml` | MySQL, PostgreSQL, SQLite dialects, window functions |

## Features Implemented

### Syntax Highlighting
- Regex-based token rules with priority system
- Multi-line constructs (block comments, strings, heredocs)
- Language-specific tokens (keywords, types, functions, operators)
- Dynamic rule reloading on language change

### Brace Matching
- Curly braces `{}` for blocks, classes, namespaces
- Round parentheses `()` for expressions, function calls
- Square brackets `[]` for arrays, indexing
- Angle brackets `<>` for templates/generics
- HTML/XML tag matching

### Indentation
- Smart indentation with trigger keywords
- Tab vs space configuration per language
- Indent after braces, colons, or specific keywords
- Outdent on return/break/continue statements

### Auto-Completion Triggers
- Member access (`.`)
- Pointer/reference access (`->`, `?.`, `::`)
- Function calls (`(`)
- Generics/templates (`<`)
- Lambda arrows (`=>`)

### Language Detection
- File extension mapping (e.g., `.cpp` → C++)
- Filename matching (e.g., `Cargo.toml` → Rust)
- Shebang pattern detection (e.g., `#!/usr/bin/env python3`)
- Content analysis for ambiguous cases

## Integration Points

### CMake Build System
All language support files are integrated into `src/CMakeLists.txt`:
- `NOVAPAD_LANGUAGES_BASE_SOURCES` - Base interfaces and C-family
- `NOVAPAD_LANGUAGES_DEFINITIONS_SOURCES` - 17 language implementations
- `NOVAPAD_LANGUAGES_SUPPORT_SOURCES` - Registry and highlighter

### Editor Integration
The `AdvancedSyntaxHighlighter` class extends `QSyntaxHighlighter` and can be:
1. Set on any `QTextDocument`
2. Configured with a language ID via `setLanguage()`
3. Automatically rehighlighted when language changes

## Future Enhancements

### Priority: High

1. **Semantic Analysis**
   - Parser-based syntax highlighting for accurate token recognition
   - Symbol resolution and type information
   - Go-to-definition and find-references
   - Real-time error detection

2. **Language Server Protocol (LSP)**
   - Integration with language servers for:
     - Advanced diagnostics
     - Code actions
     - Refactoring
     - Workspace symbols
     - Document symbols

3. **Code Folding**
   - Integration with existing `CodeFoldingManager`
   - Language-aware fold regions based on:
     - Brace matching (C-family)
     - Indentation (Python, YAML)
     - Markup tags (HTML, XML)
     - Comments (Markdown, docstrings)

4. **Auto-Completion Engine**
   - Context-aware completion using:
     - Language keywords and builtins
     - Document symbols
     - Project-wide index
     - Language server results
   - Snippet support with placeholders
   - Parameter hints

### Priority: Medium

5. **Additional Languages**
   - **Ruby** - `.rb`, `.erb`
   - **Java** - `.java`, `.class`
   - **Scala** - `.scala`, `.sbt`
   - **Haskell** - `.hs`, `.lhs`
   - **Lua** - `.lua`
   - **R** - `.r`, `.Rmd`
   - **MATLAB** - `.m`
   - **Julia** - `.jl`
   - **F#** - `.fs`, `.fsx`
   - **Elixir** - `.ex`, `.exs`
   - **Clojure** - `.clj`, `.cljs`
   - **Erlang** - `.erl`, `.hrl`
   - **V** - `.v`
   - **Zig** - `.zig`
   - **Nim** - `.nim`

6. **Enhanced Syntax Features**
   - Rainbow brackets/braces coloring
   - Indentation guides
   - Current line highlighting
   - Column rulers at specific positions
   - Invisible character display

7. **Documentation Integration**
   - Hover tooltips with documentation
   - Quick info panels
   - Integration with doc generators (Doxygen, JSDoc, Rustdoc)

### Priority: Low

8. **Configuration System**
   - User-defined language overrides
   - Custom syntax highlighting rules
   - Language-specific settings persistence
   - Import/export language definitions

9. **Performance Optimizations**
   - Incremental parsing for large files
   - Background thread highlighting
   - Lazy loading of language definitions
   - Memory pooling for token objects

10. **Testing Infrastructure**
    - Unit tests for each language parser
    - Syntax highlighting test suite with golden files
    - Performance benchmarks
    - Fuzzing for edge cases

## Architecture Improvements Needed

### 1. Parser-Based Highlighting
**Current**: Regex-based token rules
**Issue**: Limited accuracy, cannot handle nested structures
**Solution**: Integrate tree-sitter or similar incremental parser
```cpp
class TreeSitterHighlighter : public AdvancedSyntaxHighlighter {
    TSParser* m_parser;
    TSTree* m_tree;
    // Incremental parse on edit
};
```

### 2. Thread-Safe Document Access
**Current**: QSyntaxHighlighter runs on main thread
**Issue**: Large files cause UI blocking
**Solution**: Offload highlighting to background thread with chunked updates

### 3. Language Server Integration
**Current**: No LSP support
**Solution**: Add LSP client infrastructure
```cpp
class LspClient {
    void initialize(const QString& rootUri);
    void didOpen(const Document& doc);
    void didChange(const ChangeEvent& event);
    // Request-based features
};
```

### 4. Memory Optimization
**Current**: Each document creates its own highlighter
**Issue**: Duplicated regex patterns across documents
**Solution**: Shared pattern cache, flyweight pattern objects

### 5. Better Error Recovery
**Current**: Failed highlighting leaves document unformatted
**Solution**: Graceful degradation, partial highlighting on error

## File Organization

```
src/languages/
├── base/
│   ├── language_support_base.h/.cpp    # Abstract interface
│   └── c_family_base.h/.cpp              # C-style base class
├── definitions/
│   ├── c_language.h/.cpp                 # C implementation
│   ├── cpp_language.h/.cpp                 # C++ implementation
│   ├── cs_language.h/.cpp                  # C# implementation
│   ├── rust_language.h/.cpp                # Rust implementation
│   ├── go_language.h/.cpp                  # Go implementation
│   ├── python_language.h/.cpp              # Python implementation
│   ├── javascript_language.h/.cpp          # JavaScript/TypeScript
│   ├── kotlin_language.h/.cpp              # Kotlin implementation
│   ├── swift_language.h/.cpp               # Swift implementation
│   ├── php_language.h/.cpp                 # PHP implementation
│   ├── perl_language.h/.cpp                # Perl implementation
│   ├── html_css_language.h/.cpp            # HTML/CSS/JSON/XML
│   ├── markdown_language.h/.cpp            # Markdown implementation
│   └── sql_language.h/.cpp                 # SQL implementation
└── support/
    ├── language_registry.h/.cpp            # Central registry
    ├── advanced_syntax_highlighter.h/.cpp # Highlighter engine
    └── language_system_initializer.h/.cpp   # Startup init
```

## API Usage Example

```cpp
#include "languages/support/language_registry.h"
#include "languages/support/advanced_syntax_highlighter.h"

// Initialize at application startup
NovaPad::Languages::LanguageSystemInitializer langInit;

// Detect language from file
QString filePath = "/path/to/file.rs";
QString langId = NovaPad::Languages::LanguageRegistry::instance()
    .detectLanguage(filePath);

// Set up highlighter
auto* document = editor->document();
auto* highlighter = new NovaPad::Languages::AdvancedSyntaxHighlighter(document);
highlighter->setLanguage(langId);

// Get language info
const auto* lang = NovaPad::Languages::LanguageRegistry::instance()
    .getLanguage(langId);
if (lang) {
    auto braces = lang->bracePairs();
    auto comments = lang->commentRule();
    auto indentation = lang->indentationRule();
}
```

## Contributing New Languages

To add a new language:

1. **Create header file** in `src/languages/definitions/my_language.h`:
   - Inherit from `LanguageSupport` or `CFamilyLanguageSupport`
   - Implement all pure virtual methods
   - Declare language-specific metadata (id, name, extensions)

2. **Create implementation file** in `src/languages/definitions/my_language.cpp`:
   - Implement all virtual methods
   - Define token rules with appropriate colors
   - Configure brace pairs, comments, indentation

3. **Register in CMakeLists.txt**:
   - Add to `NOVAPAD_LANGUAGES_DEFINITIONS_SOURCES`

4. **Register in LanguageRegistry**:
   - Add factory function in `initializeBuiltinLanguages()`
   - Add extension mappings in `buildExtensionMap()`
   - Add filename mappings if applicable

5. **Test the implementation**:
   - Verify syntax highlighting
   - Check brace matching
   - Validate indentation
   - Test language detection

---

*Generated: March 2026*
*Version: 1.0*
*Maintainer: NovaPad Development Team*

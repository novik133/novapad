/**
 * @file syntax_highlighter.cpp
 * @brief Implementation of the native syntax highlighting engine.
 *
 * The highlighter uses a two-pass approach:
 *   1. Single-line rules (keywords, strings, numbers, operators) are applied
 *      in order via regex matching.
 *   2. Multi-line constructs (block comments, heredocs) are handled through
 *      QSyntaxHighlighter's block-state mechanism.
 *
 * Language-specific rule sets are built dynamically when setLanguage() is
 * called, keeping the highlighting performant even for large files.
 */

#include "syntax_highlighter.h"

#include <QColor>

namespace NovaPad::Editor {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    initDefaultFormats();
}

// ---------------------------------------------------------------------------
// Default colour scheme (dark-theme friendly)
// ---------------------------------------------------------------------------

void SyntaxHighlighter::initDefaultFormats()
{
    // Keywords — bold blue
    m_keywordFormat.setForeground(QColor("#569CD6"));
    m_keywordFormat.setFontWeight(QFont::Bold);

    // Built-in types — teal
    m_typeFormat.setForeground(QColor("#4EC9B0"));

    // String literals — warm orange
    m_stringFormat.setForeground(QColor("#CE9178"));

    // Comments — green / italic
    m_commentFormat.setForeground(QColor("#6A9955"));
    m_commentFormat.setFontItalic(true);

    // Numeric literals — light green
    m_numberFormat.setForeground(QColor("#B5CEA8"));

    // Function calls / definitions — yellow
    m_functionFormat.setForeground(QColor("#DCDCAA"));

    // Preprocessor directives — magenta
    m_preprocessorFormat.setForeground(QColor("#C586C0"));

    // Operators — light grey
    m_operatorFormat.setForeground(QColor("#D4D4D4"));

    // Annotations / decorators — gold
    m_annotationFormat.setForeground(QColor("#D7BA7D"));

    // Variables (shell $VAR, etc.) — light blue
    m_variableFormat.setForeground(QColor("#9CDCFE"));

    // HTML/XML tags
    m_tagFormat.setForeground(QColor("#569CD6"));

    // HTML/XML attributes
    m_attributeFormat.setForeground(QColor("#9CDCFE"));
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void SyntaxHighlighter::setLanguage(const Language *language)
{
    m_currentLanguage = language;
    m_rules.clear();
    m_commentStartExpr = {};
    m_commentEndExpr   = {};

    if (language) {
        buildRulesForLanguage(language);
    }

    rehighlight();
}

void SyntaxHighlighter::setColorScheme(const QMap<QString, QTextCharFormat> &scheme)
{
    if (scheme.contains("keyword"))      m_keywordFormat      = scheme["keyword"];
    if (scheme.contains("type"))         m_typeFormat         = scheme["type"];
    if (scheme.contains("string"))       m_stringFormat       = scheme["string"];
    if (scheme.contains("comment"))      m_commentFormat      = scheme["comment"];
    if (scheme.contains("number"))       m_numberFormat       = scheme["number"];
    if (scheme.contains("function"))     m_functionFormat     = scheme["function"];
    if (scheme.contains("preprocessor")) m_preprocessorFormat = scheme["preprocessor"];
    if (scheme.contains("operator"))     m_operatorFormat     = scheme["operator"];
    if (scheme.contains("annotation"))   m_annotationFormat   = scheme["annotation"];
    if (scheme.contains("variable"))     m_variableFormat     = scheme["variable"];

    // Rebuild rules with new formats if a language is active
    if (m_currentLanguage) {
        m_rules.clear();
        buildRulesForLanguage(m_currentLanguage);
        rehighlight();
    }
}

// ---------------------------------------------------------------------------
// Rule building — dispatches to language-specific helpers
// ---------------------------------------------------------------------------

void SyntaxHighlighter::buildRulesForLanguage(const Language *language)
{
    if (!language) return;

    const QString &mode = language->mode;

    // Determine which rule set to use based on the language mode / id
    if (mode == "clike" || language->id == "cpp" || language->id == "c"
        || language->id == "java" || language->id == "csharp"
        || language->id == "objective-c" || language->id == "kotlin"
        || language->id == "swift" || language->id == "dart"
        || language->id == "scala" || language->id == "typescript"
        || language->id == "javascript") {
        addCFamilyRules();
    } else if (language->id == "python") {
        addPythonRules();
    } else if (language->id == "rust") {
        addRustRules();
    } else if (language->id == "go") {
        addGoRules();
    } else if (language->id == "sql" || language->id == "plsql") {
        addSqlRules();
    } else if (language->id == "shell" || language->id == "bash") {
        addShellRules();
    } else if (language->id == "html" || language->id == "xml"
               || language->id == "css" || language->id == "php") {
        addWebRules();
    } else {
        // Fallback: add basic C-family rules — they cover most languages
        addCFamilyRules();
    }

    // -- Universal rules applied to all languages ----------------------------

    // Numeric literals (integers and floats, hex, binary, octal)
    m_rules.push_back({
        QRegularExpression(R"(\b(?:0[xX][0-9a-fA-F]+|0[bB][01]+|0[oO]?[0-7]+|\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)\b)"),
        m_numberFormat
    });

    // Double-quoted strings
    m_rules.push_back({
        QRegularExpression(R"("(?:[^"\\]|\\.)*")"),
        m_stringFormat
    });

    // Single-quoted strings
    m_rules.push_back({
        QRegularExpression(R"('(?:[^'\\]|\\.)*')"),
        m_stringFormat
    });

    // Single-line comments: //
    m_rules.push_back({
        QRegularExpression(R"(//[^\n]*)"),
        m_commentFormat
    });

    // Single-line comments: #
    m_rules.push_back({
        QRegularExpression(R"(#[^\n]*)"),
        m_commentFormat
    });

    // Multi-line comment delimiters for C-family
    m_commentStartExpr = QRegularExpression(R"(/\*)");
    m_commentEndExpr   = QRegularExpression(R"(\*/)");
}

// ---------------------------------------------------------------------------
// C-family rules (C, C++, Java, C#, JS, TS, Swift, Kotlin, Dart, Scala, …)
// ---------------------------------------------------------------------------

void SyntaxHighlighter::addCFamilyRules()
{
    // Keywords
    static const QStringList keywords = {
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand",
        "bitor", "bool", "break", "case", "catch", "char", "char8_t",
        "char16_t", "char32_t", "class", "compl", "concept", "const",
        "consteval", "constexpr", "constinit", "const_cast", "continue",
        "co_await", "co_return", "co_yield", "decltype", "default",
        "delete", "do", "double", "dynamic_cast", "else", "enum",
        "explicit", "export", "extern", "false", "float", "for",
        "friend", "goto", "if", "import", "inline", "int", "long",
        "module", "mutable", "namespace", "new", "noexcept", "not",
        "not_eq", "nullptr", "operator", "or", "or_eq", "private",
        "protected", "public", "register", "reinterpret_cast", "requires",
        "return", "short", "signed", "sizeof", "static", "static_assert",
        "static_cast", "struct", "switch", "template", "this", "throw",
        "true", "try", "typedef", "typeid", "typename", "union",
        "unsigned", "using", "virtual", "void", "volatile", "wchar_t",
        "while", "xor", "xor_eq",
        // JavaScript / TypeScript extras
        "abstract", "async", "await", "const", "debugger", "extends",
        "function", "implements", "in", "instanceof", "interface", "let",
        "of", "package", "super", "var", "yield", "type", "declare",
        // Java / C# / Kotlin extras
        "final", "finally", "native", "strictfp", "synchronized",
        "throws", "transient", "override", "sealed", "record",
        "fun", "val", "when", "object", "companion", "data",
    };

    for (const QString &kw : keywords) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(kw)),
            m_keywordFormat
        });
    }

    // Built-in types
    static const QStringList types = {
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "size_t", "ptrdiff_t", "intptr_t", "uintptr_t",
        "string", "String", "vector", "map", "set", "list",
        "Array", "Map", "Set", "Promise", "Observable",
        "List", "HashMap", "ArrayList", "Optional",
    };

    for (const QString &t : types) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(t)),
            m_typeFormat
        });
    }

    // Preprocessor directives
    m_rules.push_back({
        QRegularExpression(R"(^\s*#\s*\w+)"),
        m_preprocessorFormat
    });

    // Function declarations / calls
    m_rules.push_back({
        QRegularExpression(R"(\b([a-zA-Z_]\w*)\s*(?=\())"),
        m_functionFormat
    });
}

// ---------------------------------------------------------------------------
// Python rules
// ---------------------------------------------------------------------------

void SyntaxHighlighter::addPythonRules()
{
    static const QStringList keywords = {
        "False", "None", "True", "and", "as", "assert", "async", "await",
        "break", "class", "continue", "def", "del", "elif", "else",
        "except", "finally", "for", "from", "global", "if", "import",
        "in", "is", "lambda", "nonlocal", "not", "or", "pass", "raise",
        "return", "try", "while", "with", "yield", "match", "case",
    };

    for (const QString &kw : keywords) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(kw)),
            m_keywordFormat
        });
    }

    static const QStringList builtins = {
        "print", "len", "range", "type", "int", "float", "str", "list",
        "dict", "set", "tuple", "bool", "bytes", "bytearray",
        "enumerate", "zip", "map", "filter", "sorted", "reversed",
        "input", "open", "super", "isinstance", "issubclass",
        "hasattr", "getattr", "setattr", "property", "classmethod",
        "staticmethod", "abs", "all", "any", "max", "min", "sum",
    };

    for (const QString &fn : builtins) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(fn)),
            m_functionFormat
        });
    }

    // Decorators
    m_rules.push_back({
        QRegularExpression(R"(@\w+)"),
        m_annotationFormat
    });

    // f-strings prefix
    m_rules.push_back({
        QRegularExpression(R"(\bf(?=["']))"),
        m_preprocessorFormat
    });

    // Triple-quoted strings (simplified — does not span blocks)
    m_rules.push_back({
        QRegularExpression(R"(""".*?"""|'''.*?''')"),
        m_stringFormat
    });

    // Function definitions
    m_rules.push_back({
        QRegularExpression(R"(\bdef\s+(\w+))"),
        m_functionFormat
    });

    // self / cls
    m_rules.push_back({
        QRegularExpression(R"(\b(?:self|cls)\b)"),
        m_variableFormat
    });
}

// ---------------------------------------------------------------------------
// Rust rules
// ---------------------------------------------------------------------------

void SyntaxHighlighter::addRustRules()
{
    static const QStringList keywords = {
        "as", "async", "await", "break", "const", "continue", "crate",
        "dyn", "else", "enum", "extern", "false", "fn", "for", "if",
        "impl", "in", "let", "loop", "match", "mod", "move", "mut",
        "pub", "ref", "return", "self", "Self", "static", "struct",
        "super", "trait", "true", "type", "unsafe", "use", "where",
        "while", "yield",
    };

    for (const QString &kw : keywords) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(kw)),
            m_keywordFormat
        });
    }

    // Rust types
    static const QStringList types = {
        "i8", "i16", "i32", "i64", "i128", "isize",
        "u8", "u16", "u32", "u64", "u128", "usize",
        "f32", "f64", "bool", "char", "str",
        "String", "Vec", "Box", "Rc", "Arc", "Option", "Result",
        "HashMap", "HashSet", "BTreeMap", "BTreeSet",
    };

    for (const QString &t : types) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(t)),
            m_typeFormat
        });
    }

    // Macros (name!)
    m_rules.push_back({
        QRegularExpression(R"(\b\w+!)"),
        m_preprocessorFormat
    });

    // Lifetimes ('a)
    m_rules.push_back({
        QRegularExpression(R"('\w+)"),
        m_annotationFormat
    });

    // Attributes (#[...])
    m_rules.push_back({
        QRegularExpression(R"(#\[.*?\])"),
        m_annotationFormat
    });

    // Function definitions
    m_rules.push_back({
        QRegularExpression(R"(\bfn\s+(\w+))"),
        m_functionFormat
    });
}

// ---------------------------------------------------------------------------
// Go rules
// ---------------------------------------------------------------------------

void SyntaxHighlighter::addGoRules()
{
    static const QStringList keywords = {
        "break", "case", "chan", "const", "continue", "default", "defer",
        "else", "fallthrough", "for", "func", "go", "goto", "if",
        "import", "interface", "map", "package", "range", "return",
        "select", "struct", "switch", "type", "var",
    };

    for (const QString &kw : keywords) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(kw)),
            m_keywordFormat
        });
    }

    // Built-in types
    static const QStringList types = {
        "bool", "byte", "complex64", "complex128", "error", "float32",
        "float64", "int", "int8", "int16", "int32", "int64", "rune",
        "string", "uint", "uint8", "uint16", "uint32", "uint64", "uintptr",
    };

    for (const QString &t : types) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(t)),
            m_typeFormat
        });
    }

    // Built-in functions
    static const QStringList builtins = {
        "append", "cap", "close", "complex", "copy", "delete", "imag",
        "len", "make", "new", "panic", "print", "println", "real", "recover",
    };

    for (const QString &fn : builtins) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(fn)),
            m_functionFormat
        });
    }

    // func definitions
    m_rules.push_back({
        QRegularExpression(R"(\bfunc\s+(\w+))"),
        m_functionFormat
    });
}

// ---------------------------------------------------------------------------
// SQL rules
// ---------------------------------------------------------------------------

void SyntaxHighlighter::addSqlRules()
{
    static const QStringList keywords = {
        "SELECT", "FROM", "WHERE", "INSERT", "INTO", "VALUES", "UPDATE",
        "SET", "DELETE", "CREATE", "ALTER", "DROP", "TABLE", "INDEX",
        "VIEW", "DATABASE", "SCHEMA", "JOIN", "INNER", "LEFT", "RIGHT",
        "OUTER", "FULL", "CROSS", "ON", "AND", "OR", "NOT", "IN",
        "BETWEEN", "LIKE", "IS", "NULL", "AS", "ORDER", "BY", "GROUP",
        "HAVING", "UNION", "ALL", "DISTINCT", "LIMIT", "OFFSET",
        "EXISTS", "CASE", "WHEN", "THEN", "ELSE", "END", "BEGIN",
        "COMMIT", "ROLLBACK", "TRANSACTION", "GRANT", "REVOKE",
        "PRIMARY", "KEY", "FOREIGN", "REFERENCES", "CONSTRAINT",
        "DEFAULT", "CHECK", "UNIQUE", "CASCADE", "TRIGGER", "PROCEDURE",
        "FUNCTION", "DECLARE", "CURSOR", "FETCH", "OPEN", "CLOSE",
    };

    for (const QString &kw : keywords) {
        // SQL keywords are case-insensitive
        m_rules.push_back({
            QRegularExpression(
                QStringLiteral("\\b%1\\b").arg(kw),
                QRegularExpression::CaseInsensitiveOption),
            m_keywordFormat
        });
    }

    // SQL single-line comment
    m_rules.push_back({
        QRegularExpression(R"(--[^\n]*)"),
        m_commentFormat
    });
}

// ---------------------------------------------------------------------------
// Shell / Bash rules
// ---------------------------------------------------------------------------

void SyntaxHighlighter::addShellRules()
{
    static const QStringList keywords = {
        "if", "then", "else", "elif", "fi", "case", "esac", "for",
        "while", "until", "do", "done", "in", "function", "select",
        "time", "coproc", "return", "exit", "break", "continue",
        "local", "declare", "typeset", "readonly", "export", "unset",
        "shift", "source", "eval", "exec", "trap",
    };

    for (const QString &kw : keywords) {
        m_rules.push_back({
            QRegularExpression(QStringLiteral("\\b%1\\b").arg(kw)),
            m_keywordFormat
        });
    }

    // Shell variables ($VAR, ${VAR})
    m_rules.push_back({
        QRegularExpression(R"(\$\{?\w+\}?)"),
        m_variableFormat
    });

    // Shebang
    m_rules.push_back({
        QRegularExpression(R"(^#!.*)"),
        m_preprocessorFormat
    });
}

// ---------------------------------------------------------------------------
// Web rules (HTML / XML / CSS / PHP)
// ---------------------------------------------------------------------------

void SyntaxHighlighter::addWebRules()
{
    // HTML/XML tags
    m_rules.push_back({
        QRegularExpression(R"(</?[a-zA-Z][\w-]*(?:\s|>|/>))"),
        m_tagFormat
    });

    // HTML attributes
    m_rules.push_back({
        QRegularExpression(R"(\b[a-zA-Z][\w-]*(?==))"),
        m_attributeFormat
    });

    // CSS selectors (simplified)
    m_rules.push_back({
        QRegularExpression(R"([.#][\w-]+)"),
        m_annotationFormat
    });

    // CSS properties
    m_rules.push_back({
        QRegularExpression(R"([\w-]+(?=\s*:))"),
        m_variableFormat
    });

    // PHP opening/closing tags
    m_rules.push_back({
        QRegularExpression(R"(<\?php|\?>)"),
        m_preprocessorFormat
    });

    // HTML comment delimiters
    m_commentStartExpr = QRegularExpression(R"(<!--)");
    m_commentEndExpr   = QRegularExpression(R"(-->)");
}

// ---------------------------------------------------------------------------
// Core highlighting callback
// ---------------------------------------------------------------------------

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    // Pass 1: Apply single-line rules
    for (const auto &rule : m_rules) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            const QRegularExpressionMatch match = it.next();
            setFormat(static_cast<int>(match.capturedStart()),
                      static_cast<int>(match.capturedLength()),
                      rule.format);
        }
    }

    // Pass 2: Multi-line block comments
    if (!m_commentStartExpr.pattern().isEmpty()) {
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1) {
            const auto match = m_commentStartExpr.match(text);
            startIndex = match.hasMatch() ? static_cast<int>(match.capturedStart()) : -1;
        }

        while (startIndex >= 0) {
            const auto endMatch = m_commentEndExpr.match(text, startIndex);
            int endIndex   = endMatch.hasMatch() ? static_cast<int>(endMatch.capturedStart()) : -1;
            int commentLen = 0;

            if (endIndex == -1) {
                // Comment extends to end of block (and beyond)
                setCurrentBlockState(1);
                commentLen = text.length() - startIndex;
            } else {
                commentLen = endIndex - startIndex + static_cast<int>(endMatch.capturedLength());
            }

            setFormat(startIndex, commentLen, m_commentFormat);

            // Look for next comment start after the current one
            const auto nextStart = m_commentStartExpr.match(text, startIndex + commentLen);
            startIndex = nextStart.hasMatch() ? static_cast<int>(nextStart.capturedStart()) : -1;
        }
    }
}

} // namespace NovaPad::Editor

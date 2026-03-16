/**
 * @file python_language.cpp
 * @brief Implementation of Python language support.
 */

#include "python_language.h"

namespace NovaPad::Languages {

QStringList PythonLanguageSupport::keywords() const
{
    return {
        "False", "None", "True", "and", "as", "assert", "async", "await",
        "break", "class", "continue", "def", "del", "elif", "else", "except",
        "finally", "for", "from", "global", "if", "import", "in", "is",
        "lambda", "match", "case", "nonlocal", "not", "or", "pass", "raise",
        "return", "try", "while", "with", "yield"
    };
}

QStringList PythonLanguageSupport::builtinTypes() const
{
    return {
        "bool", "int", "float", "complex", "str", "bytes", "bytearray",
        "list", "tuple", "dict", "set", "frozenset", "range", "slice",
        "object", "type", "NoneType", "NotImplementedType", "EllipsisType",
        "Exception", "BaseException", "ArithmeticError", "LookupError",
        "AssertionError", "AttributeError", "EOFError", "FloatingPointError",
        "GeneratorExit", "ImportError", "ModuleNotFoundError", "IndexError",
        "KeyError", "KeyboardInterrupt", "MemoryError", "NameError",
        "NotImplementedError", "OSError", "OverflowError", "RecursionError",
        "ReferenceError", "RuntimeError", "StopIteration", "StopAsyncIteration",
        "SyntaxError", "IndentationError", "TabError", "SystemError",
        "SystemExit", "TypeError", "UnboundLocalError", "UnicodeError",
        "UnicodeEncodeError", "UnicodeDecodeError", "UnicodeTranslateError",
        "ValueError", "ZeroDivisionError"
    };
}

QStringList PythonLanguageSupport::shebangPatterns() const
{
    return {
        "#!/usr/bin/env python",
        "#!/usr/bin/env python3",
        "#!/usr/bin/python",
        "#!/usr/bin/python3",
        "#!/usr/local/bin/python",
        "#!/usr/local/bin/python3"
    };
}

std::vector<BracePair> PythonLanguageSupport::bracePairs() const
{
    // Python uses indentation for blocks, but has brackets for data structures
    return {
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {}),
        BracePair('{', '}', BraceType::Curly, true, {})
    };
}

CommentRule PythonLanguageSupport::commentRule() const
{
    return CommentRule("#", {}, {}, false);
}

QStringList PythonLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "\"\"\"", "'''", "r\"", "r'", "f\"", "f'", "b\"", "b'"};
}

IndentationRule PythonLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 4;
    rule.smartIndent = true;
    rule.indentAfterBrace = false;  // Uses colon instead
    rule.indentAfterColon = true;   // Indent after colon for blocks
    rule.indentTriggers = {"def", "class", "if", "elif", "else", "for", "while",
                          "try", "except", "finally", "with", "async", "match", "case"};
    rule.outdentTriggers = {"return", "break", "continue", "pass", "raise", "yield"};
    return rule;
}

std::vector<TokenRule> PythonLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // Keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor{"#0000FF"});
    keywordFormat.setFontWeight(QFont::Bold);
    QString kw = keywords().join("|");
    rules.emplace_back("keyword",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(kw)},
        keywordFormat, false, 100);
    
    // Decorators @...
    QTextCharFormat decoratorFormat;
    decoratorFormat.setForeground(QColor{"#A52A2A"});
    decoratorFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("decorator",
        QRegularExpression{R"(^\s*@\w+(?:\.\w+)*)"},
        decoratorFormat, false, 95);
    
    // f-strings with embedded expressions
    QTextCharFormat fstringFormat;
    fstringFormat.setForeground(QColor{"#20B2AA"});  // Light sea green
    rules.emplace_back("fstring",
        QRegularExpression{R"((?:f|F)(?:\"\"\"|'''|\"|')(?:[^"\\{}]|\{[^}]*\})*(?:\"\"\"|'''|\"|'))"},
        fstringFormat, true, 90);
    
    // Regular strings
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor{"#008000"});
    rules.emplace_back("string_triple",
        QRegularExpression{R"((?:r|R)?(?:\"\"\"|''')(?:[^\\]|\\.)*?(?:\"\"\"|'''))"},
        stringFormat, true, 89);
    rules.emplace_back("string_single",
        QRegularExpression{R"((?:r|R)?(?:\"|')(?:[^"'\\\n]|\\.)*?(?:\"|'))"},
        stringFormat, false, 88);
    
    // Byte strings
    QTextCharFormat bytesFormat;
    bytesFormat.setForeground(QColor{"#6B8E23"});
    rules.emplace_back("bytes",
        QRegularExpression{R"((?:b|B)(?:r|R)?(?:\"\"\"|'''|\"|')(?:[^"'\\]|\\.)*?(?:\"\"\"|'''|\"|'))"},
        bytesFormat, true, 87);
    
    // Types and built-ins
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        typeFormat, false, 85);
    
    // Built-in functions
    QTextCharFormat builtinFormat;
    builtinFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("builtin",
        QRegularExpression{R"(\b(?:abs|all|any|ascii|bin|bool|breakpoint|bytearray|bytes|callable|chr|classmethod|compile|complex|delattr|dict|dir|divmod|enumerate|eval|exec|filter|float|format|frozenset|getattr|globals|hasattr|hash|help|hex|id|input|int|isinstance|issubclass|iter|len|list|locals|map|max|memoryview|min|next|object|oct|open|ord|pow|print|property|range|repr|reversed|round|set|setattr|slice|sorted|staticmethod|str|sum|super|tuple|type|vars|zip|__import__)\s*\()"},
        builtinFormat, false, 80);
    
    // Type hints/annotations
    QTextCharFormat annotationFormat;
    annotationFormat.setForeground(QColor{"#9932CC"});  // Dark orchid
    rules.emplace_back("annotation",
        QRegularExpression{R"(\b\w+\s*:\s*[A-Z][a-zA-Z0-9_\[\]]*)"},
        annotationFormat, false, 75);
    
    // Walrus operator :=
    QTextCharFormat walrusFormat;
    walrusFormat.setForeground(QColor{"#FF1493"});
    walrusFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("walrus",
        QRegularExpression{R"(:=)"},
        walrusFormat, false, 70);
    
    // Numbers
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("number",
        QRegularExpression{R"(\b(?:0[xX][0-9a-fA-F_]+|0[oO][0-7_]+|0[bB][01_]+|[0-9][0-9_]*)(?:\.[0-9_]+)?(?:[eE][+-]?[0-9_]+)?[jJ]?\b)"},
        numberFormat, false, 65);
    
    // Self and cls
    QTextCharFormat selfFormat;
    selfFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("self",
        QRegularExpression{R"(\b(?:self|cls)\b)"},
        selfFormat, false, 60);
    
    // Magic methods __xxx__
    QTextCharFormat magicFormat;
    magicFormat.setForeground(QColor{"#9370DB"});
    rules.emplace_back("magic",
        QRegularExpression{R"(\b__[a-zA-Z_][a-zA-Z0-9_]*__\b)"},
        magicFormat, false, 55);
    
    // Docstrings
    QTextCharFormat docstringFormat;
    docstringFormat.setForeground(QColor{"#808080"});
    docstringFormat.setFontItalic(true);
    rules.emplace_back("docstring",
        QRegularExpression{R"(^\s*(?:class|def|async\s+def)\s+\w+[^:]*:\s*\n\s*(?:\"\"\"|''')(?:[^\\]|\\.)*?(?:\"\"\"|'''))"},
        docstringFormat, true, 50);
    
    // Operators
    QTextCharFormat opFormat;
    opFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("operator",
        QRegularExpression{R"([+\-*/%=<>!&|^~@]+)"},
        opFormat, false, 40);
    
    // Comments
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("comment",
        QRegularExpression{R"(#[^\n]*)"},
        commentFormat, false, 30);
    
    return rules;
}

std::vector<AutoCompleteTrigger> PythonLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"." , true, 0, {"after_identifier"}},
        {"(", false, 0, {"after_identifier"}},
        {":", false, 0, {"after_identifier"}},  // Type annotation
        {"->", false, 0, {"after_parenthesis"}},  // Return type
        {"=", false, 1, {"after_identifier"}},  // Assignment
        {"f\"", false, 0, {"after_operator"}},  // f-string
        {"r\"", false, 0, {"after_operator"}},  // raw string
        {"b\"", false, 0, {"after_operator"}}   // bytes
    };
}

} // namespace NovaPad::Languages

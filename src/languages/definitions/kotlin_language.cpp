/**
 * @file kotlin_language.cpp
 * @brief Implementation of Kotlin language support.
 */

#include "kotlin_language.h"

namespace NovaPad::Languages {

QStringList KotlinLanguageSupport::keywords() const
{
    return {
        "abstract", "actual", "annotation", "as", "break", "by", "catch",
        "class", "companion", "const", "constructor", "continue", "crossinline",
        "data", "delegate", "do", "dynamic", "else", "enum", "expect",
        "external", "field", "file", "final", "finally", "for", "fun",
        "get", "if", "import", "in", "infix", "init", "inline", "inner",
        "interface", "internal", "is", "it", "lateinit", "noinline", "null",
        "object", "open", "operator", "out", "override", "package", "param",
        "private", "property", "protected", "public", "receiver", "reified",
        "return", "sealed", "set", "setparam", "suspend", "tailrec", "this",
        "throw", "to", "true", "try", "typealias", "typeof", "val", "var",
        "vararg", "when", "where", "while"
    };
}

QStringList KotlinLanguageSupport::builtinTypes() const
{
    return {
        "Boolean", "Byte", "Char", "Short", "Int", "Long", "Float", "Double",
        "String", "Any", "Unit", "Nothing", "Array", "ByteArray", "CharArray",
        "ShortArray", "IntArray", "LongArray", "FloatArray", "DoubleArray",
        "BooleanArray", "List", "MutableList", "Set", "MutableSet", "Map",
        "MutableMap", "Collection", "MutableCollection", "Iterable", "Sequence",
        "Comparator", "Function", "Throwable", "Exception", "RuntimeException",
        "IllegalArgumentException", "IllegalStateException", "IndexOutOfBoundsException",
        "NullPointerException", "NumberFormatException", "UnsupportedOperationException",
        "ClassCastException", "AssertionError", "OutOfMemoryError", "StackOverflowError"
    };
}

QStringList KotlinLanguageSupport::stringDelimiters() const
{
    return {"\"", "\"\"\"", "'"};  // Regular, multi-line, character
}

IndentationRule KotlinLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 4;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"fun", "class", "interface", "object", "if", "else", "when",
                          "for", "while", "do", "try", "catch", "finally"};
    rule.outdentTriggers = {"return", "break", "continue", "throw"};
    return rule;
}

std::vector<TokenRule> KotlinLanguageSupport::tokenRules() const
{
    auto rules = buildCommonRules();
    
    // Kotlin-specific keywords
    QTextCharFormat kotlinFormat;
    kotlinFormat.setForeground(QColor{"#B7410E"});  // Rust orange
    rules.emplace_back("kotlin",
        QRegularExpression{R"(\b(?:data|sealed|inline|crossinline|noinline|reified|suspend|tailrec|operator|infix|external|actual|expect|companion|object|when|by|it)\b)"},
        kotlinFormat, false, 92);
    
    // Multi-line strings """..."""
    QTextCharFormat multilineFormat;
    multilineFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("multiline_string",
        QRegularExpression{R"("""[\s\S]*?""")"},
        multilineFormat, true, 90);
    
    // String templates $var and ${expr}
    QTextCharFormat templateFormat;
    templateFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("template",
        QRegularExpression{R"(\$\w+|\$\{[^}]*\})"},
        templateFormat, false, 88);
    
    // Function declarations
    QTextCharFormat funcFormat;
    funcFormat.setForeground(QColor{"#800080"});
    rules.emplace_back("function",
        QRegularExpression{R"(\bfun\s+(\w+))"},
        funcFormat, false, 85);
    
    // Type annotations
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        typeFormat, false, 80);
    
    // Nullable types ?
    QTextCharFormat nullableFormat;
    nullableFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("nullable",
        QRegularExpression{R"(\w+\?)"},
        nullableFormat, false, 75);
    
    // Annotations @...
    QTextCharFormat annoFormat;
    annoFormat.setForeground(QColor{"#808080"});
    annoFormat.setFontItalic(true);
    rules.emplace_back("annotation",
        QRegularExpression{R"(@\w+(?:::\w+)?)"},
        annoFormat, false, 70);
    
    // Lambda expressions {}
    QTextCharFormat lambdaFormat;
    lambdaFormat.setForeground(QColor{"#9370DB"});
    rules.emplace_back("lambda",
        QRegularExpression{R"(\{[^}]*->[^}]*\})"},
        lambdaFormat, false, 65);
    
    // Elvis operator ?: 
    QTextCharFormat elvisFormat;
    elvisFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("elvis",
        QRegularExpression{R"(\?\:)"},
        elvisFormat, false, 60);
    
    // Safe call ?.
    rules.emplace_back("safe_call",
        QRegularExpression{R"(\?\.)"},
        elvisFormat, false, 60);
    
    return rules;
}

std::vector<AutoCompleteTrigger> KotlinLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"." , true, 0, {"after_identifier"}},
        {"?.", true, 0, {"after_identifier"}},
        {"?:", true, 0, {"after_identifier"}},
        {"::", true, 0, {"after_identifier"}},
        {"(", false, 0, {"after_identifier"}},
        {"<", false, 0, {"after_identifier"}},
        {"{", false, 0, {"after_arrow"}},
        {"->", true, 0, {"in_lambda"}},
        {"${", true, 0, {"in_string"}}
    };
}

} // namespace NovaPad::Languages

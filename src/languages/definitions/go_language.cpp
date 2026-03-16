/**
 * @file go_language.cpp
 * @brief Implementation of Go language support.
 */

#include "go_language.h"

namespace NovaPad::Languages {

QStringList GoLanguageSupport::keywords() const
{
    return {
        "break", "case", "chan", "const", "continue", "default", "defer",
        "else", "fallthrough", "for", "func", "go", "goto", "if", "import",
        "interface", "map", "package", "range", "return", "select", "struct",
        "switch", "type", "var",
        // Predeclared identifiers (not keywords but reserved)
        "any", "bool", "byte", "comparable", "complex64", "complex128", "error",
        "float32", "float64", "int", "int8", "int16", "int32", "int64",
        "make", "new", "nil", "panic", "print", "println", "real", "recover",
        "rune", "string", "true", "false", "uint", "uint8", "uint16", "uint32",
        "uint64", "uintptr"
    };
}

QStringList GoLanguageSupport::builtinTypes() const
{
    return {
        "bool", "byte", "complex64", "complex128", "error", "float32",
        "float64", "int", "int8", "int16", "int32", "int64", "rune",
        "string", "uint", "uint8", "uint16", "uint32", "uint64", "uintptr",
        "any", "comparable"
    };
}

std::vector<BracePair> GoLanguageSupport::bracePairs() const
{
    return {
        BracePair('{', '}', BraceType::Curly, true, {}),
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {})
    };
}

CommentRule GoLanguageSupport::commentRule() const
{
    return CommentRule("//", "/*", "*/", false);
}

QStringList GoLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "`"};  // Raw string literals with backticks
}

IndentationRule GoLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = true;  // Go standard uses tabs
    rule.tabSize = 8;     // Traditional Go tab width
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"func", "if", "else", "for", "switch", "select", "case", "struct", "interface", "type"};
    rule.outdentTriggers = {"return", "break", "continue", "goto", "fallthrough"};
    return rule;
}

std::vector<TokenRule> GoLanguageSupport::tokenRules() const
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
    
    // Raw string literals `...`
    QTextCharFormat rawStringFormat;
    rawStringFormat.setForeground(QColor{"#008000"});
    rules.emplace_back("raw_string",
        QRegularExpression{R"(`[^`]*`)"},
        rawStringFormat, true, 90);
    
    // Regular strings
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor{"#008000"});
    rules.emplace_back("string",
        QRegularExpression{R"("[^"\\]*(?:\\.[^"\\]*)*")"},
        stringFormat, false, 89);
    rules.emplace_back("char",
        QRegularExpression{R"('[^'\\]*(?:\\.[^'\\]*)*')"},
        stringFormat, false, 88);
    
    // Types
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        typeFormat, false, 85);
    
    // Functions - func name(
    QTextCharFormat funcFormat;
    funcFormat.setForeground(QColor{"#800080"});
    rules.emplace_back("function",
        QRegularExpression{R"(\bfunc\s+(\w+))"},
        funcFormat, false, 80);
    
    // Method calls and function calls
    QTextCharFormat methodFormat;
    methodFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("method",
        QRegularExpression{R"(\b\w+\s*\()"},
        methodFormat, false, 75);
    
    // Struct tags `json:"..."`
    QTextCharFormat tagFormat;
    tagFormat.setForeground(QColor{"#808080"});
    rules.emplace_back("struct_tag",
        QRegularExpression{R"(`[\w:]+"[^"]*"`)"},
        tagFormat, false, 70);
    
    // Numbers
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("number",
        QRegularExpression{R"(\b(?:0[xX][0-9a-fA-F]+|0[0-7]+|0[bB][01]+|[0-9]+)(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?[i]?\b)"},
        numberFormat, false, 65);
    
    // Operators
    QTextCharFormat opFormat;
    opFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("operator",
        QRegularExpression{R"([+\-*/%=<>!&|^~:]+)"},
        opFormat, false, 50);
    
    // Comments
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("line_comment",
        QRegularExpression{R"(//[^\n]*)"},
        commentFormat, false, 40);
    rules.emplace_back("block_comment",
        QRegularExpression{R"(/\*.*?\*/)"},
        commentFormat, true, 40);
    
    // Package declaration
    QTextCharFormat packageFormat;
    packageFormat.setForeground(QColor{"#A52A2A"});
    packageFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("package",
        QRegularExpression{R"(\bpackage\s+\w+)"},
        packageFormat, false, 95);
    
    // Import statements
    QTextCharFormat importFormat;
    importFormat.setForeground(QColor{"#9932CC"});
    rules.emplace_back("import",
        QRegularExpression{R"(\bimport\s+(?:\(|\"))"},
        importFormat, false, 94);
    
    return rules;
}

std::vector<AutoCompleteTrigger> GoLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"." , true, 0, {"after_identifier"}},
        {"(", false, 0, {"after_identifier"}},
        {"{", false, 0, {"after_func", "after_struct", "after_interface", "after_if", "after_for", "after_switch"}},
        {":=", true, 0, {"after_identifier"}},  // Short variable declaration
        {"<-", true, 0, {"after_chan"}},  // Channel operations
        {"make(", false, 0, {"after_operator"}},
        {"new(", false, 0, {"after_operator"}}
    };
}

} // namespace NovaPad::Languages

/**
 * @file c_family_base.cpp
 * @brief Implementation of common C-family language support.
 */

#include "c_family_base.h"

namespace NovaPad::Languages {

std::vector<BracePair> CFamilyLanguageSupport::bracePairs() const
{
    return {
        // Curly braces for blocks, classes, namespaces
        BracePair('{', '}', BraceType::Curly, true, {}),
        // Parentheses for expressions, calls
        BracePair('(', ')', BraceType::Round, true, {}),
        // Square brackets for arrays, indexing, attributes
        BracePair('[', ']', BraceType::Square, true, {}),
        // Angle brackets for templates, generics
        BracePair('<', '>', BraceType::Angle, true, {})
    };
}

CommentRule CFamilyLanguageSupport::commentRule() const
{
    // Standard C-style comments
    return CommentRule("//", "/*", "*/", false);
}

QStringList CFamilyLanguageSupport::stringDelimiters() const
{
    return {"\"", "'"};
}

IndentationRule CFamilyLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 4;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentAfterColon = false;
    rule.indentTriggers = controlFlowKeywords();
    rule.outdentTriggers = {"break", "continue", "return", "throw", "goto"};
    return rule;
}

QStringList CFamilyLanguageSupport::controlFlowKeywords()
{
    return {
        "if", "else", "for", "while", "do", "switch", "case", "default",
        "try", "catch", "finally", "break", "continue", "return",
        "goto", "throw"
    };
}

QStringList CFamilyLanguageSupport::typeKeywords()
{
    return {
        "void", "bool", "char", "short", "int", "long", "float", "double",
        "signed", "unsigned", "auto", "const", "volatile", "static",
        "extern", "inline", "virtual", "explicit", "mutable"
    };
}

QStringList CFamilyLanguageSupport::storageKeywords()
{
    return {
        "static", "extern", "inline", "virtual", "explicit", "mutable",
        "constexpr", "consteval", "constinit"
    };
}

std::vector<TokenRule> CFamilyLanguageSupport::buildCommonRules() const
{
    std::vector<TokenRule> rules;
    
    // Preprocessor directives
    QTextCharFormat preprocessorFormat;
    preprocessorFormat.setForeground(QColor{"#A52A2A"});  // Brown/dark red
    preprocessorFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("preprocessor", 
        QRegularExpression{R"(^\s*#[ \t]*[a-zA-Z_]+)"}, 
        preprocessorFormat, false, 100);
    
    // Control flow keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor{"#0000FF"});  // Blue
    keywordFormat.setFontWeight(QFont::Bold);
    QString controlFlow = controlFlowKeywords().join("|");
    rules.emplace_back("keyword",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(controlFlow)},
        keywordFormat, false, 90);
    
    // Type keywords
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});  // Sea green
    QString types = typeKeywords().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        typeFormat, false, 80);
    
    // Numbers (integers, floats, hex, binary, octal)
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor{"#FF6600"});  // Orange
    rules.emplace_back("number",
        QRegularExpression{R"(\b(?:0[xX][0-9a-fA-F]+|0[bB][01]+|0[0-7]*|[1-9][0-9]*)(?:[uU][lL]?[lL]?|[lL][lL]?[uU]?)?\b|\b[0-9]+\.[0-9]+(?:[eE][+-]?[0-9]+)?[fF]?\b)"},
        numberFormat, false, 70);
    
    // Operators
    QTextCharFormat operatorFormat;
    operatorFormat.setForeground(QColor{"#FF00FF"});  // Magenta
    rules.emplace_back("operator",
        QRegularExpression{R"([+\-*/%=<>!&|^~]+)"},
        operatorFormat, false, 50);
    
    return rules;
}

} // namespace NovaPad::Languages

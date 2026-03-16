/**
 * @file php_language.cpp
 * @brief Implementation of PHP language support.
 */

#include "php_language.h"

namespace NovaPad::Languages {

QStringList PhpLanguageSupport::keywords() const
{
    return {
        "__halt_compiler", "abstract", "and", "array", "as", "break", "callable",
        "case", "catch", "class", "clone", "const", "continue", "declare",
        "default", "die", "do", "echo", "else", "elseif", "empty", "enddeclare",
        "endfor", "endforeach", "endif", "endswitch", "endwhile", "enum",
        "eval", "exit", "extends", "final", "finally", "fn", "for", "foreach",
        "function", "global", "goto", "if", "implements", "include",
        "include_once", "instanceof", "insteadof", "interface", "isset", "list",
        "match", "namespace", "new", "or", "print", "private", "protected",
        "public", "readonly", "require", "require_once", "return", "static",
        "switch", "throw", "trait", "try", "unset", "use", "var", "while",
        "xor", "yield", "from"
    };
}

QStringList PhpLanguageSupport::builtinTypes() const
{
    return {
        // Scalar types
        "bool", "int", "float", "string",
        // Special types
        "array", "object", "callable", "iterable", "void", "null",
        // PHP 8+ types
        "false", "true", "static", "self", "parent",
        // Union types (combinations)
        "array|object", "int|float", "string|null",
        // Built-in classes
        "stdClass", "Exception", "Error", "Throwable", "Stringable",
        "ArrayObject", "ArrayIterator", "SplFixedArray",
        "DateTime", "DateTimeImmutable", "DateTimeZone",
        "Closure", "Generator", "ReflectionClass",
        "SimpleXMLElement", "DOMDocument", "PDO", "mysqli"
    };
}

QStringList PhpLanguageSupport::shebangPatterns() const
{
    return {
        "#!/usr/bin/env php",
        "#!/usr/bin/php",
        "#!/usr/local/bin/php"
    };
}

QStringList PhpLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "<<<"};  // Heredoc/Nowdoc
}

IndentationRule PhpLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 4;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"function", "class", "interface", "trait", "namespace",
                          "if", "else", "elseif", "for", "foreach", "while",
                          "do", "switch", "case", "try", "catch", "finally"};
    rule.outdentTriggers = {"return", "break", "continue", "throw", "exit", "die"};
    return rule;
}

std::vector<TokenRule> PhpLanguageSupport::tokenRules() const
{
    auto rules = buildCommonRules();
    
    // PHP keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor{"#6C407E"});  // PHP purple
    keywordFormat.setFontWeight(QFont::Bold);
    QString kw = keywords().join("|");
    rules.emplace_back("keyword",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(kw)},
        keywordFormat, false, 100);
    
    // PHP opening/closing tags
    QTextCharFormat tagFormat;
    tagFormat.setForeground(QColor{"#FF0000"});
    tagFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("php_tag",
        QRegularExpression{R"(<\?(?:php|=)?|\?>)"},
        tagFormat, false, 98);
    
    // PHP 8 attributes #[...]
    QTextCharFormat attrFormat;
    attrFormat.setForeground(QColor{"#9932CC"});
    rules.emplace_back("attribute",
        QRegularExpression{R"(#\[.*?\])"},
        attrFormat, false, 95);
    
    // Heredoc/Nowdoc
    QTextCharFormat heredocFormat;
    heredocFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("heredoc",
        QRegularExpression{R"(<<<(['"]?)(\w+)\1[\s\S]*?^\2;)"},
        heredocFormat, true, 90);
    
    // Variables $var
    QTextCharFormat varFormat;
    varFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("variable",
        QRegularExpression{R"(\$\w+)"},
        varFormat, false, 85);
    
    // Types
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        typeFormat, false, 80);
    
    // Magic methods __construct, etc.
    QTextCharFormat magicFormat;
    magicFormat.setForeground(QColor{"#9370DB"});
    rules.emplace_back("magic",
        QRegularExpression{R"(\b__[a-zA-Z_][a-zA-Z0-9_]*__\b)"},
        magicFormat, false, 78);
    
    // Nullsafe operator ?->
    QTextCharFormat nullsafeFormat;
    nullsafeFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("nullsafe",
        QRegularExpression{R"(\?->)"},
        nullsafeFormat, false, 75);
    
    // Arrow functions fn() =>
    QTextCharFormat arrowFormat;
    arrowFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("arrow",
        QRegularExpression{R"(\bfn\s*\([^)]*\)\s*=>)"},
        arrowFormat, false, 73);
    
    // Match expression
    QTextCharFormat matchFormat;
    matchFormat.setForeground(QColor{"#FF69B4"});
    rules.emplace_back("match",
        QRegularExpression{R"(\bmatch\s*\()"},
        matchFormat, false, 72);
    
    // Named arguments name:
    QTextCharFormat namedArgFormat;
    namedArgFormat.setForeground(QColor{"#D2691E"});
    rules.emplace_back("named_arg",
        QRegularExpression{R"(\w+\s*:\s*(?!:))"},
        namedArgFormat, false, 70);
    
    // Namespaces and uses
    QTextCharFormat namespaceFormat;
    namespaceFormat.setForeground(QColor{"#A52A2A"});
    rules.emplace_back("namespace",
        QRegularExpression{R"(\b(?:namespace|use)\s+[\\\w]+)"},
        namespaceFormat, false, 68);
    
    // Superglobals
    QTextCharFormat superglobalFormat;
    superglobalFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("superglobal",
        QRegularExpression{R"(\$_(?:GET|POST|REQUEST|SERVER|SESSION|COOKIE|FILES|ENV|GLOBALS))"},
        superglobalFormat, false, 65);
    
    return rules;
}

std::vector<AutoCompleteTrigger> PhpLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"->", true, 0, {"after_identifier"}},
        {"?->", true, 0, {"after_identifier"}},
        {"::", true, 0, {"after_identifier"}},
        {"(", false, 0, {"after_identifier"}},
        {"=>", true, 0, {"after_arrow_fn"}},
        {"$", false, 0, {"after_operator"}},
        {"[", false, 0, {"after_identifier"}},  // Array access
        {"{", false, 0, {"after_dollar"}}  // Variable interpolation
    };
}

} // namespace NovaPad::Languages
